// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Databyte DTB190502A1 — graduated from generated/.
// CC2530 based IO Board.
//
// Fix: z2m wires the custom `fz.DTB190502A1` decoder and exposes
// led_state (binary ON/OFF, read-only) + key_state (enum). The
// auto-port fabricated a generic controllable `state` + a phantom
// battery/voltage bundle (this mains IO board has no battery), and
// dropped the real decoder entirely. Wire the vendor decoder (genOnOff
// manufacturer attrs 0xA191 cpu_temperature / 0xA192 key_state /
// 0xA193 led_state) and replace the exposes with z2m's schema.
//
// z2m-source: databyte.ts #DTB190502A1 (fz.DTB190502A1).
#include "definitions/_generic/_shared.hpp"
#include "definitions/databyte/_shared.hpp"

namespace zhc::devices::databyte {
namespace {


constexpr const char* kModels_DTB190502A1[] = { "DTB190502A1" };

}  // namespace


namespace {
const FzConverter* const kFz_min_DTB190502A1[] = {
    &::zhc::databyte::kFzDtb190502a1,
};
constexpr const char* kKeyStateValues_DTB190502A1[] = {
    "KEY_SYS", "KEY_UP", "KEY_DOWN", "KEY_NONE",
};
constexpr Expose kExp_min_DTB190502A1[] = {
    { "led_state", ExposeType::Binary, ::zhc::Access::State, nullptr, nullptr, nullptr, 0 },
    { "key_state", ExposeType::Enum,   ::zhc::Access::State, nullptr, nullptr,
      kKeyStateValues_DTB190502A1,
      sizeof(kKeyStateValues_DTB190502A1)/sizeof(kKeyStateValues_DTB190502A1[0]) },
};
constexpr BindingSpec kBind_min_DTB190502A1[] = {
    { 1, 0x0006 },
};
}
extern const PreparedDefinition kDef_DTB190502A1{
    .zigbee_models=kModels_DTB190502A1, .zigbee_models_count=sizeof(kModels_DTB190502A1)/sizeof(kModels_DTB190502A1[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="DTB190502A1", .vendor="Databyte",
    .meta=nullptr, .exposes=kExp_min_DTB190502A1, .exposes_count=sizeof(kExp_min_DTB190502A1)/sizeof(kExp_min_DTB190502A1[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_min_DTB190502A1, .from_zigbee_count=sizeof(kFz_min_DTB190502A1)/sizeof(kFz_min_DTB190502A1[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings                 = kBind_min_DTB190502A1,
    .bindings_count           = sizeof(kBind_min_DTB190502A1)/sizeof(kBind_min_DTB190502A1[0]),
};

}  // namespace zhc::devices::databyte
