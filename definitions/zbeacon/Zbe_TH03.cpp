// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Zbeacon TH03 temperature & humidity sensor (zigbeeModel TH03), new in
// z2m v26.126.0..v26.133.0. m.battery + m.temperature + m.humidity (10 s .. 1 h
// on 1 C / 1 %). Generic converters only. Exact-match "TH03" does not touch the
// zigbeetlc TS0201-z def, which lists "TH03Z-z" / "TH03Z-bz".
// Not ported: identify, battery-percentage reporting.
// z2m-source: zbeacon.ts #TH03
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::zbeacon {
namespace {
const FzConverter* const kFz_TH03[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzTemperature,
    &::zhc::generic::kFzHumidity,
};
constexpr const char* kModels_TH03[] = { "TH03" };
constexpr Expose kExposes_TH03[] = {
    {"battery", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity", ExposeType::Numeric, Access::State, "%", nullptr, nullptr, 0},
};
constexpr BindingSpec kBindings_TH03[] = {
    {1, 0x0001},
    {1, 0x0402},
    {1, 0x0405},
};
constexpr ReportingSpec kReports_TH03[] = {
    {1, 0x0402, 0x0000, 0x29, 10, 3600, 100, 0},
    {1, 0x0405, 0x0000, 0x21, 10, 3600, 100, 0},
};
}  // namespace

extern const PreparedDefinition kDef_TH03{
    .zigbee_models=kModels_TH03, .zigbee_models_count=sizeof(kModels_TH03)/sizeof(kModels_TH03[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="TH03", .vendor="Zbeacon",
    .meta=nullptr, .exposes=kExposes_TH03, .exposes_count=sizeof(kExposes_TH03)/sizeof(kExposes_TH03[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_TH03, .from_zigbee_count=sizeof(kFz_TH03)/sizeof(kFz_TH03[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_TH03, .bindings_count=sizeof(kBindings_TH03)/sizeof(kBindings_TH03[0]),
    .reports=kReports_TH03, .reports_count=sizeof(kReports_TH03)/sizeof(kReports_TH03[0]),
};

}  // namespace zhc::devices::zbeacon
