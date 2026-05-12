// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Climax RS-23ZBS — typed-IAS audit 2026-04-29a.
// Temperature & humidity sensor — minimal-port stub had wrong wiring
// (kFzBattery+kFzOnOff/state). z2m only declares fz.temperature + fz.humidity.
// z2m-source: climax.ts #RS-23ZBS.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::climax {
namespace {

constexpr const char* kModels_RS_23ZBS[] = { "RS_00.00.02.06TC" };

const FzConverter* const kFz_RS_23ZBS[] = {
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
constexpr Expose kExp_RS_23ZBS[] = {
    { "temperature", ExposeType::Numeric, ::zhc::Access::State, "C",  nullptr, nullptr, 0 },
    { "humidity",    ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_RS_23ZBS[] = {
    { 1, 0x0402 }, { 1, 0x0405 },
};

}  // namespace

extern const PreparedDefinition kDef_RS_23ZBS{
    .zigbee_models=kModels_RS_23ZBS, .zigbee_models_count=sizeof(kModels_RS_23ZBS)/sizeof(kModels_RS_23ZBS[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="RS-23ZBS", .vendor="Climax",
    .meta=nullptr, .exposes=kExp_RS_23ZBS, .exposes_count=sizeof(kExp_RS_23ZBS)/sizeof(kExp_RS_23ZBS[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_RS_23ZBS, .from_zigbee_count=sizeof(kFz_RS_23ZBS)/sizeof(kFz_RS_23ZBS[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_RS_23ZBS,
    .bindings_count           = sizeof(kBind_RS_23ZBS)/sizeof(kBind_RS_23ZBS[0]),
};

}  // namespace zhc::devices::climax
