// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Diyruz DIYRuZ_KEYPAD20 — auto-generated.
// DiY 20 button keypad
// z2m-source: diyruz.ts #DIYRuZ_KEYPAD20.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::diyruz {
namespace {


constexpr const char* kModels_DIYRuZ_KEYPAD20[] = { "DIYRuZ_KEYPAD20" };

constexpr ::zhc::EndpointLabel kEndpoints_DIYRuZ_KEYPAD20[] = { {"btn_1", 1}, {"btn_2", 2}, {"btn_3", 3}, {"btn_4", 4}, {"btn_5", 5}, {"btn_6", 6}, {"btn_7", 7}, {"btn_8", 8}, {"btn_9", 9}, {"btn_10", 10}, {"btn_11", 11}, {"btn_12", 12}, {"btn_13", 13}, {"btn_14", 14}, {"btn_15", 15}, {"btn_16", 16}, {"btn_17", 17}, {"btn_18", 18}, {"btn_19", 19}, {"btn_20", 20} };

}  // namespace


namespace {
const FzConverter* const kFz_min_DIYRuZ_KEYPAD20[] = {
    &::zhc::generic::kFzBattery,
    &::zhc::generic::kFzOnOff,
};
constexpr Expose kExp_min_DIYRuZ_KEYPAD20[] = {
    { "state",   ExposeType::Binary,  ::zhc::Access::StateSet, nullptr, nullptr, nullptr, 0 },
    { "battery", ExposeType::Numeric, ::zhc::Access::State,    "%",  nullptr, nullptr, 0 },
    { "voltage", ExposeType::Numeric, ::zhc::Access::State,    "mV", nullptr, nullptr, 0 },
};
constexpr BindingSpec kBind_min_DIYRuZ_KEYPAD20[] = {
    { 1, 0x0001 }, { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_DIYRuZ_KEYPAD20{
    .zigbee_models=kModels_DIYRuZ_KEYPAD20, .zigbee_models_count=sizeof(kModels_DIYRuZ_KEYPAD20)/sizeof(kModels_DIYRuZ_KEYPAD20[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DIYRuZ_KEYPAD20", .vendor="Diyruz",
    .meta=nullptr, .exposes=kExp_min_DIYRuZ_KEYPAD20, .exposes_count=sizeof(kExp_min_DIYRuZ_KEYPAD20)/sizeof(kExp_min_DIYRuZ_KEYPAD20[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_DIYRuZ_KEYPAD20, .from_zigbee_count=sizeof(kFz_min_DIYRuZ_KEYPAD20)/sizeof(kFz_min_DIYRuZ_KEYPAD20[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_DIYRuZ_KEYPAD20,
    .bindings_count           = sizeof(kBind_min_DIYRuZ_KEYPAD20)/sizeof(kBind_min_DIYRuZ_KEYPAD20[0]),
    .endpoint_map       = kEndpoints_DIYRuZ_KEYPAD20,
    .endpoint_map_count = sizeof(kEndpoints_DIYRuZ_KEYPAD20)/sizeof(kEndpoints_DIYRuZ_KEYPAD20[0]),
};

}  // namespace zhc::devices::diyruz
