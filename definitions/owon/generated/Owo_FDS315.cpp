// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Owon FDS315 — auto-generated.
// Fall Detection Sensor
// z2m-source: owon.ts #FDS315.
// TODO(zhc): RUNTIME-BLOCKED. z2m uses owonExtend.addFallDetectionOwonCluster() —
// a manufacturer-specific cluster (mfgcode OWON_TECHNOLOGY_INC) that exposes
// `status` (enum), breathing_rate, location_x/y, fall_detection_settings via
// fzLocal.owonFds315 / tzLocal.owonFds315SetFallSettings. ZHC currently ships
// only the placeholder onOff+battery skeleton below; adding real coverage
// requires wiring an owon manuSpec cluster reader/writer (similar to the lumi
// FCC0 pattern) plus a typed FzConverter for the proprietary attrs.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::owon {
namespace {


constexpr const char* kModels_FDS315[] = { "FDS315", "FDS315-AH" };

}  // namespace


namespace {
const FzConverter* const kFz_min_FDS315[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_FDS315[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_FDS315[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_FDS315{
    .zigbee_models=kModels_FDS315, .zigbee_models_count=sizeof(kModels_FDS315)/sizeof(kModels_FDS315[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="FDS315", .vendor="Owon",
    .meta=nullptr, .exposes=kExp_min_FDS315, .exposes_count=sizeof(kExp_min_FDS315)/sizeof(kExp_min_FDS315[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_FDS315, .from_zigbee_count=sizeof(kFz_min_FDS315)/sizeof(kFz_min_FDS315[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_FDS315,
    .bindings_count           = sizeof(kBind_min_FDS315)/sizeof(kBind_min_FDS315[0]),
};

}  // namespace zhc::devices::owon
