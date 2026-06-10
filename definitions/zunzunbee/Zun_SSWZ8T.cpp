// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Zunzunbee SSWZ8T — hand-curated (was remote-as-switch).
// Slate switch — 8-button battery scene controller. z2m role:
//   fromZigbee: fzLocal.fzZunzunbeeSlateSwitchIAS  (ssIasZone zoneStatus →
//     action = button_<n>_<short|long>_press), extend: m.battery() +
//     m.temperature(); exposes a single composite `action` enum. There is
//     NO controllable on/off — the device only reports button presses.
// The auto-port collapsed it into a settable on/off `state`
// (kFzOnOff + kTzOnOff binding genOnOff 0x0006), a dead control that also
// dropped the 8-button action decode and the temperature channel.
// Replaced with the vendor kFzZunzunbeeSlateSwitch (custom zoneStatus
// decode), kFzBattery + kFzTemperature, an `action` enum expose +
// temperature, and dropped the bogus to_zigbee + genOnOff bind.
// z2m-source: zunzunbee.ts #SSWZ8T.
#include "definitions/_generic/_shared.hpp"
#include "definitions/zunzunbee/_shared.hpp"

namespace zhc::devices::zunzunbee {
namespace {
const FzConverter* const kFz_SSWZ8T[] = {
    &::zhc::zunzunbee::kFzZunzunbeeSlateSwitch,
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
};
constexpr const char* kModels_SSWZ8T[] = { "SSWZ8T" };
// z2m fingerprint gates on manufacturerName "zunzunbee" as well as the
// model id, so mirror that here (model-only would over-match clones).
constexpr const char* kManus_SSWZ8T[] = { "zunzunbee" };

}  // namespace


// --- hand-curated exposes / bindings (was auto-generated) ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},   // genPowerCfg (battery)
    {1, 0x0402},   // msTemperatureMeasurement
};
// --- end hand-curated block ---

extern const PreparedDefinition kDef_SSWZ8T{
    .zigbee_models=kModels_SSWZ8T, .zigbee_models_count=sizeof(kModels_SSWZ8T)/sizeof(kModels_SSWZ8T[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_SSWZ8T, .manufacturer_names_count=sizeof(kManus_SSWZ8T)/sizeof(kManus_SSWZ8T[0]),
    .model="SSWZ8T", .vendor="Zunzunbee",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_SSWZ8T, .from_zigbee_count=sizeof(kFz_SSWZ8T)/sizeof(kFz_SSWZ8T[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::zunzunbee
