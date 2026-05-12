// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: CustomDevicesDiy Silabs series 2 router — auto-generated.
// Silabs series 2 adapter with router firmware
// z2m-source: custom_devices_diy.ts #Silabs series 2 router.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::custom_devices_diy {
namespace {
// z2m: TS uses fingerprint match on 19 modelIDs (all running Nerivec/silabs-firmware-builder
// router image with appVer=200). toZigbee=[tz.factory_reset], exposes=[e.enum("reset", SET,
// ["reset"])], extend=[m.linkQuality({reporting: true})], meta={disableDefaultResponse:true}.
// TODO(parity): no fingerprint matching implemented (only zigbee_models). Models fall back
// to generic factory reset path - users won't see "reset" enum or linkQuality reporting.
// Vendor in TS is "Silabs" (not "Custom devices (DiY)") - vendor= here is the bucket name.
constexpr const char* kModels_Silabs_series_2_router[] = {
    "ZGA008", "ZB-GW04", "ZB-GW04-1v1", "ZB-GW04-1v2", "SkyConnect", "ZBT-2",
    "SLZB-06M", "SLZB-06MG24", "SLZB-06MG26", "SLZB-06MG26U", "SLZB-07", "SLZB-07MG24",
    "DONGLE-E", "Dongle-LMG21", "Dongle-M", "Dongle-PMG24",
    "MGM240P", "MGM24", "BM24",
};
}  // namespace
extern const PreparedDefinition kDef_Silabs_series_2_router{
    .zigbee_models=kModels_Silabs_series_2_router, .zigbee_models_count=sizeof(kModels_Silabs_series_2_router)/sizeof(kModels_Silabs_series_2_router[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Silabs series 2 router", .vendor="CustomDevicesDiy",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::custom_devices_diy
