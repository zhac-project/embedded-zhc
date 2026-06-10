// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Aeotec WG001 — graduated (was phantom on/off).
// "Range extender Zi" — a mains repeater/router, NOT a sensor or switch.
// z2m: fromZigbee=[fz.linkquality_from_basic], toZigbee=[], exposes=[].
// The auto-port mis-classified it as a controllable on/off: it wired
// kFzOnOff + kTzOnOff, a phantom `state` expose and a genOnOff (0x0006)
// bind. A router has no controllable cluster, so that whole bundle is a
// dead state. Stripped to an exposeless def (no fz/tz, no exposes, no
// bind), matching z2m's empty role — linkquality is reported globally,
// not per-def, so fz.linkquality_from_basic needs no converter here.
// z2m-source: aeotec.ts #WG001.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::aeotec {
namespace {
constexpr const char* kModels_WG001[] = { "WG001-Z01" };
}  // namespace

extern const PreparedDefinition kDef_WG001{
    .zigbee_models=kModels_WG001, .zigbee_models_count=sizeof(kModels_WG001)/sizeof(kModels_WG001[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="WG001", .vendor="Aeotec",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::aeotec
