// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Databyte Touch4 — graduated from generated/.
// Wall touchsensor with 4 keys.
//
// Fix: z2m wires `fzLocal.DTB2011014` + `fz.battery` and exposes
// key_1..key_4 (binary ON/OFF). The auto-port dropped the custom
// decoder entirely (battery-only), so the four touch keys never
// surfaced. Wire the vendor decoder (genOnOff manufacturer attrs
// 0xA191..0xA194) and restore the key_1..key_4 exposes.
//
// z2m-source: databyte.ts #Touch4 (fzLocal.DTB2011014).
#include "definitions/_generic/_shared.hpp"
#include "definitions/databyte/_shared.hpp"

namespace zhc::devices::databyte {
namespace {
const FzConverter* const kFz_Touch4[] = {
    &::zhc::databyte::kFzTouch4Keys,
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_Touch4[] = { "DTB-ED2011-014" };

}  // namespace


// --- key_1..key_4 + battery ---
constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    {"key_1", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"key_2", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"key_3", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
    {"key_4", ExposeType::Binary, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
};
// --- end auto-generated block ---

extern const PreparedDefinition kDef_Touch4{
    .zigbee_models=kModels_Touch4, .zigbee_models_count=sizeof(kModels_Touch4)/sizeof(kModels_Touch4[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Touch4", .vendor="Databyte",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Touch4, .from_zigbee_count=sizeof(kFz_Touch4)/sizeof(kFz_Touch4[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::databyte
