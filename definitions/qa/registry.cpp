// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// qa registry. Hand-edited 2026-04-28 to drop wrong-bundle duplicate ports for
// TS0601/TS011F Tuya fingerprints that have DP-correct sibling ports
// (Qa__TZE284_*.cpp / Qa__TZ3218_kwht8j5m.cpp). See docs/QA_PARITY.md.
#include "zhc/devices/qa_registry.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::qa {

// DP-correct ports (preferred over the wrong-bundle generic siblings).
extern const PreparedDefinition kDefQa__TZ3218_kwht8j5m;   // covers QASZP
extern const PreparedDefinition kDefQa__TZE284_1aqlsquf;   // covers QAT42Z2B
extern const PreparedDefinition kDefQa__TZE284_ms97nkyy;   // covers QAT44Z6
extern const PreparedDefinition kDefQa__TZE284_nthhgkd6;   // covers QADZ4DIN
extern const PreparedDefinition kDefQa__TZE284_nzns7udm;   // covers QAT42Z1B
extern const PreparedDefinition kDefQa__TZE284_pgxndxp4;   // covers QAT42Z3B

// Generic on/off ports (one model per Tuya fingerprint).
extern const PreparedDefinition kDef_QADZ1;
extern const PreparedDefinition kDef_QADZ2;
extern const PreparedDefinition kDef_QARZ1DC;
extern const PreparedDefinition kDef_QARZ2LR;
extern const PreparedDefinition kDef_QARZ3LR;
extern const PreparedDefinition kDef_QARZ4LR;
extern const PreparedDefinition kDef_QARZDC1LR;
extern const PreparedDefinition kDef_QAT42Z1;
extern const PreparedDefinition kDef_QAT42Z1H;
extern const PreparedDefinition kDef_QAT42Z2;
extern const PreparedDefinition kDef_QAT42Z2H;
extern const PreparedDefinition kDef_QAT42Z3;
extern const PreparedDefinition kDef_QAT42Z3H;
extern const PreparedDefinition kDef_QAT44Z4H;
extern const PreparedDefinition kDef_QAT44Z6H;
extern const PreparedDefinition kDef_QAFZ200;

const PreparedDefinition* const kQaRegistry[] = {
    &kDefQa__TZ3218_kwht8j5m,
    &kDefQa__TZE284_1aqlsquf,
    &kDefQa__TZE284_ms97nkyy,
    &kDefQa__TZE284_nthhgkd6,
    &kDefQa__TZE284_nzns7udm,
    &kDefQa__TZE284_pgxndxp4,
    &kDef_QADZ1,
    &kDef_QADZ2,
    &kDef_QARZ1DC,
    &kDef_QARZ2LR,
    &kDef_QARZ3LR,
    &kDef_QARZ4LR,
    &kDef_QARZDC1LR,
    &kDef_QAT42Z1,
    &kDef_QAT42Z1H,
    &kDef_QAT42Z2,
    &kDef_QAT42Z2H,
    &kDef_QAT42Z3,
    &kDef_QAT42Z3H,
    &kDef_QAT44Z4H,
    &kDef_QAT44Z6H,
    &kDef_QAFZ200,
};
const std::size_t kQaRegistryCount = sizeof(kQaRegistry) / sizeof(kQaRegistry[0]);

}  // namespace zhc::devices::qa
