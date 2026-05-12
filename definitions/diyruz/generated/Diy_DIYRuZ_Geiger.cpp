// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Diyruz DIYRuZ_Geiger — hand-extended 2026-04-28.
// DiY Geiger counter — uses commandOn/commandOff for action + a custom
// diyruz_geiger fz that carries radiation/RPM + manuSpec config attrs
// (led_feedback, buzzer_feedback, alert_threshold, sensors_type,
// sensors_count, sensitivity).
// z2m-source: diyruz.ts #DIYRuZ_Geiger.
// PARTIAL: action wired via kFzCommandOn/Off; diyruz_geiger custom cluster
// is BLOCKED. Numeric/enum exposes declared for UI shape (reports drop).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {
const FzConverter* const kFz_DIYRuZ_Geiger[] = {
    &::zhc::generic::kFzCommandOn,
    &::zhc::generic::kFzCommandOff,
};
const TzConverter* const kTz_DIYRuZ_Geiger[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_DIYRuZ_Geiger[] = { "DIYRuZ_Geiger" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"action",                        ExposeType::Enum,    Access::State,    nullptr, nullptr, nullptr, 0},
    {"radioactive_events_per_minute", ExposeType::Numeric, Access::State,    "rpm",   nullptr, nullptr, 0},
    {"radiation_dose_per_hour",       ExposeType::Numeric, Access::State,    "uR/h",  nullptr, nullptr, 0},
    {"led_feedback",                  ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"buzzer_feedback",               ExposeType::Binary,  Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"alert_threshold",               ExposeType::Numeric, Access::StateSet, "uR/h",  nullptr, nullptr, 0},
    {"sensors_type",                  ExposeType::Enum,    Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensors_count",                 ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"sensitivity",                   ExposeType::Numeric, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},  // genOnOff (commandOn/Off)
    {1, 0x0400},  // msIlluminanceMeasurement (manuSpec geiger attrs ride here per z2m configure)
};

extern const PreparedDefinition kDef_DIYRuZ_Geiger{
    .zigbee_models=kModels_DIYRuZ_Geiger, .zigbee_models_count=sizeof(kModels_DIYRuZ_Geiger)/sizeof(kModels_DIYRuZ_Geiger[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_Geiger", .vendor="Diyruz",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_DIYRuZ_Geiger, .from_zigbee_count=sizeof(kFz_DIYRuZ_Geiger)/sizeof(kFz_DIYRuZ_Geiger[0]),
    .to_zigbee=kTz_DIYRuZ_Geiger, .to_zigbee_count=sizeof(kTz_DIYRuZ_Geiger)/sizeof(kTz_DIYRuZ_Geiger[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::diyruz
