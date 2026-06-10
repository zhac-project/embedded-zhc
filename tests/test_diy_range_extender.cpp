// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Tests driven by real wire captures from a DIY ESP32-C6 Zigbee device
// (Arduino `ZigbeeRangeExtender` example forked to expose temperature +
// humidity on a second endpoint). Model: "ZigbeeRangeExtender",
// manufacturer: "Espressif".
//
// The upstream z2m Espressif definition covers onOff + battery only —
// the DIY fork's extra clusters (0x0402 TempMeas, 0x0405 HumMeas) have
// no converters in the registry def. These tests pin down two things:
//
//   1. The generic fz_temperature / fz_humidity converters decode the
//      bytes the device actually puts on the wire (captured from a
//      real flash, not hand-authored).
//   2. Dispatching a minimal PreparedDefinition that lists those
//      generic converters (the shape the runtime fallback synthesises
//      in the firmware) produces the expected key/value outputs.
//
// If any of these break, the cluster-aware fallback in the firmware
// (`components/zhc_adapter/src/zhc_adapter_fallback.cpp`) will stop
// surfacing temp/humidity for this class of device.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "definitions/_generic/_shared.hpp"
#include "zhc/devices/espressif_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::uint8_t  src_endpoint,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xDA;
    f.data         = bytes;
    return f;
}

// Helper: compare floats with tolerance (raw-to-float / 100 has <0.01
// rounding headroom on host; be explicit so rounding differences
// between x86 and Xtensa don't nickname the test flaky).
bool nearly(float a, float b, float eps = 0.005f) {
    return std::fabs(a - b) < eps;
}

// ── Wire captures from the DIY device ────────────────────────────────
//
// The ZCL body is what the library consumes (decode_frame skips the AF
// framing outside — see components/zhc_adapter/src/zhc_adapter.cpp).
// Byte-for-byte copies of what appeared in znp_wire hex dumps:
//
//   RX cmd0=0x44 cmd1=0x81 len=28  (AF_INCOMING_MSG)
//   00 00 02 04 29 94 02 01 00 da 00 54 1e 02 00 00    <- AF header
//   08 08 76 0a 00 00 29 74 07                          <- ZCL body (8 B)
//     ^  -- ignore first byte; it's a duplicate LQI the
//           driver emits before the real ZCL start. Real ZCL is the
//           remaining 8 bytes below.
//
// Temperature report — attr 0x0000 measuredValue (int16 = 0x0774) →
// 1908 raw → 19.08 °C.
constexpr std::uint8_t kTempReport_1908[] = {
    0x08,              // frame control: SRV→CLI, disable default rsp
    0x76,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000
    0x29,              // type int16
    0x74, 0x07,        // value LE = 0x0774 = 1908
};

// Humidity report — attr 0x0000 measuredValue (u16 = 0x0D0C) →
// 3340 raw → 33.40 %.
constexpr std::uint8_t kHumReport_3340[] = {
    0x08,
    0x83,
    0x0A,
    0x00, 0x00,
    0x21,              // type u16
    0x0C, 0x0D,        // 0x0D0C = 3340
};

// Temperature report — second capture (attr int16 = 0x05DE) →
// 1502 raw → 15.02 °C. Used to exercise the sign handling path even
// though positive values don't trip it.
constexpr std::uint8_t kTempReport_1502[] = {
    0x08, 0x82, 0x0A,
    0x00, 0x00,
    0x29,
    0xDE, 0x05,        // 0x05DE = 1502
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// 1. Unit tests — individual generic converters on real DIY bytes.
// ─────────────────────────────────────────────────────────────────────

static void test_fz_temperature_19_08c_from_diy_ep2() {
    auto raw = build_frame(/*cluster=*/0x0402, /*src_ep=*/2, kTempReport_1908);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::kFzTemperature.fn.zcl_fn(msg, generic::kFzTemperature,
                                               def, ctx, out));

    const Value* t = out.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(nearly(t->f, 19.08f));
}

static void test_fz_temperature_15_02c_from_diy_ep2() {
    auto raw = build_frame(0x0402, 2, kTempReport_1502);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::kFzTemperature.fn.zcl_fn(msg, generic::kFzTemperature,
                                               def, ctx, out));

    const Value* t = out.find("temperature");
    assert(t && t->type == ValueType::Float);
    assert(nearly(t->f, 15.02f));
}

static void test_fz_humidity_33_40pct_from_diy_ep2() {
    auto raw = build_frame(0x0405, 2, kHumReport_3340);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";

    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
    PreparedDefinition def{};
    RuntimeContext ctx{};
    assert(generic::kFzHumidity.fn.zcl_fn(msg, generic::kFzHumidity,
                                            def, ctx, out));

    const Value* h = out.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(nearly(h->f, 33.40f));
}

// ─────────────────────────────────────────────────────────────────────
// 2. Registry introspection — document the gap the fallback fills.
// ─────────────────────────────────────────────────────────────────────
//
// The registry entry for this device is a PURE ROUTER: z2m's
// Espressif-ZigbeeRangeExtender has fromZigbee:[] + exposes:[] (configure
// binds genBasic only), so the def declares NO converters at all. The
// earlier auto-port carried a phantom kFzOnOff + kFzBattery bundle; the
// parity sweep stripped it to match z2m. This test now documents that the
// def covers NOTHING — so for this fingerprint the firmware's cluster-aware
// fallback (section 3) must synthesise every channel the device actually
// reports (temp/humidity/etc.), not just the gap above onOff+battery.

static void test_registry_def_is_empty_router() {
    const PreparedDefinition* def = nullptr;
    for (std::size_t i = 0; i < devices::espressif::kEspressifRegistryCount; ++i) {
        const auto* d = devices::espressif::kEspressifRegistry[i];
        if (d && d->model && std::strcmp(d->model, "Espressif-ZigbeeRangeExtender") == 0) {
            def = d;
            break;
        }
    }
    assert(def);
    assert(def->zigbee_models_count == 1);
    assert(std::strcmp(def->zigbee_models[0], "ZigbeeRangeExtender") == 0);

    // Pure router (z2m exposes:[] / fromZigbee:[]): no converters, no exposes.
    // Regression guard against the old phantom onOff + battery bundle.
    assert(def->from_zigbee_count == 0);
    assert(def->exposes_count == 0);
}

// ─────────────────────────────────────────────────────────────────────
// 3. Integration — dispatch through a synthetic "fallback-style"
// definition that only lists the generic sensor converters. This is
// the shape that components/zhc_adapter/src/zhc_adapter_fallback.cpp
// builds at runtime from the parsed Simple_Desc_rsp cluster list.
// ─────────────────────────────────────────────────────────────────────

namespace {

const FzConverter* const kFallbackSensorFz[] = {
    &generic::kFzTemperature,
    &generic::kFzHumidity,
};

constexpr Expose kFallbackExposes[] = {
    { "temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C",
      nullptr, nullptr, 0, ExposeCategory::State },
    { "humidity",    ExposeType::Numeric, Access::State, "%",
      nullptr, nullptr, 0, ExposeCategory::State },
};

const PreparedDefinition kFallbackSensorDef{
    .zigbee_models        = nullptr,
    .zigbee_models_count  = 0,
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names   = nullptr,
    .manufacturer_names_count = 0,
    .model                = "synthetic-th-sensor",
    .vendor               = "fallback",
    .meta                 = nullptr,
    .exposes              = kFallbackExposes,
    .exposes_count        = 2,
    .white_labels         = nullptr,
    .white_labels_count   = 0,
    .from_zigbee          = kFallbackSensorFz,
    .from_zigbee_count    = 2,
    .to_zigbee            = nullptr,
    .to_zigbee_count      = 0,
    .configure            = nullptr,
    .on_event             = nullptr,
};

}  // namespace

static void test_dispatch_temp_frame_through_synthetic_fallback_def() {
    auto raw = build_frame(0x0402, 2, kTempReport_1908);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, kFallbackSensorDef, raw, ctx);

    assert(result.any_matched);
    assert(result.merged.count == 1);
    const auto& kv = result.merged.items[0];
    assert(kv.key && std::strcmp(kv.key, "temperature") == 0);
    assert(kv.value.type == ValueType::Float);
    assert(nearly(kv.value.f, 19.08f));
}

static void test_dispatch_humidity_frame_through_synthetic_fallback_def() {
    auto raw = build_frame(0x0405, 2, kHumReport_3340);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, kFallbackSensorDef, raw, ctx);

    assert(result.any_matched);
    assert(result.merged.count == 1);
    const auto& kv = result.merged.items[0];
    assert(kv.key && std::strcmp(kv.key, "humidity") == 0);
    assert(kv.value.type == ValueType::Float);
    assert(nearly(kv.value.f, 33.40f));
}

static void test_dispatch_unknown_cluster_returns_no_match() {
    // Cluster 0x0300 ColorCtrl is NOT in the fallback def's fz list.
    // Pinning this behaviour protects against the earlier bug where a
    // stale SIMPLE_DESC_RSP parser fed a phantom 0x0300 cluster into
    // the fallback synth — the resulting def incorrectly claimed
    // colour support for a humidity sensor.
    constexpr std::uint8_t kColorTempReport[] = {
        0x08, 0x10, 0x0A,
        0x07, 0x00,        // attr id 0x0007 colorTemperature
        0x21,              // u16
        0x6E, 0x00,        // 110 mired
    };
    auto raw = build_frame(0x0300, 1, kColorTempReport);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "lightingColorCtrl";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, kFallbackSensorDef, raw, ctx);

    assert(!result.any_matched);
    assert(result.merged.count == 0);
}

// ─────────────────────────────────────────────────────────────────────
// Entry point.
// ─────────────────────────────────────────────────────────────────────

int main() {
    test_fz_temperature_19_08c_from_diy_ep2();
    test_fz_temperature_15_02c_from_diy_ep2();
    test_fz_humidity_33_40pct_from_diy_ep2();

    test_registry_def_is_empty_router();

    test_dispatch_temp_frame_through_synthetic_fallback_def();
    test_dispatch_humidity_frame_through_synthetic_fallback_def();
    test_dispatch_unknown_cluster_returns_no_match();
    return 0;
}
