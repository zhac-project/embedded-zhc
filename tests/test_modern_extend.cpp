// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: tests for the modernExtend helper pack.
//
// Each helper gets a synthetic "test device" whose PreparedDefinition
// references only that helper — proves the helper works end-to-end
// without pulling in any vendor-specific code.

#include <cassert>
#include <cstdint>
#include <cstring>

#include "zhc/modern_extend.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"

using namespace zhc;

namespace {

InboundApsFrame build_frame(std::uint16_t cluster,
                             std::span<const std::uint8_t> bytes) {
    InboundApsFrame f{};
    f.cluster_id   = cluster;
    f.src_endpoint = 1;
    f.dst_endpoint = 1;
    f.linkquality  = 0xC8;
    f.data         = bytes;
    return f;
}

// msTemperatureMeasurement (0x0402) attr 0x0000 int16 = 2595 (25.95°C).
constexpr std::uint8_t kTempFrame[] = {
    0x18, 0x01, 0x0A,          // header
    0x00, 0x00, 0x29, 0x23, 0x0A,  // attr 0x0000, type 0x29 int16, value 2595 LE
};

// genOnOff (0x0006) attr 0x0000 bool = 1.
constexpr std::uint8_t kBoolFrame[] = {
    0x18, 0x02, 0x0A,
    0x00, 0x00, 0x10, 0x01,
};

// genBasic (0x0000) attr 0x0007 u8 = 3 (z2m powerSource "battery").
constexpr std::uint8_t kEnumFrame[] = {
    0x18, 0x03, 0x0A,
    0x07, 0x00, 0x20, 0x03,
};

PreparedDefinition make_def(const FzConverter* const* fz, std::uint8_t n,
                             const char* model, const char* cluster_name) {
    PreparedDefinition def{};
    static const char* models[] = { "SYNTH" };
    def.zigbee_models       = models;
    def.zigbee_models_count = 1;
    def.model               = model;
    def.vendor              = "Test";
    def.from_zigbee         = fz;
    def.from_zigbee_count   = n;
    (void)cluster_name;
    return def;
}

}  // namespace

// ── Numeric ────────────────────────────────────────────────────────

struct temperature_opts {
    static constexpr const char*   name    = "temperature";
    static constexpr const char*   unit    = "C";
    static constexpr const char*   cluster = "msTemperatureMeasurement";
    static constexpr std::uint16_t attr    = 0x0000;
    static constexpr std::uint32_t divisor = 100;
};
using TempHelper = m::Numeric<temperature_opts>;

static void test_numeric_decodes_scaled_temperature() {
    auto raw = build_frame(0x0402, kTempFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "msTemperatureMeasurement";

    const FzConverter* const fz_list[] = { &TempHelper::converter };
    auto def = make_def(fz_list, 1, "GEN_TEMP", "msTemperatureMeasurement");

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* t = result.merged.find("temperature");
    assert(t && t->type == ValueType::Float);
    // 2595 / 100 = 25.95 ± rounding.
    const float diff = t->f - 25.95f;
    assert(diff < 0.001f && diff > -0.001f);
}

// ── Binary ─────────────────────────────────────────────────────────

struct state_opts {
    static constexpr const char*   name    = "state";
    static constexpr const char*   cluster = "genOnOff";
    static constexpr std::uint16_t attr    = 0x0000;
};
using StateHelper = m::Binary<state_opts>;

static void test_binary_decodes_bool() {
    auto raw = build_frame(0x0006, kBoolFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genOnOff";

    const FzConverter* const fz_list[] = { &StateHelper::converter };
    auto def = make_def(fz_list, 1, "GEN_BIN", "genOnOff");

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* s = result.merged.find("state");
    assert(s && s->type == ValueType::Bool && s->b == true);
}

// ── EnumLookup ─────────────────────────────────────────────────────

struct power_src_opts {
    static constexpr const char*   name    = "power_source";
    static constexpr const char*   cluster = "genBasic";
    static constexpr std::uint16_t attr    = 0x0007;
    static constexpr std::array<m::EnumEntry, 4> entries{{
        {0, "unknown"},
        {1, "mains"},
        {3, "battery"},
        {4, "dc"},
    }};
};
using PowerSrcHelper = m::EnumLookup<power_src_opts>;

static void test_enum_lookup_maps_u8() {
    auto raw = build_frame(0x0000, kEnumFrame);
    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = "genBasic";

    const FzConverter* const fz_list[] = { &PowerSrcHelper::converter };
    auto def = make_def(fz_list, 1, "GEN_ENUM", "genBasic");

    RuntimeContext ctx{};
    const auto result = dispatch_from_zigbee(msg, {}, def, raw, ctx);
    assert(result.any_matched);

    const Value* ps = result.merged.find("power_source");
    assert(ps && ps->type == ValueType::StringRef);
    assert(std::strcmp(ps->str, "battery") == 0);
}

// ── Configure helpers ─────────────────────────────────────────────

static void test_identify_returns_callable() {
    auto cfg = m::identify();
    assert(cfg != nullptr);
    RuntimeContext ctx{};
    cfg(0, ctx);   // no-op on host — just confirms no crash.
}

static void test_bind_cluster_is_unique_per_cluster() {
    auto a = m::bind_cluster<0x0006>();
    auto b = m::bind_cluster<0x0500>();
    auto a2 = m::bind_cluster<0x0006>();
    assert(a != b);     // different template instantiations
    assert(a == a2);    // same template arg → same pointer
}

// ── DeviceEndpoints ───────────────────────────────────────────────

constexpr m::DeviceEndpoints<3> kEndpoints{
    std::array<m::EndpointEntry, 3>{{
        {1, "left"},
        {2, "right"},
        {3, "both"},
    }},
};

static void test_device_endpoints_label_lookup() {
    assert(std::strcmp(kEndpoints.label_for(1), "left")  == 0);
    assert(std::strcmp(kEndpoints.label_for(2), "right") == 0);
    assert(std::strcmp(kEndpoints.label_for(3), "both")  == 0);
    assert(kEndpoints.label_for(99) == nullptr);
}

// ── Metadata tags ─────────────────────────────────────────────────

constexpr m::ForcePowerSource kBatteryPowered{
    .source = m::PowerSource::Battery,
};

constexpr m::ForceDeviceType kRouter{
    .kind = m::ForceDeviceType::Kind::Router,
};

constexpr m::QuirkCheckinInterval kCheckin{ .interval_ms = 60000 };

constexpr m::OtaPassthrough kOta{};

static void test_metadata_tags_round_trip() {
    assert(kBatteryPowered.source == m::PowerSource::Battery);
    assert(kRouter.kind           == m::ForceDeviceType::Kind::Router);
    assert(kCheckin.interval_ms   == 60000);
    (void)kOta;   // presence-only marker; nothing to assert.
}

int main() {
    test_numeric_decodes_scaled_temperature();
    test_binary_decodes_bool();
    test_enum_lookup_maps_u8();
    test_identify_returns_callable();
    test_bind_cluster_is_unique_per_cluster();
    test_device_endpoints_label_lookup();
    test_metadata_tags_round_trip();
    return 0;
}
