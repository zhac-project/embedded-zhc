// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Tuya TS0001, the `_TZ3000_bzzgvet0` batch = Moes ZS-US1-LN
// 1-gang smart light switch (z2m v26.102.0, #12947).
//
// Upstream keeps this batch inside the generic TS0001 entry and switches the
// extra features on by manufacturerName at runtime (`tuyaOnOff({
// powerOnBehavior2, switchType, backlightModeOffOn })`). A converter here
// sees the manufacturer CODE, not the name, so the batch gets its own
// fingerprinted definition — the matcher prefers the manufacturer-specific
// candidate over the model-only TS0001.
//
//   power_on_behavior  genOnOff 0x8002 enum  off / on / previous
//   switch_type        0xE001   0xD030 enum  toggle / state / momentary
//   backlight_mode     genOnOff 0x8001 enum  OFF (0) / ON (1) — the two-state
//                      form; other Tuya switches publish the same attribute
//                      as the four-way `indicator_mode`, hence a local pair.
//
// z2m-source: tuya.ts #TS0001 (whiteLabel Moes ZS-US1-LN).
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::tuya {
namespace {

bool fz_backlight_off_on(const ::zhc::DecodedMessage& msg, const FzConverter&,
                         const PreparedDefinition&, ::zhc::RuntimeContext&,
                         ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const ::zhc::Value* v = msg.payload.find("32769");   // 0x8001 tuyaBacklightMode
    if (!v) return false;
    std::uint64_t raw;
    switch (v->type) {
        case ::zhc::ValueType::Uint: raw = v->u; break;
        case ::zhc::ValueType::Int:  raw = static_cast<std::uint64_t>(v->i); break;
        case ::zhc::ValueType::Bool: raw = v->b ? 1 : 0; break;
        default: return false;
    }
    ::zhc::Value o{}; o.type = ::zhc::ValueType::Bool; o.b = raw != 0;
    out.put("backlight_mode", o);
    return true;
}
constexpr FzConverter kFzBacklightOffOn{
    .family            = FrameFamily::Zcl,
    .cluster           = "genOnOff",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_backlight_off_on },
    .user_config       = nullptr,
};
constexpr ::zhc::generic::ZclWriteLookup kBacklightLut[] = { {"OFF", 0}, {"ON", 1} };
constexpr ::zhc::generic::ZclWriteSpec kSpecBacklight{
    "backlight_mode", 0x8001, 0x30, 0, kBacklightLut, 2 };
constexpr TzConverter kTzBacklightOffOn{
    "backlight_mode", "genOnOff", 0x0006, 0x02,
    &::zhc::generic::tz_zcl_write_attr, &kSpecBacklight };

const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaPowerOnBehavior,
    &::zhc::tuya::kFzTuyaSwitchType,
    &kFzBacklightOffOn,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaPowerOnBehavior,
    &::zhc::tuya::kTzTuyaSwitchType,
    &kTzBacklightOffOn,
    &::zhc::tuya::kTzTuyaChildLock,
};

constexpr const char* kPowerOnOpts[]   = { "off", "on", "previous" };
constexpr const char* kSwitchTypeOpts[] = { "toggle", "state", "momentary" };

constexpr Expose kExposes[] = {
    { "state",             ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "power_on_behavior", ExposeType::Enum,   Access::StateSet, nullptr, nullptr, kPowerOnOpts, 3, ExposeCategory::Config },
    { "switch_type",       ExposeType::Enum,   Access::StateSet, nullptr, nullptr, kSwitchTypeOpts, 3, ExposeCategory::Config },
    { "backlight_mode",    ExposeType::Binary, Access::StateSet, nullptr, "Mode of the backlight", nullptr, 0, ExposeCategory::Config },
};
constexpr BindingSpec kBindings[] = { {1, 0x0006} };
constexpr WhiteLabel kWhiteLabels[] = { {"Moes", "ZS-US1-LN"} };
constexpr const char* kModels[] = { "TS0001" };
constexpr const char* kManus[]  = { "_TZ3000_bzzgvet0" };

}  // namespace

extern const PreparedDefinition kDef_TS0001_bzzgvet0{
    .zigbee_models=kModels, .zigbee_models_count=sizeof(kModels)/sizeof(kModels[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus, .manufacturer_names_count=sizeof(kManus)/sizeof(kManus[0]),
    .model="TS0001", .vendor="Tuya",
    .meta=nullptr, .exposes=kExposes, .exposes_count=sizeof(kExposes)/sizeof(kExposes[0]),
    .white_labels=kWhiteLabels, .white_labels_count=sizeof(kWhiteLabels)/sizeof(kWhiteLabels[0]),
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .reports=::zhc::tuya::kReportsOnOff_1ep, .reports_count=::zhc::tuya::kReportsOnOff_1ep_count,
};

}  // namespace zhc::devices::tuya
