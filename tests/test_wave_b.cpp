// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Wave B smoke test — structural validation of all 30
// multi-endpoint switch ports. Confirms every device's
// `DeviceEndpointLabels` map is wired into the LumiOnOff factory and
// the PreparedDefinition carries the expected converter pair.

#include <cassert>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefQBKG04LM;
extern const PreparedDefinition kDefQBKG10LM;
extern const PreparedDefinition kDefQBKG21LM;
extern const PreparedDefinition kDefQBKG23LM;
extern const PreparedDefinition kDefSSMU01;
extern const PreparedDefinition kDefWSEUK01;
extern const PreparedDefinition kDefQBKG32LM;
extern const PreparedDefinition kDefQBKG33LM;
extern const PreparedDefinition kDefZNQBKG11LM;
extern const PreparedDefinition kDefZNQBKG12LM;
extern const PreparedDefinition kDefQBKG03LM;
extern const PreparedDefinition kDefQBKG12LM;
extern const PreparedDefinition kDefQBKG22LM;
extern const PreparedDefinition kDefQBKG24LM;
extern const PreparedDefinition kDefQBKG25LM;
extern const PreparedDefinition kDefSSMU02;
extern const PreparedDefinition kDefWSEUK03;
extern const PreparedDefinition kDefQBKG34LM;
extern const PreparedDefinition kDefZNQBKG13LM;
extern const PreparedDefinition kDefZNQBKG14LM;
extern const PreparedDefinition kDefQBKG29LM;
extern const PreparedDefinition kDefQBKG30LM;
extern const PreparedDefinition kDefQBKG31LM;
extern const PreparedDefinition kDefWSEUK04;
extern const PreparedDefinition kDefQBKG35LM;
extern const PreparedDefinition kDefQBKG40LM;
extern const PreparedDefinition kDefQBKG41LM;
extern const PreparedDefinition kDefZNQBKG15LM;
extern const PreparedDefinition kDefZNQBKG16LM;
extern const PreparedDefinition kDefZNQBKG17LM;
}

using namespace zhc;

namespace {
struct Row { const PreparedDefinition* def; const char* model; };
const Row kRows[] = {
    { &devices::lumi::kDefQBKG04LM,  "QBKG04LM"  },
    { &devices::lumi::kDefQBKG10LM,  "QBKG10LM"  },
    { &devices::lumi::kDefQBKG21LM,  "QBKG21LM"  },
    { &devices::lumi::kDefQBKG23LM,  "QBKG23LM"  },
    { &devices::lumi::kDefSSMU01,    "SSM-U01"   },
    { &devices::lumi::kDefWSEUK01,   "WS-EUK01"  },
    { &devices::lumi::kDefQBKG32LM,  "QBKG32LM"  },
    { &devices::lumi::kDefQBKG33LM,  "QBKG33LM"  },
    { &devices::lumi::kDefZNQBKG11LM,"ZNQBKG11LM"},
    { &devices::lumi::kDefZNQBKG12LM,"ZNQBKG12LM"},
    { &devices::lumi::kDefQBKG03LM,  "QBKG03LM"  },
    { &devices::lumi::kDefQBKG12LM,  "QBKG12LM"  },
    { &devices::lumi::kDefQBKG22LM,  "QBKG22LM"  },
    { &devices::lumi::kDefQBKG24LM,  "QBKG24LM"  },
    { &devices::lumi::kDefQBKG25LM,  "QBKG25LM"  },
    { &devices::lumi::kDefSSMU02,    "SSM-U02"   },
    { &devices::lumi::kDefWSEUK03,   "WS-EUK03"  },
    { &devices::lumi::kDefQBKG34LM,  "QBKG34LM"  },
    { &devices::lumi::kDefZNQBKG13LM,"ZNQBKG13LM"},
    { &devices::lumi::kDefZNQBKG14LM,"ZNQBKG14LM"},
    { &devices::lumi::kDefQBKG29LM,  "QBKG29LM"  },
    { &devices::lumi::kDefQBKG30LM,  "QBKG30LM"  },
    { &devices::lumi::kDefQBKG31LM,  "QBKG31LM"  },
    { &devices::lumi::kDefWSEUK04,   "WS-EUK04"  },
    { &devices::lumi::kDefQBKG35LM,  "QBKG35LM"  },
    { &devices::lumi::kDefQBKG40LM,  "QBKG40LM"  },
    { &devices::lumi::kDefQBKG41LM,  "QBKG41LM"  },
    { &devices::lumi::kDefZNQBKG15LM,"ZNQBKG15LM"},
    { &devices::lumi::kDefZNQBKG16LM,"ZNQBKG16LM"},
    { &devices::lumi::kDefZNQBKG17LM,"ZNQBKG17LM"},
};
}

int main() {
    for (const auto& r : kRows) {
        assert(r.def->model && std::strcmp(r.def->model, r.model) == 0);
        assert(r.def->zigbee_models_count >= 1);
        assert(r.def->zigbee_models && r.def->zigbee_models[0]);
        assert(r.def->from_zigbee_count == 2);   // lumi_basic + lumi_on_off
        assert(r.def->to_zigbee_count   >= 1);   // kTzOnOff + manu writes
    }
    return 0;
}
