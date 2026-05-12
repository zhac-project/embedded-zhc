// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Qa QAFZ200 — hand-rewritten (was wrong-bundle: emitted kFzBattery for a mains CCT light).
// CCT light controller (TS0502B / _TZ3218_op6ztaju) — z2m uses tuya.modernExtend.tuyaLight()
// which maps to genOnOff + genLevelCtrl + lightingColorCtrl (color temperature).
// z2m-source: qa.ts #QAFZ200.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::qa {
namespace {
const FzConverter* const kFz_QAFZ200[] = {
    &::zhc::generic::kFzOnOff,
    &::zhc::generic::kFzBrightness,
    &::zhc::generic::kFzColorTemperature,
};
const TzConverter* const kTz_QAFZ200[] = {
    &::zhc::generic::kTzOnOff,
    &::zhc::generic::kTzBrightness,
    &::zhc::generic::kTzColorTemp,
};
constexpr const char* kModels_QAFZ200[] = { "TS0502B" };
constexpr const char* kManus_QAFZ200[] = { "_TZ3218_op6ztaju" };

constexpr Expose kExp_QAFZ200[] = {
    { "state",            ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "brightness",       ExposeType::Numeric, ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "color_temp",       ExposeType::Numeric, ::zhc::Access::StateSet, "mired", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_QAFZ200[] = {
    { 1, 0x0006 }, // genOnOff
    { 1, 0x0008 }, // genLevelCtrl
    { 1, 0x0300 }, // lightingColorCtrl
};
}  // namespace

extern const PreparedDefinition kDef_QAFZ200{
    .zigbee_models=kModels_QAFZ200, .zigbee_models_count=sizeof(kModels_QAFZ200)/sizeof(kModels_QAFZ200[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QAFZ200, .manufacturer_names_count=sizeof(kManus_QAFZ200)/sizeof(kManus_QAFZ200[0]),
    .model="QAFZ200", .vendor="Qa",
    .meta=nullptr, .exposes=kExp_QAFZ200, .exposes_count=sizeof(kExp_QAFZ200)/sizeof(kExp_QAFZ200[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_QAFZ200, .from_zigbee_count=sizeof(kFz_QAFZ200)/sizeof(kFz_QAFZ200[0]),
    .to_zigbee=kTz_QAFZ200, .to_zigbee_count=sizeof(kTz_QAFZ200)/sizeof(kTz_QAFZ200[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind_QAFZ200,
    .bindings_count=sizeof(kBind_QAFZ200)/sizeof(kBind_QAFZ200[0]),
};

}  // namespace zhc::devices::qa
