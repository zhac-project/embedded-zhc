// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: SIMON i7 1-gang smart switches (z2m v26.125.0 window): the
// pairing modelID S2100-1001, the neutral 70E838TY and the no-neutral 70E850TY.
// All three are m.onOff without power-on behaviour on the endpoints below; the
// 70E8xx models add tuya.modernExtend.tuyaLedIndicator() (genOnOff 0x8001
// backlight_mode off/normal/inverted).
// Upstream's S2100-1001 configure rewrites device.modelID to 70E838TY/70E850TY from the
// power source; that modelID remap is not portable, so S2100-1001 keeps plain
// relay control. The 70E8xx manufacturer-only fingerprints (_TZ2000_*) can't
// be expressed without a modelID: they are matched on S2100-1001 here, the
// pairing modelID those units report before the remap.
// z2m-source: simon.ts #S2100-1001 #70E838TY #70E850TY.
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
constexpr ::zhc::EndpointLabel kEndpoints[] = { {"switch", 1} };
constexpr BindingSpec kBindings[] = {
    {1, 0x0006},
};

constexpr const char* kModels_S2100_1001[] = { "S2100-1001" };
}  // namespace

extern const PreparedDefinition kDef_S2100_1001{
    .zigbee_models=kModels_S2100_1001, .zigbee_models_count=sizeof(kModels_S2100_1001)/sizeof(kModels_S2100_1001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="S2100-1001", .vendor="SIMON",
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
constexpr const char* kModels_D70E838TY[] = { "70E838TY" };
}  // namespace

extern const PreparedDefinition kDef_D70E838TY{
    .zigbee_models=kModels_D70E838TY, .zigbee_models_count=sizeof(kModels_D70E838TY)/sizeof(kModels_D70E838TY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="70E838TY", .vendor="SIMON",
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
constexpr const char* kModels_D70E850TY[] = { "70E850TY" };
}  // namespace

extern const PreparedDefinition kDef_D70E850TY{
    .zigbee_models=kModels_D70E850TY, .zigbee_models_count=sizeof(kModels_D70E850TY)/sizeof(kModels_D70E850TY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="70E850TY", .vendor="SIMON",
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
constexpr const char* kModels_D70E850TY_manu[] = { "S2100-1001" };
constexpr const char* kManus_D70E850TY_manu[] = { "_TZ2000_s93al43ewlu7wzwi" };
}  // namespace

extern const PreparedDefinition kDef_D70E850TY_manu{
    .zigbee_models=kModels_D70E850TY_manu, .zigbee_models_count=sizeof(kModels_D70E850TY_manu)/sizeof(kModels_D70E850TY_manu[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_D70E850TY_manu, .manufacturer_names_count=sizeof(kManus_D70E850TY_manu)/sizeof(kManus_D70E850TY_manu[0]),
    .model="70E850TY", .vendor="SIMON",
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
