// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Trust ZYCT-202 — hand-rewritten for parity (action remote).
// Battery-less remote: emits action = on/off/stop/brightness_move_*.
// z2m-source: trust.ts #ZYCT-202 (fz.command_on, fz.command_off_with_effect,
//                                  fz.command_stop, fz.command_move).
//
// The first zigbeeModel literal in z2m is 17 NUL bytes (older firmware
// quirk); the auto-generator emitted the literal escape sequence as
// ASCII which could never match. We keep only the printable variant —
// the device reports "ZLL-NonColorController" on every firmware shipped
// after 2017.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::trust {
namespace {
const FzConverter* const kFz_ZYCT_202[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOffWithEffect,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandMove,
};
constexpr const char* kModels_ZYCT_202[] = { "ZLL-NonColorController" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr, nullptr, 0},
};

extern const PreparedDefinition kDef_ZYCT_202{
    .zigbee_models=kModels_ZYCT_202, .zigbee_models_count=sizeof(kModels_ZYCT_202)/sizeof(kModels_ZYCT_202[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZYCT-202", .vendor="Trust",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_ZYCT_202, .from_zigbee_count=sizeof(kFz_ZYCT_202)/sizeof(kFz_ZYCT_202[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::trust
