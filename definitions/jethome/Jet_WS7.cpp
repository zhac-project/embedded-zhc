// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: JetHome WS7 — hand-curated (was phantom-on/off + dead channels).
//
// 3-channel battery discrete-input module. z2m role:
//   fromZigbee = [fz.battery, jetHome.fz.multiStateAction]
//   toZigbee   = []  (uncontrollable)
//   exposes    = battery, battery_voltage, action(release/single/double/
//                hold per input in1..in3)
//   endpoint   = {in1:1, in2:2, in3:3}  (meta.multiEndpoint)
//
// The auto-port was wrong on every count:
//   * fingerprint dropped the `JetHome` manufacturerName gate (model-only).
//   * wired a phantom genOnOff in/out (kFzOnOff + kTzOnOff + `state` Binary
//     expose + 0x0006 binding + to_zigbee) — the device has NO on/off.
//   * dropped the entire discrete-input channel: genMultistateInput
//     presentValue → action was never decoded.
// Fixed: restored the JetHome manufacturer gate, wired kFzBattery +
// kFzJetHomeMultiStateAction, replaced the `state` expose with an `action`
// enum, kept `voltage`, set endpoint_action_suffix so each input surfaces
// as action_in1 / action_in2 / action_in3, and dropped the bogus to_zigbee.
// z2m-source: jethome.ts #WS7.
#include "definitions/jethome/_shared.hpp"

namespace zhc::devices::jethome {
namespace {
const FzConverter* const kFz_WS7[] = {
    &::zhc::generic::kFzBattery,
    &kFzJetHomeMultiStateAction,
};
constexpr const char* kModels_WS7[] = { "WS7" };
constexpr const char* kManufacturers_WS7[] = { "JetHome" };

constexpr ::zhc::EndpointLabel kEndpoints_WS7[] = { {"in1", 1}, {"in2", 2}, {"in3", 3} };

}  // namespace


// --- hand-curated expose/binding block ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg (battery)
};
// --- end block ---

extern const PreparedDefinition kDef_WS7{
    .zigbee_models=kModels_WS7, .zigbee_models_count=sizeof(kModels_WS7)/sizeof(kModels_WS7[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManufacturers_WS7, .manufacturer_names_count=sizeof(kManufacturers_WS7)/sizeof(kManufacturers_WS7[0]),
    .model="WS7", .vendor="Jethome",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS7, .from_zigbee_count=sizeof(kFz_WS7)/sizeof(kFz_WS7[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .endpoint_map       = kEndpoints_WS7,
    .endpoint_map_count = sizeof(kEndpoints_WS7)/sizeof(kEndpoints_WS7[0]),
    .endpoint_action_suffix=true,
};

}  // namespace zhc::devices::jethome
