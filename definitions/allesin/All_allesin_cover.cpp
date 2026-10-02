// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Allesin roller shade (z2m v26.115.1 window, new vendor).
// Fingerprint TLSR82xx + manufacturer Aubor. z2m m.windowCovering({controls: [lift]})
// + m.battery({dontDividePercentage: true}) + m.forcePowerSource(Battery).
// z2m-source: allesin.ts #allesin_cover.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::allesin {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzCoverPosition,
    &::zhc::generic::kFzBatteryNoDivide,
};
const TzConverter* const kTz[] = {
    &::zhc::generic::kTzCoverPosition,
};
constexpr const char* kOpts0[] = { "OPEN", "CLOSE", "STOP" };
constexpr Expose kExp[] = {
    {"state", ExposeType::Enum, Access::StateSet, nullptr, nullptr, kOpts0, 3},
    {"position", ExposeType::Numeric, Access::StateSet, "%", nullptr, nullptr, 0},
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "TLSR82xx" };
constexpr const char* kN[] = { "Aubor" };
constexpr BindingSpec kBind[] = {
    {1, 0x0001},
    {1, 0x0102},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0102, 0x0008, 0x20, 1, 65000, 1, 0},
    {1, 0x0001, 0x0021, 0x20, 3600, 65000, 10, 0},
};
}  // namespace

extern const PreparedDefinition kDef_allesin_cover{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kN, .manufacturer_names_count=sizeof(kN)/sizeof(kN[0]),
    .model="allesin_cover", .vendor="Allesin",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=kTz, .to_zigbee_count=sizeof(kTz)/sizeof(kTz[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
    .power_source_override=0x03,
};

}  // namespace zhc::devices::allesin
