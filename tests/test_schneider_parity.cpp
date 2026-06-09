// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Parity tests for three Schneider Electric (Wiser) devices whose
// auto-generated ports dropped a core decoder. Each was graduated to a
// Tier-1/2 parent override (definitions/schneider/Sch_*.cpp).
//
//   * EER53000  — Wiser radiator thermostat (VACT), zigbeeModel "EH-ZB-VACT".
//       The auto-emit produced a bare on/off switch (state/battery only);
//       z2m decodes a full climate. The override wires the generic thermostat
//       converters so local_temperature / current_heating_setpoint /
//       system_mode surface (climate ships FLAT — project rule).
//   * 550B1024  — Temperature & humidity sensor, zigbeeModel "CCT593011_AS".
//       The auto-emit produced a battery-only port; z2m fromZigbee is
//       [fz.humidity, fz.temperature, fz.battery]. The override adds the
//       generic temperature + humidity converters + bindings.
//   * CCTFR6400 — "Temp/Humidity measurement with thermostat interface".
//       The thermostat half was wired but the named humidity measurement was
//       missing (z2m carries fz.humidity + e.humidity() + msRelativeHumidity
//       bind). The override adds kFzHumidity + expose + binding 0x0405.
//
// These tests pin the expose lists / bindings and that real ZCL reports
// decode through the def at the dispatch_from_zigbee boundary.

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/types.hpp"
#include "zhc/zcl/decoder.hpp"

namespace zhc::devices::schneider {
extern const PreparedDefinition kDef_EER53000;
extern const PreparedDefinition kDef_D550B1024;
extern const PreparedDefinition kDef_CCTFR6400;
}  // namespace zhc::devices::schneider

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                            std::uint8_t  src_endpoint,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = src_endpoint;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

bool nearly(float a, float b, float eps = 0.005f) {
    return std::fabs(a - b) < eps;
}

bool has_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i) {
        const char* n = def.exposes[i].name;
        if (n && std::strcmp(n, name) == 0) return true;
    }
    return false;
}

bool has_binding(const PreparedDefinition& def, std::uint16_t cluster_id) {
    for (std::size_t i = 0; i < def.bindings_count; ++i) {
        if (def.bindings[i].cluster_id == cluster_id) return true;
    }
    return false;
}

bool has_fz_cluster(const PreparedDefinition& def, const char* cluster) {
    for (std::uint8_t i = 0; i < def.from_zigbee_count; ++i) {
        const FzConverter* fz = def.from_zigbee[i];
        if (fz && fz->cluster && std::strcmp(fz->cluster, cluster) == 0) return true;
    }
    return false;
}

// hvacThermostat (0x0201) LocalTemperature attr 0x0000, int16.
//   0x093A = 2362 raw -> z2m 23.62 °C (ZHC emits the raw int).
constexpr std::uint8_t kThermLocalTemp_2362[] = {
    0x08,              // FC: SRV->CLI, disable default rsp
    0x42,              // tsn
    0x0A,              // Report Attributes
    0x00, 0x00,        // attr id 0x0000 (LocalTemperature)
    0x29,              // type int16
    0x3A, 0x09,        // LE value 0x093A = 2362
};

// msTemperatureMeasurement (0x0402) attr 0x0000 measuredValue, int16.
//   0x093A = 2362 raw -> 23.62 °C.
constexpr std::uint8_t kTempReport_2362[] = {
    0x08, 0x43, 0x0A,
    0x00, 0x00,
    0x29,              // int16
    0x3A, 0x09,        // 0x093A = 2362
};

// msRelativeHumidity (0x0405) attr 0x0000 measuredValue, u16.
//   0x1450 = 5200 raw -> 52.00 %.
constexpr std::uint8_t kHumReport_5200[] = {
    0x08, 0x44, 0x0A,
    0x00, 0x00,
    0x21,              // u16
    0x50, 0x14,        // 0x1450 = 5200
};

}  // namespace

// ─────────────────────────────────────────────────────────────────────
// EER53000 — TRV that was ported as a switch.
// ─────────────────────────────────────────────────────────────────────

static void test_eer53000_exposes_and_bindings_cover_thermostat() {
    const auto& def = devices::schneider::kDef_EER53000;
    // The gap this override fixes: a battery TRV must surface a climate.
    assert(has_expose(def, "local_temperature"));
    assert(has_expose(def, "current_heating_setpoint"));
    assert(has_expose(def, "system_mode"));
    assert(has_expose(def, "battery"));
    assert(has_fz_cluster(def, "hvacThermostat"));
    assert(has_binding(def, 0x0201));  // hvacThermostat bound
    assert(has_binding(def, 0x0001));  // genPowerCfg (battery)
}

static void test_eer53000_decodes_local_temperature() {
    auto raw = build_frame(/*cluster=*/0x0201, /*src_ep=*/1, kThermLocalTemp_2362);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "hvacThermostat";

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{}, devices::schneider::kDef_EER53000,
        raw, ctx);

    assert(result.any_matched);
    const Value* t = result.merged.find("local_temperature");
    assert(t && t->type == ValueType::Int);
    assert(t->i == 2362);  // raw s16; z2m scales /100 downstream
}

// ─────────────────────────────────────────────────────────────────────
// 550B1024 — temp/humidity sensor that was ported battery-only.
// ─────────────────────────────────────────────────────────────────────

static void test_550b1024_exposes_and_bindings_cover_sensors() {
    const auto& def = devices::schneider::kDef_D550B1024;
    assert(has_expose(def, "temperature"));  // ← gap fixed
    assert(has_expose(def, "humidity"));     // ← gap fixed
    assert(has_expose(def, "battery"));
    assert(has_fz_cluster(def, "msTemperatureMeasurement"));
    assert(has_fz_cluster(def, "msRelativeHumidity"));
    assert(has_binding(def, 0x0402));
    assert(has_binding(def, 0x0405));
    assert(has_binding(def, 0x0001));
}

static void test_550b1024_decodes_temperature_and_humidity() {
    // temperature
    {
        auto raw = build_frame(0x0402, 1, kTempReport_2362);
        DecodedMessage msg{};
        assert(decode_frame(raw, {}, msg));
        msg.cluster = "msTemperatureMeasurement";
        RuntimeContext ctx{};
        const auto result = dispatch_from_zigbee(
            msg, std::span<const TuyaDpRecord>{},
            devices::schneider::kDef_D550B1024, raw, ctx);
        assert(result.any_matched);
        const Value* t = result.merged.find("temperature");
        assert(t && t->type == ValueType::Float);
        assert(nearly(t->f, 23.62f));
    }
    // humidity
    {
        auto raw = build_frame(0x0405, 1, kHumReport_5200);
        DecodedMessage msg{};
        assert(decode_frame(raw, {}, msg));
        msg.cluster = "msRelativeHumidity";
        RuntimeContext ctx{};
        const auto result = dispatch_from_zigbee(
            msg, std::span<const TuyaDpRecord>{},
            devices::schneider::kDef_D550B1024, raw, ctx);
        assert(result.any_matched);
        const Value* h = result.merged.find("humidity");
        assert(h && h->type == ValueType::Float);
        assert(nearly(h->f, 52.00f));
    }
}

// ─────────────────────────────────────────────────────────────────────
// CCTFR6400 — thermostat-with-humidity that was missing humidity.
// ─────────────────────────────────────────────────────────────────────

static void test_cctfr6400_keeps_thermostat_and_gains_humidity() {
    const auto& def = devices::schneider::kDef_CCTFR6400;
    assert(has_expose(def, "humidity"));            // ← gap fixed
    assert(has_expose(def, "local_temperature"));   // retained
    assert(has_expose(def, "current_heating_setpoint"));
    assert(has_fz_cluster(def, "msRelativeHumidity"));
    assert(has_fz_cluster(def, "hvacThermostat"));
    assert(has_binding(def, 0x0405));
    assert(has_binding(def, 0x0201));
}

static void test_cctfr6400_decodes_humidity() {
    auto raw = build_frame(0x0405, 1, kHumReport_5200);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msRelativeHumidity";
    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(
        msg, std::span<const TuyaDpRecord>{},
        devices::schneider::kDef_CCTFR6400, raw, ctx);
    assert(result.any_matched);
    const Value* h = result.merged.find("humidity");
    assert(h && h->type == ValueType::Float);
    assert(nearly(h->f, 52.00f));
}

int main() {
    test_eer53000_exposes_and_bindings_cover_thermostat();
    test_eer53000_decodes_local_temperature();
    test_550b1024_exposes_and_bindings_cover_sensors();
    test_550b1024_decodes_temperature_and_humidity();
    test_cctfr6400_keeps_thermostat_and_gains_humidity();
    test_cctfr6400_decodes_humidity();
    return 0;
}
