// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Legrand WNP10 smart plug-in switch with Netatmo (z2m v26.115.1 window).
// The model id is "Hospitality on off plug" padded with NULs, which end the
// C string. z2m: fz.on_off + binary_input_on_off (genBinaryInput presentValue
// -> state) + cluster_fc01 (LED modes) + m.commandsOnOff (action); tz on_off +
// led_mode. z2m's configure is an empty placeholder; only commandsOnOff's
// genOnOff bind is kept.
// z2m-source: legrand.ts #WNP10.
#include "definitions/_generic/_shared.hpp"
#include "definitions/legrand/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
constexpr ::zhc::generic::ZclAttrRow kBinInRows[] = { { 0x0055, "state", 1, nullptr, 0, ::zhc::generic::kZclAttrFlagBool } };
constexpr ::zhc::generic::ZclAttrMap kBinInMap{ kBinInRows, 1 };
constexpr FzConverter kFzBinaryInputState = ::zhc::generic::zcl_attr_fz("genBinaryInput", &kBinInMap);
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzOnOff,
    &kFzBinaryInputState,
    &::zhc::legrand::kFzClusterFc01,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::legrand::kTzLedInDark,
    &::zhc::legrand::kTzLedIfOn,
};
constexpr const char* kOpts3[] = { "on", "off", "toggle" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_in_dark", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"led_if_on", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, kOpts3, 3},
};
constexpr const char* kM[] = { "Hospitality on off plug" };
constexpr BindingSpec kBind[] = {
    {1, 0x0006},
};
}  // namespace

extern const PreparedDefinition kDef_WNP10{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WNP10", .vendor="Legrand",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
};

}  // namespace zhc::devices::legrand
