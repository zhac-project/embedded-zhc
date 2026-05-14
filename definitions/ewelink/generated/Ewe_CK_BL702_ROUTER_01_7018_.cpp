// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Ewelink CK-BL702-ROUTER-01(7018) — hand-corrected.
// USB signal repeater (router only). z2m exposes: nothing.
// z2m fz: [linkquality_from_basic]; z2m tz: []; z2m exposes: [].
// Generic generator wrongly added kFzOnOff/kTzOnOff + 0x0006 binding —
// removed here. Whitelabel HOBEIAN ZG-807Z carries two manuf names
// (_TZ3000_piuensvr + _TZ3000_hgm6k8ku).
// z2m-source: ewelink.ts #CK-BL702-ROUTER-01(7018).
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ewelink {
namespace {
constexpr const char* kModels_CK_BL702_ROUTER_01_7018_[] = { "CK-BL702-ROUTER-01(7018)", "TS0207" };
constexpr const char* kManus_CK_BL702_ROUTER_01_7018_[] = {
    "_TZ3000_hgm6k8ku", "_TZ3000_piuensvr",
};
}  // namespace


constexpr WhiteLabel kWhiteLabels_CK_BL702_ROUTER_01_7018[] = {
    {"HOBEIAN","ZG-807Z"},
};
extern const PreparedDefinition kDef_CK_BL702_ROUTER_01_7018_{
    .zigbee_models=kModels_CK_BL702_ROUTER_01_7018_, .zigbee_models_count=sizeof(kModels_CK_BL702_ROUTER_01_7018_)/sizeof(kModels_CK_BL702_ROUTER_01_7018_[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=kManus_CK_BL702_ROUTER_01_7018_, .manufacturer_names_count=sizeof(kManus_CK_BL702_ROUTER_01_7018_)/sizeof(kManus_CK_BL702_ROUTER_01_7018_[0]),
    .model="CK-BL702-ROUTER-01(7018)", .vendor="Ewelink",
    .meta=nullptr, .exposes=nullptr, .exposes_count=0,
    .white_labels=kWhiteLabels_CK_BL702_ROUTER_01_7018, .white_labels_count=sizeof(kWhiteLabels_CK_BL702_ROUTER_01_7018)/sizeof(kWhiteLabels_CK_BL702_ROUTER_01_7018[0]),
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::ewelink
