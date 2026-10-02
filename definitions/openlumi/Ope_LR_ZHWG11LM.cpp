// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: OpenLumi LR-ZHWG11LM, Lumi router firmware for the Xiaomi/Aqara ZHWG11LM.
// z2m v26.115.1 split it out of GWRJN5169: m.deviceTemperature with
// reporting 5 min .. 1 h, change 1.
// z2m-source: openlumi.ts #LR-ZHWG11LM.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::openlumi {
namespace {
const FzConverter* const kFz[] = {
    &::zhc::generic::kFzDeviceTemperature,
};
constexpr Expose kExp[] = {
    {"device_temperature", ExposeType::Numeric, Access::State, "°C", nullptr, nullptr, 0},
};
constexpr const char* kM[] = { "openlumi.gw_router.zhwg11lm" };
constexpr BindingSpec kBind[] = {
    {1, 0x0002},
};
constexpr ReportingSpec kRep[] = {
    {1, 0x0002, 0x0000, 0x29, 300, 3600, 1, 0},
};
}  // namespace

extern const PreparedDefinition kDef_LR_ZHWG11LM{
    .zigbee_models=kM, .zigbee_models_count=sizeof(kM)/sizeof(kM[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="LR-ZHWG11LM", .vendor="OpenLumi",
    .meta=nullptr, .exposes=kExp, .exposes_count=sizeof(kExp)/sizeof(kExp[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz, .from_zigbee_count=sizeof(kFz)/sizeof(kFz[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBind, .bindings_count=sizeof(kBind)/sizeof(kBind[0]),
    .reports=kRep, .reports_count=sizeof(kRep)/sizeof(kRep[0]),
};

}  // namespace zhc::devices::openlumi
