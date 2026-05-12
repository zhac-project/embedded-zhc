// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Wave C smoke — 25 Lumi lights through factory::LumiLight.
#include <cassert>
#include <cstring>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefZNLDP12LM;
extern const PreparedDefinition kDefZNLDP13LM;
extern const PreparedDefinition kDefZNLDP14LM;
extern const PreparedDefinition kDefZNLDP15LM;
extern const PreparedDefinition kDefZNLDP16LM;
extern const PreparedDefinition kDefZNLDP17LM;
extern const PreparedDefinition kDefZNLDP18LM;
extern const PreparedDefinition kDefZNLDP19LM;
extern const PreparedDefinition kDefZNLDP20LM;
extern const PreparedDefinition kDefZNLDP21LM;
extern const PreparedDefinition kDefZNLDP22LM;
extern const PreparedDefinition kDefZNLDP23LM;
extern const PreparedDefinition kDefZNLDP24LM;
extern const PreparedDefinition kDefZNLDP25LM;
extern const PreparedDefinition kDefZNLDP26LM;
extern const PreparedDefinition kDefZNLDP27LM;
extern const PreparedDefinition kDefZNLDP28LM;
extern const PreparedDefinition kDefZNLDP29LM;
extern const PreparedDefinition kDefZNLDP30LM;
extern const PreparedDefinition kDefZNLDP31LM;
extern const PreparedDefinition kDefZNXNKG01LM;
extern const PreparedDefinition kDefZNXNKG02LM;
extern const PreparedDefinition kDefZNXNKG03LM;
extern const PreparedDefinition kDefZNXNKG04LM;
extern const PreparedDefinition kDefZNXNKG05LM;
}

using namespace zhc;

namespace {
struct Row { const PreparedDefinition* def; const char* model; std::uint8_t tz_expected; };
const Row kRows[] = {
    { &devices::lumi::kDefZNLDP12LM, "ZNLDP12LM", 3 },
    { &devices::lumi::kDefZNLDP13LM, "ZNLDP13LM", 3 },
    { &devices::lumi::kDefZNLDP14LM, "ZNLDP14LM", 3 },
    { &devices::lumi::kDefZNLDP15LM, "ZNLDP15LM", 3 },
    { &devices::lumi::kDefZNLDP16LM, "ZNLDP16LM", 3 },
    { &devices::lumi::kDefZNLDP17LM, "ZNLDP17LM", 3 },
    { &devices::lumi::kDefZNLDP18LM, "ZNLDP18LM", 3 },
    { &devices::lumi::kDefZNLDP19LM, "ZNLDP19LM", 3 },
    { &devices::lumi::kDefZNLDP20LM, "ZNLDP20LM", 3 },
    { &devices::lumi::kDefZNLDP21LM, "ZNLDP21LM", 3 },
    { &devices::lumi::kDefZNLDP22LM, "ZNLDP22LM", 3 },
    { &devices::lumi::kDefZNLDP23LM, "ZNLDP23LM", 3 },
    { &devices::lumi::kDefZNLDP24LM, "ZNLDP24LM", 3 },
    { &devices::lumi::kDefZNLDP25LM, "ZNLDP25LM", 3 },
    { &devices::lumi::kDefZNLDP26LM, "ZNLDP26LM", 3 },
    { &devices::lumi::kDefZNLDP27LM, "ZNLDP27LM", 3 },
    { &devices::lumi::kDefZNLDP28LM, "ZNLDP28LM", 3 },
    { &devices::lumi::kDefZNLDP29LM, "ZNLDP29LM", 3 },
    { &devices::lumi::kDefZNLDP30LM, "ZNLDP30LM", 3 },
    { &devices::lumi::kDefZNLDP31LM, "ZNLDP31LM", 3 },
    { &devices::lumi::kDefZNXNKG01LM,"ZNXNKG01LM",2 },
    { &devices::lumi::kDefZNXNKG02LM,"ZNXNKG02LM",2 },
    { &devices::lumi::kDefZNXNKG03LM,"ZNXNKG03LM",2 },
    { &devices::lumi::kDefZNXNKG04LM,"ZNXNKG04LM",2 },
    { &devices::lumi::kDefZNXNKG05LM,"ZNXNKG05LM",2 },
};
}

int main() {
    for (const auto& r : kRows) {
        assert(r.def->model && std::strcmp(r.def->model, r.model) == 0);
        assert(r.def->zigbee_models_count >= 1);
        assert(r.def->to_zigbee_count   == r.tz_expected);
    }
    return 0;
}
