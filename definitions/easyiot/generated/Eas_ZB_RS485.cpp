// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Easyiot ZB-RS485 — auto-generated.
// Zigbee to RS485 controller
// z2m-source: easyiot.ts #ZB-RS485.
//
// RUNTIME BLOCKER: identical to ZB-IR01 — talks seTunneling
// (0x1000) with the Easyiot data-point protocol. send_command and
// rs485_config (composite: baud_rate / parity / stop_bits) need a
// vendor-specific encoder. Exposed here as a contract; runtime path
// is a v2 follow-up.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::easyiot {
namespace {
constexpr const char* kModels_ZB_RS485[] = { "ZB-RS485" };

// TODO(easyiot/ZB-RS485): wire fz/tz against seTunneling cluster
// data-point protocol (tunnelId 0x0001, attr-id|len|type|payload).

constexpr Expose kExposes_ZB_RS485[] = {
    {"last_received_command", ExposeType::String,  Access::State, nullptr, nullptr, nullptr, 0},
    {"send_command",          ExposeType::String,  Access::Set,   nullptr, nullptr, nullptr, 0},
    // rs485_config composite is flattened into individual exposes —
    // ZHC has no composite/struct expose type.
    {"baud_rate",             ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
    {"parity",                ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
    {"stop_bits",             ExposeType::Enum,    Access::Set,   nullptr, nullptr, nullptr, 0},
};

constexpr BindingSpec kBindings_ZB_RS485[] = {
    {1, 0x1000},  // seTunneling
};

}  // namespace

extern const PreparedDefinition kDef_ZB_RS485{
    .zigbee_models=kModels_ZB_RS485, .zigbee_models_count=sizeof(kModels_ZB_RS485)/sizeof(kModels_ZB_RS485[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="ZB-RS485", .vendor="Easyiot",
    .meta=nullptr, .exposes=kExposes_ZB_RS485, .exposes_count=sizeof(kExposes_ZB_RS485)/sizeof(kExposes_ZB_RS485[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=nullptr, .from_zigbee_count=0,
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_ZB_RS485, .bindings_count=sizeof(kBindings_ZB_RS485)/sizeof(kBindings_ZB_RS485[0]),
};

}  // namespace zhc::devices::easyiot
