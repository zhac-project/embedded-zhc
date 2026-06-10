// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Qmotion QZR-ZIG2400 — 5 channel remote, NOT a controllable cover.
//
// The Tier-1 auto-port mis-modelled this remote as an on/off cover: it
// wired kFzOnOff (z2m never decodes genOnOff here), a writable `state`
// Binary expose, and phantom kTzOnOff + kTzCoverPosition write paths.
// z2m's remote has an EMPTY toZigbee — it is an input device that reports
// the shade position it drives, plus an `identify` action.
//
// Fix: decode position via the cover-position-tilt decoder (read-only),
// decode the genIdentify `action`, drop all toZigbee, drop the phantom
// genOnOff binding, and expose `action` + read-only `position`.
//
// z2m-source: qmotion.ts #QZR-ZIG2400 —
//   fromZigbee: [fz.identify, fz.cover_position_tilt]
//   exposes: [e.action(["identify"]), e.numeric("position", ea.STATE)]
//   toZigbee: []
#include "_shared.hpp"

namespace zhc::devices::qmotion {
namespace {
const FzConverter* const kFz_QZR_ZIG2400[] = {
    &::zhc::qmotion::kFzIdentifyAction,   // fz.identify → action = "identify"
    &::zhc::generic::kFzCoverPosition,    // fz.cover_position_tilt (lift) → position
};
constexpr const char* kModels_QZR_ZIG2400[] = { "Rollershade QdR" };

// z2m: e.action(["identify"]) + e.numeric("position", ea.STATE).
// position is STATE (read-only) — the remote reports, it is not commanded.
constexpr Expose kAutoExposes[] = {
    {"action",   ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0},
    {"position", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0},
};

// Only the closuresWindowCovering report binding remains; the genOnOff
// (0x0006) binding the auto-port carried was phantom.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0102},
};

}  // namespace

extern const PreparedDefinition kDef_QZR_ZIG2400{
    .zigbee_models=kModels_QZR_ZIG2400, .zigbee_models_count=sizeof(kModels_QZR_ZIG2400)/sizeof(kModels_QZR_ZIG2400[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="QZR-ZIG2400", .vendor="Qmotion",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QZR_ZIG2400, .from_zigbee_count=sizeof(kFz_QZR_ZIG2400)/sizeof(kFz_QZR_ZIG2400[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::qmotion
