// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Lumi JWDL001A — auto-generated.
// Embedded spot led light
// z2m-source: lumi.ts #JWDL001A.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::lumi {
namespace {


constexpr const char* kModels_JWDL001A[] = { "lumi.light.cwjwcn02" };

}  // namespace

namespace {
const FzConverter* const kFz_min_JWDL001A[] = {
    &::zhc::generic::kFzBattery, &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_JWDL001A[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State, "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State, "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_JWDL001A[] = { { 1, 0x0001 }, { 1, 0x0006 } };
}  // namespace
extern const PreparedDefinition kDef_JWDL001A{
    .zigbee_models=kModels_JWDL001A, .zigbee_models_count=sizeof(kModels_JWDL001A)/sizeof(kModels_JWDL001A[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="JWDL001A", .vendor="Lumi",
    .meta=nullptr, .exposes=kExp_min_JWDL001A, .exposes_count=sizeof(kExp_min_JWDL001A)/sizeof(kExp_min_JWDL001A[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_JWDL001A, .from_zigbee_count=sizeof(kFz_min_JWDL001A)/sizeof(kFz_min_JWDL001A[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::lumi
