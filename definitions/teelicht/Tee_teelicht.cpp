// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: teelicht rechargeable flickering tealight, zigbeeModel teelicht-c6
// (z2m v26.125.0 window, new vendor). m.light with colour temperature
// 370-555 mired (no startup, no power-on behaviour, no effect) + identify +
// battery with voltage. Custom cluster teelichtFlicker 0xFC00 (not
// manufacturer-specific): flicker_depth 0x0000 u8 %, flicker_speed 0x0001 u8
// (x10 on the wire, 0.3-4.0), flicker_effect 0x0002 enum8 candle/steady,
// power_state 0x0003 enum8 battery/charging/charged/no_cell (read-only).
// 0xFC00 resolves to "manuSpecificPhilips" in the shared cluster-name table,
// so the converters use that label, scoped by this def's lists.
// z2m-source: teelicht.ts #teelicht.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::teelicht {
namespace {

constexpr std::uint16_t kClusterFlicker = 0xFC00;

constexpr ::zhc::generic::ZclWriteLookup kFlickerEffects[] = { {"candle", 0}, {"steady", 1} };
constexpr ::zhc::generic::ZclWriteLookup kPowerStates[] = {
    {"battery", 0}, {"charging", 1}, {"charged", 2}, {"no_cell", 3} };
constexpr ::zhc::generic::ZclAttrRow kRowsFlicker[] = {
    {0x0000, "flicker_depth"},
    {0x0001, "flicker_speed", 10},
    {0x0002, "flicker_effect", 1, kFlickerEffects, 2},
    {0x0003, "power_state", 1, kPowerStates, 4},
};
constexpr ::zhc::generic::ZclAttrMap kMapFlicker{ kRowsFlicker, sizeof(kRowsFlicker)/sizeof(kRowsFlicker[0]) };
constexpr FzConverter kFzFlicker = ::zhc::generic::zcl_attr_fz("manuSpecificPhilips", &kMapFlicker);

constexpr ::zhc::generic::ZclWriteSpec kSpecDepth{ "flicker_depth", 0x0000, 0x20, 0, nullptr, 0 };
constexpr ::zhc::generic::ZclWriteSpec kSpecSpeed{ "flicker_speed", 0x0001, 0x20, 0, nullptr, 0, 0, 10 };
constexpr ::zhc::generic::ZclWriteSpec kSpecEffect{ "flicker_effect", 0x0002, 0x30, 0, kFlickerEffects, 2 };
constexpr TzConverter kTzDepth = ::zhc::generic::zcl_write_tz("manuSpecificPhilips", kClusterFlicker, &kSpecDepth);
constexpr TzConverter kTzSpeed = ::zhc::generic::zcl_write_tz("manuSpecificPhilips", kClusterFlicker, &kSpecSpeed);
constexpr TzConverter kTzEffect = ::zhc::generic::zcl_write_tz("manuSpecificPhilips", kClusterFlicker, &kSpecEffect);

const FzConverter* const kFz_teelicht[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
    &::zhc::generic::kFzBattery,
    &kFzFlicker,
};
const TzConverter* const kTz_teelicht[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
    &kTzDepth,
    &kTzSpeed,
    &kTzEffect,
};
constexpr const char* kModels_teelicht[] = { "teelicht-c6" };

constexpr const char* kFlickerEffectOpts[] = { "candle", "steady" };
constexpr const char* kPowerStateOpts[] = { "battery", "charging", "charged", "no_cell" };
constexpr Expose kExp_teelicht[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"brightness", ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"color_temp", ExposeType::Numeric, Access::StateSet, "mired", nullptr, nullptr, 0, ExposeCategory::State, 370, 555, 1},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"power_state", ExposeType::Enum, Access::State, nullptr,
     "On battery, charging over USB, charged, or on USB without a cell", kPowerStateOpts, 4,
     ExposeCategory::Diagnostic},
    {"flicker_depth", ExposeType::Numeric, Access::StateSet, "%",
     "How strongly the flame flickers; 0 is a steady light", nullptr, 0, ExposeCategory::Config, 0, 100, 1},
    {"flicker_speed", ExposeType::Numeric, Access::StateSet, "x",
     "Speed of the flicker (0.3-4.0); 1 is a calm candle", nullptr, 0, ExposeCategory::Config},
    {"flicker_effect", ExposeType::Enum, Access::StateSet, nullptr,
     "Candle flicker or a steady light", kFlickerEffectOpts, 2, ExposeCategory::Config},
};

constexpr BindingSpec kBindings_teelicht[] = {
    {1, 0x0006},
    {1, 0x0008},
    {1, 0x0300},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_teelicht{
    .zigbee_models=kModels_teelicht, .zigbee_models_count=sizeof(kModels_teelicht)/sizeof(kModels_teelicht[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="teelicht", .vendor="teelicht",
    .meta=nullptr, .exposes=kExp_teelicht, .exposes_count=sizeof(kExp_teelicht)/sizeof(kExp_teelicht[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_teelicht, .from_zigbee_count=sizeof(kFz_teelicht)/sizeof(kFz_teelicht[0]),
    .to_zigbee=kTz_teelicht, .to_zigbee_count=sizeof(kTz_teelicht)/sizeof(kTz_teelicht[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_teelicht, .bindings_count=sizeof(kBindings_teelicht)/sizeof(kBindings_teelicht[0]),
};

}  // namespace zhc::devices::teelicht
