// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Wave D smoke — 15 plugs through factory::LumiPower.
#include <cassert>
#include <cstring>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefZNCZ03LM;
extern const PreparedDefinition kDefZNCZ04LM;
extern const PreparedDefinition kDefZNCZ11LM;
extern const PreparedDefinition kDefZNCZ12LM;
extern const PreparedDefinition kDefZNCZ14LM;
extern const PreparedDefinition kDefSPEUC01;
extern const PreparedDefinition kDefWPP01D;
extern const PreparedDefinition kDefQBCZ11LM;
extern const PreparedDefinition kDefQBCZ12LM;
extern const PreparedDefinition kDefQBCZ13LM;
extern const PreparedDefinition kDefZNMCPZ01LM;
extern const PreparedDefinition kDefZNMCPZ02LM;
extern const PreparedDefinition kDefSPEUC02;
extern const PreparedDefinition kDefWPP02D;
extern const PreparedDefinition kDefWPP03D;
}

using namespace zhc;

namespace {
struct Row { const PreparedDefinition* def; const char* model; };
const Row kRows[] = {
    { &devices::lumi::kDefZNCZ03LM,   "ZNCZ03LM"   },
    { &devices::lumi::kDefZNCZ04LM,   "ZNCZ04LM"   },
    { &devices::lumi::kDefZNCZ11LM,   "ZNCZ11LM"   },
    { &devices::lumi::kDefZNCZ12LM,   "ZNCZ12LM"   },
    { &devices::lumi::kDefZNCZ14LM,   "ZNCZ14LM"   },
    { &devices::lumi::kDefSPEUC01,    "SP-EUC01"   },
    { &devices::lumi::kDefWPP01D,     "WP-P01D"    },
    { &devices::lumi::kDefQBCZ11LM,   "QBCZ11LM"   },
    { &devices::lumi::kDefQBCZ12LM,   "QBCZ12LM"   },
    { &devices::lumi::kDefQBCZ13LM,   "QBCZ13LM"   },
    { &devices::lumi::kDefZNMCPZ01LM, "ZNMCPZ01LM" },
    { &devices::lumi::kDefZNMCPZ02LM, "ZNMCPZ02LM" },
    { &devices::lumi::kDefSPEUC02,    "SP-EUC02"   },
    { &devices::lumi::kDefWPP02D,     "WP-P02D"    },
    { &devices::lumi::kDefWPP03D,     "WP-P03D"    },
};
}

int main() {
    for (const auto& r : kRows) {
        assert(r.def->model && std::strcmp(r.def->model, r.model) == 0);
        assert(r.def->from_zigbee_count == 4);   // basic + onoff + power + meter
        assert(r.def->to_zigbee_count   >= 1);   // kTzOnOff + manu writes
    }
    return 0;
}
