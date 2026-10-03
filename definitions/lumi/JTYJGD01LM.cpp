// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Xiaomi JTYJ-GD-01LM/BW smoke sensor (Honeywell co-brand).
//
// Fire alarm arrives as an IAS zone status change: z2m lumi_smoke =
// ias_smoke_alarm_1 + a `test` flag (zoneStatus bit 1). Battery and
// smoke_density (tag 100) come in the Lumi MI-struct TLV. Sensitivity and
// selftest are both a u32 write to ssIasZone attr 0xFFF1 with the Lumi
// manufacturer code (z2m also sets a 35 s timeout — transport detail, not
// ported). z2m's hourly check-in quirk is not ported.
//
// z2m-source: lumi.ts #JTYJ-GD-01LM/BW (lumi.sensor_smoke).

#include "definitions/_generic/_shared.hpp"
#include "definitions/lumi/_shared.hpp"

namespace zhc::devices::lumi {
namespace {

// z2m numericAttributes2Payload case "100" for JTYJ-GD-01LM/BW.
constexpr ::zhc::lumi::LumiBasicOpts kBasicOpts{ .energy = false, .tag100_key = "smoke_density" };
constexpr FzConverter kFzBasic{
    .family            = FrameFamily::Zcl,
    .cluster           = "genBasic",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = &::zhc::lumi::fz_lumi_basic },
    .user_config       = &kBasicOpts,
};

// z2m lumi_sensitivity.
constexpr ::zhc::generic::ZclWriteLookup kSensitivityLut[] = {
    {"low", 0x04010000}, {"medium", 0x04020000}, {"high", 0x04030000},
};
constexpr ::zhc::generic::ZclWriteSpec kSensitivityWrite{
    "sensitivity", 0xFFF1, 0x23, 0x115F, kSensitivityLut, 3,
};
constexpr TzConverter kTzSensitivity{
    .key         = "sensitivity",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &::zhc::generic::tz_zcl_write_attr,
    .user_config = &kSensitivityWrite,
};

// z2m lumi_selftest: fixed value 0x03010000, whatever the input.
constexpr ::zhc::generic::ZclWriteSpec kSelftestWrite{
    "selftest", 0xFFF1, 0x23, 0x115F, nullptr, 0,
};
bool tz_selftest(std::string_view key, const Value&, const TzConverter& self,
                 const PreparedDefinition& def, RuntimeContext& ctx,
                 std::span<std::uint8_t> out, std::size_t& out_size) {
    Value v{}; v.type = ValueType::Uint; v.u = 0x03010000;
    return ::zhc::generic::tz_zcl_write_attr(key, v, self, def, ctx, out, out_size);
}
constexpr TzConverter kTzSelftest{
    .key         = "selftest",
    .cluster     = "ssIasZone",
    .cluster_id  = 0x0500,
    .command_id  = 0x02,
    .fn          = &tz_selftest,
    .user_config = &kSelftestWrite,
};

const FzConverter* const kFz[] = {
    &kFzBasic,
    &::zhc::generic::kFzIasSmokeAlarm,
    &::zhc::generic::kFzIasTestBit,
};
const TzConverter* const kTz[] = { &kTzSensitivity, &kTzSelftest };
constexpr const char* kModels[] = { "lumi.sensor_smoke" };
constexpr const char* kSensitivityValues[] = { "low", "medium", "high" };
constexpr const char* kSelftestValues[] = { "" };
}

constexpr Expose kExposes[] = {
    {"smoke", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery_low", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"tamper", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"sensitivity", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kSensitivityValues, 3,
     ExposeCategory::Config},
    {"smoke_density", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
    {"selftest", ExposeType::Enum, Access::Set, nullptr, nullptr, kSelftestValues, 1},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"test", ExposeType::Binary, Access::State, nullptr, "Test mode activated", nullptr, 0},
    {"device_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
    {"power_outage_count", ExposeType::Numeric, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings[] = {
    {1, 0x0000},
};

extern const PreparedDefinition kDefJTYJGD01LM{
    .zigbee_models=kModels,.zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .model = "JTYJGD01LM", .vendor = "Xiaomi",
    .meta = nullptr,
    .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels = nullptr, .white_labels_count = 0,
    .from_zigbee = kFz, .from_zigbee_count = sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee = kTz, .to_zigbee_count = sizeof(kTz)/sizeof(kTz[0]),
    .configure = nullptr, .on_event = nullptr,
.bindings=kBindings,.bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
};
}
