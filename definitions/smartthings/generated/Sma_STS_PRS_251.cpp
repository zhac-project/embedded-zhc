// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Smartthings STS-PRS-251 — hand-rewritten 2026-04-28.
// Arrival sensor (Bluetooth-style key-fob tracker).
// z2m-source: smartthings.ts #STS-PRS-251.
//
// z2m bundle:
//   fz: STS_PRS_251_presence (genBinaryInput presentValue), battery, identify
//   tz: STS_PRS_251_beep (identify trigger via cluster 0x0003)
//   exposes: battery, presence, action: ["identify"]
//
// Previous port had on/off + on/off TZ — that was wrong; the device has no
// genOnOff cluster and z2m never wires fz.on_off / tz.on_off.
//
// Runtime gap: presence + identify-action surfaces are not yet emitted.
//   - `presence` requires a `genBinaryInput` (cluster 0x000F) presentValue
//     attr 0x0055 -> Bool decoder, not yet in `_generic/_shared.cpp`.
//   - `action: identify` would require an identify-cmd-server FzConverter on
//     cluster 0x0003 (none exists today).
//   - `tz_sts_prs_251_beep` would write IdentifyTime (cluster 0x0003,
//     attr 0x0000); generic `kTzIdentify` is also absent.
// Until those land, only `battery`/`voltage` will be emitted.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::smartthings {
namespace {
const FzConverter* const kFz_STS_PRS_251[] = {
    &::zhc::generic::kFzBattery,
};

constexpr const char* kModels_STS_PRS_251[] = { "tagv4" };

constexpr Expose kAutoExposes[] = {
    {"battery", ExposeType::Numeric, Access::State, "%",  nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "mV", nullptr, nullptr, 0},
    // presence (genBinaryInput 0x000F) + action:["identify"] — runtime gap.
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},
    {1, 0x000F},
};
}  // namespace

extern const PreparedDefinition kDef_STS_PRS_251{
    .zigbee_models=kModels_STS_PRS_251, .zigbee_models_count=sizeof(kModels_STS_PRS_251)/sizeof(kModels_STS_PRS_251[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="STS-PRS-251", .vendor="Smartthings",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_STS_PRS_251, .from_zigbee_count=sizeof(kFz_STS_PRS_251)/sizeof(kFz_STS_PRS_251[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::smartthings
