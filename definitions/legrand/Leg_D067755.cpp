// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Legrand 067755 — batteryless scene remote, not an on/off switch.
//
// Wireless 4-scene pocket remote, no local relay. The generated def
// wired kFzOnOff + an on/off TZ (both dead). The device emits genScenes
// recall commands (client->server); z2m decodes them with
// fz.command_recall. Re-wired to the generic kFzCommandRecall decoder so
// scene presses surface as action recall_<scene>.
//
// z2m-source: legrand.ts #067755 —
//   fromZigbee: [fz.identify, fz.battery, fz.command_recall]
//   exposes: e.action(["identify","recall_1_1"]), e.battery()
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::legrand {
namespace {
const FzConverter* const kFz_D067755[] = {
    &::zhc::generic::kFzCommandRecall,
    &::zhc::generic::kFzBattery,
};
constexpr const char* kModels_D067755[] = { " Pocket remote", " Wireless Scenes Command" };

constexpr Expose kAutoExposes[] = {
    {"action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0005},
    {1, 0x0001},
};

}  // namespace

extern const PreparedDefinition kDef_D067755{
    .zigbee_models=kModels_D067755, .zigbee_models_count=sizeof(kModels_D067755)/sizeof(kModels_D067755[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="067755", .vendor="Legrand",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D067755, .from_zigbee_count=sizeof(kFz_D067755)/sizeof(kFz_D067755[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::legrand
