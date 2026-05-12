// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Cigol Cigol Connect — hand-rewritten from auto-generated stub.
//
// Zigbee interface module for LK IHC installations: up to 32
// push-button inputs and 16 relay outputs over a fixed endpoint plan:
//   inputs  = 1..8, 11..18, 31..38, 41..48
//   outputs = 21..28, 51..58
//
// z2m wires three Fz converters (genMultistateInput, genOnOff,
// genOnOffSwitchCfg) and two Tz converters (state, switch_action).
// We reuse the generic on/off pair for outputs and pull the
// multistate + switchActions decoders/encoder from cigol/_shared.
//
// Multi-EP suffixing: every populated endpoint gets an
// `EndpointLabel{"<ep>", <ep>}` so the dispatcher rewrites
// `state` / `input` / `switch_action` into `state_<ep>` etc.,
// matching z2m's `.withEndpoint("${ep}")` shape.
//
// z2m-source: cigol.ts #Cigol Connect.

#include "definitions/_generic/_shared.hpp"
#include "definitions/cigol/_shared.hpp"

namespace zhc::devices::cigol {
namespace {

const FzConverter* const kFz_Cigol_Connect[] = {
    &::zhc::cigol::kFzCigolInput,                  // genMultistateInput → input
    &::zhc::generic::kFzOnOff,                     // genOnOff           → state
    &::zhc::cigol::kFzCigolSwitchActionReport,     // genOnOffSwitchCfg  → switch_action
};
const TzConverter* const kTz_Cigol_Connect[] = {
    &::zhc::generic::kTzOnOff,                     // state         → onOff cmds
    &::zhc::cigol::kTzCigolSwitchAction,           // switch_action → write 0x0010
};
constexpr const char* kModels_Cigol_Connect[] = { "CIGOL_CONNECT" };

// Endpoint label map. Numeric labels match z2m's per-EP expose key
// suffixes (`state_1`, `input_31`, …). 32 inputs + 16 outputs = 48.
constexpr ::zhc::EndpointLabel kEndpoints_Cigol_Connect[] = {
    // Input bank A (1..8) and B (11..18)
    {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8},
    {"11", 11}, {"12", 12}, {"13", 13}, {"14", 14}, {"15", 15}, {"16", 16}, {"17", 17}, {"18", 18},
    // Output bank A (21..28)
    {"21", 21}, {"22", 22}, {"23", 23}, {"24", 24}, {"25", 25}, {"26", 26}, {"27", 27}, {"28", 28},
    // Input bank A continued (31..38) and B (41..48)
    {"31", 31}, {"32", 32}, {"33", 33}, {"34", 34}, {"35", 35}, {"36", 36}, {"37", 37}, {"38", 38},
    {"41", 41}, {"42", 42}, {"43", 43}, {"44", 44}, {"45", 45}, {"46", 46}, {"47", 47}, {"48", 48},
    // Output bank B (51..58)
    {"51", 51}, {"52", 52}, {"53", 53}, {"54", 54}, {"55", 55}, {"56", 56}, {"57", 57}, {"58", 58},
};

}  // namespace

// Per-EP exposes — one `state` per output EP, `input` + `switch_action`
// per input EP. ZHC has no per-device `endpoint()` filter so we publish
// the union (worst-case Connect carrier with all rails populated).
constexpr Expose kAutoExposes[] = {
    // Inputs A (1..8) — input + switch_action
    {"input_1",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_2",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_3",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_4",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_5",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_6",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_7",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_8",  ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"switch_action_1", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_2", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_3", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_4", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_5", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_6", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_7", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_8", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    // Inputs B (11..18)
    {"input_11", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_12", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_13", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_14", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_15", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_16", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_17", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_18", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"switch_action_11", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_12", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_13", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_14", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_15", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_16", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_17", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_18", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    // Inputs A (31..38)
    {"input_31", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_32", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_33", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_34", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_35", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_36", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_37", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_38", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"switch_action_31", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_32", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_33", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_34", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_35", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_36", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_37", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_38", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    // Inputs B (41..48)
    {"input_41", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_42", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_43", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_44", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_45", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_46", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_47", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"input_48", ExposeType::Enum, Access::State,    nullptr, "off/single/double/hold", nullptr, 0},
    {"switch_action_41", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_42", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_43", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_44", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_45", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_46", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_47", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    {"switch_action_48", ExposeType::Enum, Access::StateSet, nullptr, "off/on/toggle", nullptr, 0},
    // Outputs A (21..28) — relay state
    {"state_21", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_22", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_23", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_24", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_25", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_26", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_27", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_28", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    // Outputs B (51..58)
    {"state_51", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_52", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_53", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_54", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_55", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_56", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_57", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"state_58", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
};

// Bindings: per z2m configure(), input EPs bind genMultistateInput
// (0x0012); output EPs bind genOnOff (0x0006). genOnOffSwitchCfg
// (0x0007) is read but never bound for reporting upstream.
constexpr BindingSpec kAutoBindings[] = {
    // Inputs A (1..8)
    {1,  0x0012}, {2,  0x0012}, {3,  0x0012}, {4,  0x0012},
    {5,  0x0012}, {6,  0x0012}, {7,  0x0012}, {8,  0x0012},
    // Inputs B (11..18)
    {11, 0x0012}, {12, 0x0012}, {13, 0x0012}, {14, 0x0012},
    {15, 0x0012}, {16, 0x0012}, {17, 0x0012}, {18, 0x0012},
    // Inputs A (31..38)
    {31, 0x0012}, {32, 0x0012}, {33, 0x0012}, {34, 0x0012},
    {35, 0x0012}, {36, 0x0012}, {37, 0x0012}, {38, 0x0012},
    // Inputs B (41..48)
    {41, 0x0012}, {42, 0x0012}, {43, 0x0012}, {44, 0x0012},
    {45, 0x0012}, {46, 0x0012}, {47, 0x0012}, {48, 0x0012},
    // Outputs A (21..28)
    {21, 0x0006}, {22, 0x0006}, {23, 0x0006}, {24, 0x0006},
    {25, 0x0006}, {26, 0x0006}, {27, 0x0006}, {28, 0x0006},
    // Outputs B (51..58)
    {51, 0x0006}, {52, 0x0006}, {53, 0x0006}, {54, 0x0006},
    {55, 0x0006}, {56, 0x0006}, {57, 0x0006}, {58, 0x0006},
};

extern const PreparedDefinition kDef_Cigol_Connect{
    .zigbee_models=kModels_Cigol_Connect, .zigbee_models_count=sizeof(kModels_Cigol_Connect)/sizeof(kModels_Cigol_Connect[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="Cigol Connect", .vendor="Cigol",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_Cigol_Connect, .from_zigbee_count=sizeof(kFz_Cigol_Connect)/sizeof(kFz_Cigol_Connect[0]),
    .to_zigbee=kTz_Cigol_Connect, .to_zigbee_count=sizeof(kTz_Cigol_Connect)/sizeof(kTz_Cigol_Connect[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kAutoBindings, .bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
    .reports=nullptr, .reports_count=0,
    .config_steps=nullptr, .config_steps_count=0,
    .config_callbacks=nullptr, .config_callbacks_count=0,
    .endpoint_map=kEndpoints_Cigol_Connect,
    .endpoint_map_count=sizeof(kEndpoints_Cigol_Connect)/sizeof(kEndpoints_Cigol_Connect[0]),
};

}  // namespace zhc::devices::cigol
