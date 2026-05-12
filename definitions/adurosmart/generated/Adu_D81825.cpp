// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Adurosmart 81825 — hand-rewritten 2026-04-28s, action labels
// realigned 2026-04-28t post-sweep.
// ERIA smart wireless dimming switch (4-button battery-powered dimmer).
// z2m-source: adurosmart.ts #81825.
//
// z2m exposes [action(on/off/up/down)]. Inline Fz emits the literal
// "up"/"down" labels (z2m-exact) instead of the generic kFzCommandStep
// "brightness_step_up"/"brightness_step_down".
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::adurosmart {
namespace {

constexpr const char* kModels_D81825[] = { "Adurolight_NCC" };

constexpr const char* kActions_D81825[] = { "on", "off", "up", "down" };

// genLevelCtrl cmd 0x02 (Step). Body: step_mode u8 (0=up, 1=down),
// step_size u8, transition_time u16 LE.
bool fz_d81825_step(const ::zhc::DecodedMessage& msg,
                     const ::zhc::FzConverter&,
                     const ::zhc::PreparedDefinition&,
                     ::zhc::RuntimeContext&,
                     ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    if (msg.raw_body.size() < 1) return false;
    const std::uint8_t mode = msg.raw_body[0];
    const char* label = (mode == 0) ? "up" : (mode == 1) ? "down" : nullptr;
    if (!label) return false;
    ::zhc::Value a{}; a.type = ::zhc::ValueType::StringRef; a.str = label;
    out.put("action", a);
    return true;
}

constexpr ::zhc::FzConverter kFzD81825Step{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::Command),
    .command_id        = 0x02,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ClientToServer,
    .fn                = { .zcl_fn = fz_d81825_step },
    .user_config       = nullptr,
};

const FzConverter* const kFz_D81825[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &kFzD81825Step,
};

}  // namespace

constexpr Expose kAutoExposes[] = {
    { "action", ExposeType::Enum, Access::State, nullptr, nullptr,
      kActions_D81825,
      sizeof(kActions_D81825)/sizeof(kActions_D81825[0]) },
};

constexpr BindingSpec kAutoBindings[] = {
    { 1, 0x0006 },  // genOnOff (z2m configure)
    { 1, 0x0008 },  // genLevelCtrl (z2m configure)
};

extern const PreparedDefinition kDef_D81825{
    .zigbee_models=kModels_D81825,
    .zigbee_models_count=sizeof(kModels_D81825)/sizeof(kModels_D81825[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="81825", .vendor="Adurosmart",
    .meta=nullptr,
    .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D81825, .from_zigbee_count=sizeof(kFz_D81825)/sizeof(kFz_D81825[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::adurosmart
