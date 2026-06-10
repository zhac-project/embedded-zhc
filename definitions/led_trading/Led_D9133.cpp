// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: LED-Trading 9133 — self-powered Green Power pushbutton transmitter.
// Pushbutton transmitter module.
// z2m-source: led_trading.ts #9133 (fzLocal.led_trading_9133).
//
// PARITY FIX (z2m<->embedded-zhc): the generated port mis-classified this
// self-powered Green Power transmitter as a *controllable* on/off relay —
// it wired generic kFzOnOff + kTzOnOff, declared a dead `state` Binary
// expose, and bound genOnOff (0x0006) to the coordinator. z2m gives this
// device `toZigbee: []` (it is NOT controllable) and a single
// `e.action([...])` expose. The phantom relay role is removed here and the
// schema now matches z2m: an `action` enum carrying the pushbutton events.
//
// RUNTIME BLOCKER (deferred — Green Power infra): z2m decodes the action
// via `cluster: "greenPower"` + `type: commandNotification`, mapping
// `msg.data.commandID` through a lookup (19->press_1 … 30->hold_4).
// embedded-zhc has no Green Power frame family yet (`FrameFamily` is only
// {Zcl, TuyaDp}, `cluster_id_to_name` has no 0x0021, and the parser emits
// no commandNotification frame), so the commandID->action decode cannot be
// wired. INFRA, out of scope for this vendor pass. Schema-only here
// (from_zigbee/to_zigbee/bindings null), matching the enocean PTM 216Z and
// adeo/easyiot deferred-decoder precedents.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::led_trading {
namespace {
constexpr const char* kModels_D9133[] = { "GreenPower_2" };

// z2m e.action([...]) for fzLocal.led_trading_9133.
constexpr const char* kActions_D9133[] = {
    "press_1", "hold_1", "press_2", "hold_2",
    "press_3", "hold_3", "press_4", "hold_4",
};

constexpr Expose kExposes_D9133[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kActions_D9133, sizeof(kActions_D9133)/sizeof(kActions_D9133[0])},
};

}  // namespace

extern const PreparedDefinition kDef_D9133{
    .zigbee_models=kModels_D9133, .zigbee_models_count=sizeof(kModels_D9133)/sizeof(kModels_D9133[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="9133", .vendor="LedTrading",
    .meta=nullptr, .exposes=kExposes_D9133, .exposes_count=sizeof(kExposes_D9133)/sizeof(kExposes_D9133[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=nullptr, .bindings_count=0,
};

}  // namespace zhc::devices::led_trading
