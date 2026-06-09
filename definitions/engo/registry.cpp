// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Auto-generated engo registry.
#include "zhc/devices/engo_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::engo {

extern const PreparedDefinition kDefEng__TZE200_awnadkan;
extern const PreparedDefinition kDefEng__TZE200_gtouvmvl;
extern const PreparedDefinition kDefEng__TZE200_oahqgdig;
extern const PreparedDefinition kDefEng__TZE200_zaabefnt;
extern const PreparedDefinition kDefEng__TZE204_ca3i8m8p;
extern const PreparedDefinition kDefEng__TZE204_cg8hdnjv;
extern const PreparedDefinition kDefEng__TZE204_cmyc8g5i;
extern const PreparedDefinition kDefEng__TZE204_djurk6p5;
extern const PreparedDefinition kDefEng__TZE204_glk6viwg;
extern const PreparedDefinition kDefEng__TZE204_lnxdk2ch;

// The seven kDef_E25_230 / kDef_E25_BATB / kDef_E40 / kDef_ECB62_ZB /
// kDef_EONE / kDef_EONE_230W / kDef_EONE_BATB defs were stale battery+on/off
// stubs from an earlier generation pass. Each shared an identical
// {TS0601 + _TZE20x_*} fingerprint with one of the kDefEng__TZE* DP defs
// below, which carry the full thermostat datapoint map. Two defs matching
// the same fingerprint made the resolved behaviour depend on registry order
// (find_definition is first-match), so a device could surface only
// battery+state instead of setpoint/local_temperature/mode. The stubs are
// removed; the kDefEng__TZE* DP defs cover all ten manufacturer names.

const PreparedDefinition* const kEngoRegistry[] = {
    &kDefEng__TZE200_awnadkan,
    &kDefEng__TZE200_gtouvmvl,
    &kDefEng__TZE200_oahqgdig,
    &kDefEng__TZE200_zaabefnt,
    &kDefEng__TZE204_ca3i8m8p,
    &kDefEng__TZE204_cg8hdnjv,
    &kDefEng__TZE204_cmyc8g5i,
    &kDefEng__TZE204_djurk6p5,
    &kDefEng__TZE204_glk6viwg,
    &kDefEng__TZE204_lnxdk2ch,
};
const std::size_t kEngoRegistryCount = sizeof(kEngoRegistry) / sizeof(kEngoRegistry[0]);

}  // namespace zhc::devices::engo
