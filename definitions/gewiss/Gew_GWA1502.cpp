// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Gewiss GWA1502 — graduated.
// Contact interface - 2 channels - 230V
// z2m-source: gewiss.ts #GWA1502.
// z2m extend = [deviceEndpoints{1,2}, binary(input) x2 via
// genBinaryInput.presentValue]. It is a MAINS (230V) contact interface
// with NO battery and NO on/off. The auto-port mis-wired a phantom
// kFzBattery + phantom kFzOnOff (dead state/battery/voltage keys).
// Fix: decode genBinaryInput presentValue → `input` on both endpoints
// (dispatch suffixes to input_1 / input_2 via endpoint_map). No battery,
// no on/off, no genOnOff binding.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::gewiss {
namespace {
const FzConverter* const kFz_GWA1502[] = {
    &::zhc::generic::kFzBinaryInput,
};

constexpr const char* kModels_GWA1502[] = { "GWA1502_BinaryInput230V" };

constexpr ::zhc::EndpointLabel kEndpoints_GWA1502[] = { {"1", 1}, {"2", 2} };

constexpr Expose kExp_GWA1502[] = {
    { "input", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
};

constexpr BindingSpec kBind_GWA1502[] = {
    { 1, 0x000F }, { 2, 0x000F },
};
}  // namespace

extern const PreparedDefinition kDef_GWA1502{
    .zigbee_models=kModels_GWA1502, .zigbee_models_count=sizeof(kModels_GWA1502)/sizeof(kModels_GWA1502[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="GWA1502", .vendor="Gewiss",
    .meta=nullptr, .exposes=kExp_GWA1502, .exposes_count=sizeof(kExp_GWA1502)/sizeof(kExp_GWA1502[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_GWA1502, .from_zigbee_count=sizeof(kFz_GWA1502)/sizeof(kFz_GWA1502[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_GWA1502,
    .bindings_count           = sizeof(kBind_GWA1502)/sizeof(kBind_GWA1502[0]),
    .endpoint_map             = kEndpoints_GWA1502,
    .endpoint_map_count       = sizeof(kEndpoints_GWA1502)/sizeof(kEndpoints_GWA1502[0]),
};

}  // namespace zhc::devices::gewiss
