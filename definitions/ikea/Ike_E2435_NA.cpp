// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: IKEA E2435-NA TOFSMYGGA 2-gang outdoor plug (z2m v26.125.0 window).
// Two relay endpoints named "1"/"2" (m.deviceEndpoints + m.onOff) plus identify.
// zigbeeModel "TOFSMYGGA plug outdoor"; Ike_E2435 is the unrelated GRILLPLATS plug.
// z2m-source: ikea.ts #E2435-NA.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::ikea {
namespace {
const FzConverter* const kFz_E2435_NA[] = {
    &::zhc::generic::kFzOnOff,
};
const TzConverter* const kTz_E2435_NA[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_E2435_NA[] = { "TOFSMYGGA plug outdoor" };

constexpr ::zhc::EndpointLabel kEndpoints_E2435_NA[] = { {"1", 1}, {"2", 2} };

constexpr Expose kExposes_E2435_NA[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_E2435_NA[] = {
    {1, 0x0006},
    {2, 0x0006},
};

}  // namespace

extern const PreparedDefinition kDef_E2435_NA{
    .zigbee_models=kModels_E2435_NA, .zigbee_models_count=sizeof(kModels_E2435_NA)/sizeof(kModels_E2435_NA[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="E2435-NA", .vendor="IKEA",
    .meta=nullptr, .exposes=kExposes_E2435_NA, .exposes_count=sizeof(kExposes_E2435_NA)/sizeof(kExposes_E2435_NA[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_E2435_NA, .from_zigbee_count=sizeof(kFz_E2435_NA)/sizeof(kFz_E2435_NA[0]),
    .to_zigbee=kTz_E2435_NA, .to_zigbee_count=sizeof(kTz_E2435_NA)/sizeof(kTz_E2435_NA[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_E2435_NA, .bindings_count=sizeof(kBindings_E2435_NA)/sizeof(kBindings_E2435_NA[0]),
    .endpoint_map       = kEndpoints_E2435_NA,
    .endpoint_map_count = sizeof(kEndpoints_E2435_NA)/sizeof(kEndpoints_E2435_NA[0]),
};

}  // namespace zhc::devices::ikea
