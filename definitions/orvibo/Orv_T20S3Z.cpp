// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Orvibo T20S3Z MixSwitch 3 gangs scene remote (z2m v26.76.0 parity).
// z2m-source: orvibo.ts #T20S3Z (fzLocal.orvibo_raw_2).
//
// New device (z2m v26.67.0). Sibling of T40S6Z: a stateless scene remote,
// NOT a genOnOff relay. Each button press emits a vendor-private raw frame
// on the Orvibo cluster (0x0017) decoded by z2m's `orvibo_raw_2` into
// `action` = "button_<n>_<click|hold|release>". z2m exposes button_1..3
// (3 gangs). The same shared kFzOrviboRaw2Action converter used by T40S6Z
// handles the identity button map (button byte = index), self-guarding on
// unknown bytes, so it covers the 3-button subset unchanged. Single
// `action` String expose; no relay/state. See orvibo/_shared.{hpp,cpp}.
#include "definitions/_generic/_shared.hpp"
#include "definitions/orvibo/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {
const FzConverter* const kFz_T20S3Z[] = {
    &::zhc::orvibo::kFzOrviboRaw2Action,
};
constexpr const char* kModels_T20S3Z[] = { "cef8701bb8664a67a83033c071ef05f2" };

}  // namespace

constexpr Expose kAutoExposes[] = {
    // z2m exposes e.action(["button_1_click".."button_3_release"]); ZHC has
    // no enum/list action type, so this stays String/State. The action
    // strings are produced by kFzOrviboRaw2Action.
    {"action", ExposeType::String, Access::State, nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_T20S3Z{
    .zigbee_models=kModels_T20S3Z, .zigbee_models_count=sizeof(kModels_T20S3Z)/sizeof(kModels_T20S3Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T20S3Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T20S3Z, .from_zigbee_count=sizeof(kFz_T20S3Z)/sizeof(kFz_T20S3Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::orvibo
