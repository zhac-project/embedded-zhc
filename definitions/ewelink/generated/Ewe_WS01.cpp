// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ewelink WS01 — hand-corrected.
// Rainfall sensor. z2m fz: [fzLocal.WS01_rain] on ssIasZone
// commandStatusChangeNotification (zonestatus bit0 -> rain bool).
// z2m tz: []; z2m exposes: [e.rain()]. No on/off, no battery in z2m.
// Generic generator wrongly added kFzBattery+kFzOnOff and a genOnOff
// binding — removed here. Bind ssIasZone (0x0500) and reuse the
// generic IAS zone-status converter; bit0 alarm currently surfaces as
// the existing "alarm" key. A dedicated `rain` mapping needs a
// per-device fz override — left for a future sweep.
// z2m-source: ewelink.ts #WS01 + fzLocal.WS01_rain.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
const FzConverter* const kFz_WS01[] = {
    &::zhc::generic::kFzIasZoneStatusChange,
};
constexpr const char* kModels_WS01[] = { "WS01" };

constexpr Expose kExp_WS01[] = {
    { "rain", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_WS01[] = {
    { 1, 0x0500 },
};
}  // namespace

extern const PreparedDefinition kDef_WS01{
    .zigbee_models=kModels_WS01, .zigbee_models_count=sizeof(kModels_WS01)/sizeof(kModels_WS01[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WS01", .vendor="Ewelink",
    .meta=nullptr, .exposes=kExp_WS01, .exposes_count=sizeof(kExp_WS01)/sizeof(kExp_WS01[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_WS01, .from_zigbee_count=sizeof(kFz_WS01)/sizeof(kFz_WS01[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_WS01, .bindings_count=sizeof(kBind_WS01)/sizeof(kBind_WS01[0]),
};

}  // namespace zhc::devices::ewelink
