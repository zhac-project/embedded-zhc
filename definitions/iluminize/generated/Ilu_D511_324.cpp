// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Iluminize 511.324 — hand-rewritten battery-powered handheld remote.
// z2m: 4-endpoint multi-button remote. fromZigbee covers battery + a long
// list of command_* converters; toZigbee=[].
// Per-endpoint actions are emitted by the generic command converters; we
// surface a single "action" enum and a battery percent/voltage. The
// `endpoint`/`meta.multiEndpoint` plumbing is a runtime concern handled
// by the matcher, not a static field.
// z2m-source: iluminize.ts #511.324.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::iluminize {
namespace {

const FzConverter* const kFz_D511_324[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
    &::zhc::generic::kFzCommandToggle,
    &::zhc::generic::kFzCommandMoveToColorTemp,
    &::zhc::generic::kFzCommandMoveToHueAndSaturation,
    &::zhc::generic::kFzCommandStep,
    &::zhc::generic::kFzCommandMove,
    &::zhc::generic::kFzCommandStop,
    &::zhc::generic::kFzCommandRecall,
};

constexpr Expose kExposes_D511_324[] = {
    { "action",  ExposeType::Enum,    Access::State, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, Access::State, "%",     nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, Access::State, "mV",    nullptr, nullptr, 0 },
};

constexpr BindingSpec kBindings_D511_324[] = {
    { 1, 0x0001 }, { 1, 0x0006 }, { 1, 0x0008 }, { 1, 0x0300 },
};

constexpr const char* kModels_D511_324[] = { "511.324" };

}  // namespace

extern const PreparedDefinition kDef_D511_324{
    .zigbee_models=kModels_D511_324,
    .zigbee_models_count=sizeof(kModels_D511_324)/sizeof(kModels_D511_324[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="511.324", .vendor="Iluminize",
    .meta=nullptr,
    .exposes=kExposes_D511_324,
    .exposes_count=sizeof(kExposes_D511_324)/sizeof(kExposes_D511_324[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_D511_324,
    .from_zigbee_count=sizeof(kFz_D511_324)/sizeof(kFz_D511_324[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_D511_324,
    .bindings_count=sizeof(kBindings_D511_324)/sizeof(kBindings_D511_324[0]),
};

}  // namespace zhc::devices::iluminize
