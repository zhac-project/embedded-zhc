// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LeTV LeTV.8KEY — hand-curated (was phantom on/off + dead device).
//
// LeTV (Leeco) qlwz.letv8key.10 — an 8-key scene remote. z2m role:
//   fromZigbee = [fz.qlwz_letv8key_switch]
//   toZigbee   = []                       (uncontrollable)
//   exposes    = action(<gesture>_<button>)  — 8 buttons × 4 gestures
//
// The auto-port was wrong on every count: it mis-ported the remote as a
// controllable on/off SWITCH —
//   * wired a phantom genOnOff in/out (kFzOnOff + kTzOnOff), a settable
//     `state` Binary expose, a 0x0006 genOnOff binding, and to_zigbee.
//   * dropped the entire action channel: z2m decodes genMultistateInput
//     presentValue (attr 0x0055) keyed by the source endpoint into an
//     `action` string and never touches genOnOff at all.
//
// Fixed: replaced the phantom on/off with kFzLetv8KeyAction + an `action`
// enum, dropped to_zigbee and the bogus 0x0006 binding (z2m has no
// configure/bind for this device). The button is encoded inside the action
// string (single_up / hold_center / …), so there is NO endpoint_map — the
// converter resolves src_endpoint itself.
//
// z2m-source: letv.ts (#LeTV.8KEY) + fromZigbee.ts (qlwz_letv8key_switch).
#include "definitions/letv/_shared.hpp"

namespace zhc::devices::letv {
namespace {
const FzConverter* const kFz_LeTV_8KEY[] = {
    &kFzLetv8KeyAction,
};
constexpr const char* kModels_LeTV_8KEY[] = { "qlwz.letv8key.10" };

}  // namespace


// --- hand-curated expose block (read-only scene-remote action) ---
constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};
// --- end block ---

extern const PreparedDefinition kDef_LeTV_8KEY{
    .zigbee_models=kModels_LeTV_8KEY, .zigbee_models_count=sizeof(kModels_LeTV_8KEY)/sizeof(kModels_LeTV_8KEY[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LeTV.8KEY", .vendor="Letv",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_LeTV_8KEY, .from_zigbee_count=sizeof(kFz_LeTV_8KEY)/sizeof(kFz_LeTV_8KEY[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::letv
