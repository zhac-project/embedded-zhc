// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Qoto QT-05M — auto-generated.
// Solar powered garden watering timer
// z2m-source: qoto.ts #QT-05M.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::qoto {
namespace {


constexpr const char* kModels_QT_05M[] = { "TS0601" };
constexpr const char* kManus_QT_05M[] = { "_TZE200_arge1ptm", "_TZE200_anv5ujhv", "_TZE200_xlppj4f5" };
}  // namespace


namespace {
const FzConverter* const kFz_min_QT_05M[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_QT_05M[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_QT_05M[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_QT_05M{
    .zigbee_models=kModels_QT_05M, .zigbee_models_count=sizeof(kModels_QT_05M)/sizeof(kModels_QT_05M[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_QT_05M, .manufacturer_names_count=sizeof(kManus_QT_05M)/sizeof(kManus_QT_05M[0]),
    .model="QT-05M", .vendor="Qoto",
    .meta=nullptr, .exposes=kExp_min_QT_05M, .exposes_count=sizeof(kExp_min_QT_05M)/sizeof(kExp_min_QT_05M[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_QT_05M, .from_zigbee_count=sizeof(kFz_min_QT_05M)/sizeof(kFz_min_QT_05M[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_QT_05M,
    .bindings_count           = sizeof(kBind_min_QT_05M)/sizeof(kBind_min_QT_05M[0]),
};

}  // namespace zhc::devices::qoto
