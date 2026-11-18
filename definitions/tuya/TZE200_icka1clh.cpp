// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: TS0601_cover_4 cover motor (`_TZE200_icka1clh`, `_TZE200_zah67ekd`),
// white label Moes AM43-0.45/40-ES-EB — z2m parity v26.133.0.
// GRADUATED from definitions/tuya/generated/Gen__TZE200_icka1clh.cpp.
// z2m v26.95.0 renamed the motor_direction labels: the inline lookup
// {forward:0, back:1} became valueConverter.tubularMotorDirection,
// {normal:0, reversed:1}. Wire values unchanged; only the published
// strings differ. Renamed for z2m parity (product decision).
// z2m #13597: DP1 is the last control command, not the state, so its decode
// is ignored (the row stays for OPEN/STOP/CLOSE writes); DP2/DP3 publish the
// position plus a state derived from it (0 -> CLOSE, else OPEN); DP7
// work_state is ignored. Lift mode only: the tilt mode (sticky per-device
// openingMode + dynamic exposes) is not ported.
// z2m-source: tuya.ts #TS0601_cover_4.
#include "definitions/tuya/_shared.hpp"
#include "definitions/tuya/dp.hpp"
#include "definitions/tuya/extend.hpp"
namespace zhc::devices::tuya {
namespace {

// z2m am43Position (lift mode): {position: v, state: v === 0 ? CLOSE : OPEN}.
bool am43_position(const ::zhc::tuya::TuyaDpMapEntry& e, const Value& raw, RuntimeContext&,
                   FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (raw.type != ValueType::Int) return false;
    Value p{}; p.type = ValueType::Int; p.i = raw.i;
    out.put(e.out_key, p);
    Value s{}; s.type = ValueType::StringRef; s.str = raw.i == 0 ? "CLOSE" : "OPEN";
    out.put("state", s);
    return true;
}

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_state_dp1[] = {
    { 0, "OPEN" },
    { 1, "STOP" },
    { 2, "CLOSE" },
    { 3, "CONTINUE" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_motor_direction_dp5[] = {
    { 0, "normal" },
    { 1, "reversed" },
};

constexpr ::zhc::tuya::TuyaEnumEntry kEnum__TZE200_icka1clh_opening_mode_dp101[] = {
    { 0, "tilt" },
    { 1, "lift" },
};

constexpr ::zhc::tuya::TuyaDpMapEntry kEntries__TZE200_icka1clh[] = {
    // Decode ignored (upstream [1, null, null] comes first); the Enum still encodes writes.
    { 1, "state", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_state_dp1, sizeof(kEnum__TZE200_icka1clh_state_dp1)/sizeof(kEnum__TZE200_icka1clh_state_dp1[0]),
      0, 0.0f, &::zhc::tuya::tuya_dp_expand_ignore, nullptr },
    { 2, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.0f, &am43_position, nullptr },
    { 3, "position", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0, 0.0f, &am43_position, nullptr },
    { 5, "motor_direction", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_motor_direction_dp5, sizeof(kEnum__TZE200_icka1clh_motor_direction_dp5)/sizeof(kEnum__TZE200_icka1clh_motor_direction_dp5[0]) },
    ::zhc::tuya::dp::ignored(7),
    { 101, "opening_mode", ::zhc::TuyaDpType::Enum, 1, kEnum__TZE200_icka1clh_opening_mode_dp101, sizeof(kEnum__TZE200_icka1clh_opening_mode_dp101)/sizeof(kEnum__TZE200_icka1clh_opening_mode_dp101[0]) },
    { 102, "factory_reset", ::zhc::TuyaDpType::Bool, 1, nullptr, 0, 0 },
    { 103, "set_upper_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 104, "set_bottom_limit", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
    { 105, "motor_speed", ::zhc::TuyaDpType::Numeric, 1, nullptr, 0, 0 },
};
constexpr ::zhc::tuya::TuyaDatapointMap kMap__TZE200_icka1clh{ kEntries__TZE200_icka1clh, sizeof(kEntries__TZE200_icka1clh)/sizeof(kEntries__TZE200_icka1clh[0]) };
constexpr FzConverter kFzDp__TZE200_icka1clh{
    .family            = FrameFamily::TuyaDp,
    .cluster           = "manuSpecificTuya",
    .type_mask         = type_bit(MessageType::Command),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .tuya_fn = &::zhc::tuya::fz_tuya_datapoints },
    .user_config       = &kMap__TZE200_icka1clh,
};
const FzConverter* const kFz__TZE200_icka1clh[] = {
    &::zhc::tuya::kFzTuyaMcuSyncTime,
    &kFzDp__TZE200_icka1clh,
};
constexpr TzConverter kTzDp__TZE200_icka1clh{
    .key         = nullptr,                // wildcard — claims any mapped key
    .cluster     = "manuSpecificTuya",
    .cluster_id  = 0xEF00,
    .command_id  = 0x00,
    .fn          = &::zhc::tuya::tz_tuya_datapoints,
    .user_config = &kMap__TZE200_icka1clh,
};
const TzConverter* const kTz__TZE200_icka1clh[] = { &kTzDp__TZE200_icka1clh };
constexpr const char* kM__TZE200_icka1clh[] = { "TS0601" };
constexpr const char* kN__TZE200_icka1clh[] = { "_TZE200_zah67ekd", "_TZE200_icka1clh" };

constexpr const char* kStOpts[]   = { "OPEN", "CLOSE", "STOP" };
constexpr const char* kDirOpts[]  = { "normal", "reversed" };
constexpr const char* kModeOpts[] = { "tilt", "lift" };
// set_upper_limit / set_bottom_limit are not exposed: upstream writes them as
// a string DP ("SET"), which tz_tuya_datapoints cannot send yet.
constexpr Expose kExposes__TZE200_icka1clh[] = {
    { "state",           ExposeType::Enum,    Access::StateSet, nullptr, nullptr, kStOpts, 3 },
    { "position",        ExposeType::Numeric, Access::StateSet, "%",     nullptr, nullptr, 0,
      ExposeCategory::State, 0, 100, 0 },
    { "motor_direction", ExposeType::Enum,    Access::StateSet, nullptr, "Motor rotation direction", kDirOpts, 2,
      ExposeCategory::Config },
    { "motor_speed",     ExposeType::Numeric, Access::StateSet, "rpm",   "Motor speed", nullptr, 0,
      ExposeCategory::State, 0, 255, 0 },
    { "opening_mode",    ExposeType::Enum,    Access::StateSet, nullptr, "Opening mode", kModeOpts, 2 },
    { "factory_reset",   ExposeType::Binary,  Access::Set,      nullptr, "Factory reset the device", nullptr, 0 },
};
}  // namespace

constexpr WhiteLabel kWhiteLabels_Gen_TZE200_icka1clh[] = {
    {"Moes","AM43-0.45/40-ES-EB"},
};
extern const PreparedDefinition kDef_TZE200_icka1clh{
    .zigbee_models=kM__TZE200_icka1clh,.zigbee_models_count=1,
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN__TZE200_icka1clh,.manufacturer_names_count=sizeof(kN__TZE200_icka1clh)/sizeof(kN__TZE200_icka1clh[0]),
    .model="TS0601__TZE200_icka1clh",.vendor="Tuya",
    .meta=nullptr,.exposes=kExposes__TZE200_icka1clh,.exposes_count=sizeof(kExposes__TZE200_icka1clh)/sizeof(kExposes__TZE200_icka1clh[0]),
    .white_labels=kWhiteLabels_Gen_TZE200_icka1clh, .white_labels_count=sizeof(kWhiteLabels_Gen_TZE200_icka1clh)/sizeof(kWhiteLabels_Gen_TZE200_icka1clh[0]),
    .from_zigbee=kFz__TZE200_icka1clh,
    .from_zigbee_count=sizeof(kFz__TZE200_icka1clh)/sizeof(kFz__TZE200_icka1clh[0]),
    .to_zigbee=kTz__TZE200_icka1clh,
    .to_zigbee_count=sizeof(kTz__TZE200_icka1clh)/sizeof(kTz__TZE200_icka1clh[0]),
    .configure=::zhc::tuya::extend::tuya_base_configure(),
    .on_event=nullptr,
    .power_source_override=0x01 };  // upstream configure: powerSource "Mains (single phase)"
}  // namespace zhc::devices::tuya
