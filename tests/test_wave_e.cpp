// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Wave E smoke — 19 outliers. Structural checks only; many
// devices expose only lumi_basic until v2 adds IAS / manu-specific
// cluster paths.
// (WRDZG01LM dropped — no generated def in definitions/lumi.)
#include <cassert>
#include <cstring>
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefMFKZQ01LM;
extern const PreparedDefinition kDefCTPR01;
extern const PreparedDefinition kDefZNCLDJ11LM;
extern const PreparedDefinition kDefZNCLDJ12LM;
extern const PreparedDefinition kDefZNCLDJ14LM;
extern const PreparedDefinition kDefZNCLBL01LM;
extern const PreparedDefinition kDefZNMS11LM;
extern const PreparedDefinition kDefZNMS12LM;
extern const PreparedDefinition kDefZNMS13LM;
extern const PreparedDefinition kDefRTCZCGQ11LM;
extern const PreparedDefinition kDefRTCZCGQ12LM;
extern const PreparedDefinition kDefZNQPKG01LM;
extern const PreparedDefinition kDefSPPDC01;
extern const PreparedDefinition kDefZNCLDJ15LM;
extern const PreparedDefinition kDefSRSCM01;
extern const PreparedDefinition kDefZNXJKG01LM;
extern const PreparedDefinition kDefZNWXKG01LM;
extern const PreparedDefinition kDefSPWP01;
extern const PreparedDefinition kDefWGLZ00LM;
}

using namespace zhc;

namespace {
struct Row { const PreparedDefinition* def; const char* model; };
const Row kRows[] = {
    { &devices::lumi::kDefMFKZQ01LM,   "MFKZQ01LM"  },
    { &devices::lumi::kDefCTPR01,      "CTP-R01"    },
    { &devices::lumi::kDefZNCLDJ11LM,  "ZNCLDJ11LM" },
    { &devices::lumi::kDefZNCLDJ12LM,  "ZNCLDJ12LM" },
    { &devices::lumi::kDefZNCLDJ14LM,  "ZNCLDJ14LM" },
    { &devices::lumi::kDefZNCLBL01LM,  "ZNCLBL01LM" },
    { &devices::lumi::kDefZNMS11LM,    "ZNMS11LM"   },
    { &devices::lumi::kDefZNMS12LM,    "ZNMS12LM"   },
    { &devices::lumi::kDefZNMS13LM,    "ZNMS13LM"   },
    { &devices::lumi::kDefRTCZCGQ11LM, "RTCZCGQ11LM"},
    { &devices::lumi::kDefRTCZCGQ12LM, "RTCZCGQ12LM"},
    { &devices::lumi::kDefZNQPKG01LM,  "ZNQPKG01LM" },
    { &devices::lumi::kDefSPPDC01,     "SP-PDC01"   },
    { &devices::lumi::kDefZNCLDJ15LM,  "ZNCLDJ15LM" },
    { &devices::lumi::kDefSRSCM01,     "SRSC-M01"   },
    { &devices::lumi::kDefZNXJKG01LM,  "ZNXJKG01LM" },
    { &devices::lumi::kDefZNWXKG01LM,  "ZNWXKG01LM" },
    { &devices::lumi::kDefSPWP01,      "SP-WP01"    },
    { &devices::lumi::kDefWGLZ00LM,    "WGLZ00LM"   },
};
}

int main() {
    for (const auto& r : kRows) {
        assert(r.def->model && std::strcmp(r.def->model, r.model) == 0);
        assert(r.def->zigbee_models_count >= 1);
        assert(r.def->from_zigbee_count >= 1);
    }
    return 0;
}
