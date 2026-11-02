// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: SIMON i7 3-gang smart switches (z2m v26.125.0 window): the
// pairing modelID S2100-1003, the neutral 70E840TY and the no-neutral 70E852TY.
// All three are m.onOff without power-on behaviour on the endpoints below; the
// 70E8xx models add tuya.modernExtend.tuyaLedIndicator() (genOnOff 0x8001
// backlight_mode off/normal/inverted).
// Upstream's S2100-1003 configure rewrites device.modelID to 70E840TY/70E852TY from the
// power source; that modelID remap is not portable, so S2100-1003 keeps plain
// relay control. The 70E8xx manufacturer-only fingerprints (_TZ2000_*) can't
// be expressed without a modelID: they are matched on S2100-1003 here, the
// pairing modelID those units report before the remap.
// z2m-source: simon.ts #S2100-1003 #70E840TY #70E852TY.
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

namespace zhc::devices::simon {
namespace {
const FzConverter* const kFzPlain[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTzPlain[] = {
    &::zhc::generic::kTzOnOff,
};
const FzConverter* const kFzLed[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::tuya::kFzTuyaBacklightMode,
};
const TzConverter* const kTzLed[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::tuya::kTzTuyaBacklightMode,
};
constexpr const char* kBacklightValues[] = { "off", "normal", "inverted" };
constexpr Expose kExposesPlain[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};
constexpr Expose kExposesLed[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"backlight_mode", ExposeType::Enum, Access::StateSet, nullptr, "Mode of the backlight",
     kBacklightValues, 3, ExposeCategory::Config},
};
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"left", 1}, {"middle", 2}, {"right", 3} };
constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
    {2, 0x0006},
    {3, 0x0006},
};

constexpr const char* kModels_S2100_1003[] = { "S2100-1003" };
}  // namespace

extern const PreparedDefinition kDef_S2100_1003{
    .zigbee_models=kModels_S2100_1003, .zigbee_models_count=sizeof(kModels_S2100_1003)/sizeof(kModels_S2100_1003[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S2100-1003", .vendor="SIMON",
    .meta=nullptr, .exposes=kExposesPlain, .exposes_count=sizeof(kExposesPlain)/sizeof(kExposesPlain[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzPlain, .from_zigbee_count=sizeof(kFzPlain)/sizeof(kFzPlain[0]),
    .to_zigbee=kTzPlain, .to_zigbee_count=sizeof(kTzPlain)/sizeof(kTzPlain[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .endpoint_map       = kEndpoints,
    .endpoint_map_count = sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

namespace {
constexpr const char* kModels_D70E840TY[] = { "70E840TY" };
}  // namespace

extern const PreparedDefinition kDef_D70E840TY{
    .zigbee_models=kModels_D70E840TY, .zigbee_models_count=sizeof(kModels_D70E840TY)/sizeof(kModels_D70E840TY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="70E840TY", .vendor="SIMON",
    .meta=nullptr, .exposes=kExposesLed, .exposes_count=sizeof(kExposesLed)/sizeof(kExposesLed[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLed, .from_zigbee_count=sizeof(kFzLed)/sizeof(kFzLed[0]),
    .to_zigbee=kTzLed, .to_zigbee_count=sizeof(kTzLed)/sizeof(kTzLed[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .endpoint_map       = kEndpoints,
    .endpoint_map_count = sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

namespace {
constexpr const char* kModels_D70E840TY_manu[] = { "S2100-1003" };
constexpr const char* kManus_D70E840TY_manu[] = { "_TZ2000_8auyu8pgbug5fzl5" };
}  // namespace

extern const PreparedDefinition kDef_D70E840TY_manu{
    .zigbee_models=kModels_D70E840TY_manu, .zigbee_models_count=sizeof(kModels_D70E840TY_manu)/sizeof(kModels_D70E840TY_manu[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D70E840TY_manu, .manufacturer_names_count=sizeof(kManus_D70E840TY_manu)/sizeof(kManus_D70E840TY_manu[0]),
    .model="70E840TY", .vendor="SIMON",
    .meta=nullptr, .exposes=kExposesLed, .exposes_count=sizeof(kExposesLed)/sizeof(kExposesLed[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLed, .from_zigbee_count=sizeof(kFzLed)/sizeof(kFzLed[0]),
    .to_zigbee=kTzLed, .to_zigbee_count=sizeof(kTzLed)/sizeof(kTzLed[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .endpoint_map       = kEndpoints,
    .endpoint_map_count = sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

namespace {
constexpr const char* kModels_D70E852TY[] = { "70E852TY" };
}  // namespace

extern const PreparedDefinition kDef_D70E852TY{
    .zigbee_models=kModels_D70E852TY, .zigbee_models_count=sizeof(kModels_D70E852TY)/sizeof(kModels_D70E852TY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="70E852TY", .vendor="SIMON",
    .meta=nullptr, .exposes=kExposesLed, .exposes_count=sizeof(kExposesLed)/sizeof(kExposesLed[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLed, .from_zigbee_count=sizeof(kFzLed)/sizeof(kFzLed[0]),
    .to_zigbee=kTzLed, .to_zigbee_count=sizeof(kTzLed)/sizeof(kTzLed[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .endpoint_map       = kEndpoints,
    .endpoint_map_count = sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

namespace {
constexpr const char* kModels_D70E852TY_manu[] = { "S2100-1003" };
constexpr const char* kManus_D70E852TY_manu[] = { "_TZ2000_p4vghfngci5te7ol" };
}  // namespace

extern const PreparedDefinition kDef_D70E852TY_manu{
    .zigbee_models=kModels_D70E852TY_manu, .zigbee_models_count=sizeof(kModels_D70E852TY_manu)/sizeof(kModels_D70E852TY_manu[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D70E852TY_manu, .manufacturer_names_count=sizeof(kManus_D70E852TY_manu)/sizeof(kManus_D70E852TY_manu[0]),
    .model="70E852TY", .vendor="SIMON",
    .meta=nullptr, .exposes=kExposesLed, .exposes_count=sizeof(kExposesLed)/sizeof(kExposesLed[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFzLed, .from_zigbee_count=sizeof(kFzLed)/sizeof(kFzLed[0]),
    .to_zigbee=kTzLed, .to_zigbee_count=sizeof(kTzLed)/sizeof(kTzLed[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings, .bindings_count=sizeof(kBindings)/sizeof(kBindings[0]),
    .endpoint_map       = kEndpoints,
    .endpoint_map_count = sizeof(kEndpoints)/sizeof(kEndpoints[0]),
};

}  // namespace zhc::devices::simon
