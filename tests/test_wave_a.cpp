// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1/2: Wave A smoke test — each ported device resolves to a
// PreparedDefinition with at least one converter, a non-null model,
// and a non-empty zigbee_models list. Proves the bulk-port pipeline
// wired correctly without a per-device test.

#include <cassert>
#include <cstring>

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefRTCGQ12LM;
extern const PreparedDefinition kDefRTCGQ13LM;
extern const PreparedDefinition kDefRTCGQ14LM;
extern const PreparedDefinition kDefMCCGQ11LM;
extern const PreparedDefinition kDefWXKG02LM;
extern const PreparedDefinition kDefWXKG12LM;
extern const PreparedDefinition kDefWSDCGQ11LM;
extern const PreparedDefinition kDefDJT11LM;
extern const PreparedDefinition kDefRTCGQ01LM;
extern const PreparedDefinition kDefMCCGQ14LM;
extern const PreparedDefinition kDefWXKG06LM;
extern const PreparedDefinition kDefWXKG07LM;
extern const PreparedDefinition kDefWXKG13LM;
extern const PreparedDefinition kDefWSDCGQ01LM;
extern const PreparedDefinition kDefSJCGQ11LM;
extern const PreparedDefinition kDefJTYJGD01LM;
extern const PreparedDefinition kDefWXKG14LM;
extern const PreparedDefinition kDefWXKG16LM;
extern const PreparedDefinition kDefWXKG20LM;
extern const PreparedDefinition kDefRTCGQ15LM;
extern const PreparedDefinition kDefMCCGQ13LM;
extern const PreparedDefinition kDefWSDCGQ12LM;
extern const PreparedDefinition kDefSJCGQ12LM;
extern const PreparedDefinition kDefJYGZ01AQ;
extern const PreparedDefinition kDefWXKG15LM;
extern const PreparedDefinition kDefWXKG18LM;
extern const PreparedDefinition kDefWXKG19LM;
extern const PreparedDefinition kDefWXKG21LM;
extern const PreparedDefinition kDefDJT12LM;
extern const PreparedDefinition kDefRTCGQ16LM;
extern const PreparedDefinition kDefMCCGQ12LM;
extern const PreparedDefinition kDefGZCGQ01LM;
extern const PreparedDefinition kDefRTCGQ21LM;
extern const PreparedDefinition kDefRTCGQ22LM;
extern const PreparedDefinition kDefMCCGQ16LM;
extern const PreparedDefinition kDefMCCGQ17LM;
extern const PreparedDefinition kDefWXKG22LM;
extern const PreparedDefinition kDefWXKG23LM;
extern const PreparedDefinition kDefWSDCGQ21LM;
extern const PreparedDefinition kDefDJT21LM;
extern const PreparedDefinition kDefRTCGQ23LM;
extern const PreparedDefinition kDefMCCGQ18LM;
extern const PreparedDefinition kDefMCCGQ19LM;
extern const PreparedDefinition kDefWXKG24LM;
extern const PreparedDefinition kDefWXKG25LM;
extern const PreparedDefinition kDefWXKG26LM;
extern const PreparedDefinition kDefJYGZ02AQ;
extern const PreparedDefinition kDefGZCGQ02LM;
extern const PreparedDefinition kDefWXCJKG11LM;
extern const PreparedDefinition kDefWXCJKG12LM;
extern const PreparedDefinition kDefWXCJKG13LM;
extern const PreparedDefinition kDefRTCGQ24LM;
extern const PreparedDefinition kDefRTCGQ25LM;
extern const PreparedDefinition kDefMCCGQ20LM;
extern const PreparedDefinition kDefSJCGQ13LM;
extern const PreparedDefinition kDefDJT31LM;
extern const PreparedDefinition kDefRTCGQ26LM;
extern const PreparedDefinition kDefRTCGQ27LM;
extern const PreparedDefinition kDefRTCGQ28LM;
extern const PreparedDefinition kDefRTCGQ31LM;
extern const PreparedDefinition kDefRTCGQ32LM;
extern const PreparedDefinition kDefMCCGQ21LM;
extern const PreparedDefinition kDefMCCGQ22LM;
extern const PreparedDefinition kDefMCCGQ23LM;
extern const PreparedDefinition kDefMCCGQ24LM;
extern const PreparedDefinition kDefWXKG27LM;
extern const PreparedDefinition kDefWXKG28LM;
extern const PreparedDefinition kDefWXKG29LM;
extern const PreparedDefinition kDefWXKG30LM;
extern const PreparedDefinition kDefWXKG31LM;
extern const PreparedDefinition kDefWSDCGQ22LM;
extern const PreparedDefinition kDefWSDCGQ23LM;
extern const PreparedDefinition kDefWSDCGQ24LM;
extern const PreparedDefinition kDefGZCGQ11LM;
extern const PreparedDefinition kDefSJCGQ14LM;
extern const PreparedDefinition kDefSJCGQ15LM;
extern const PreparedDefinition kDefJYGZ03AQ;
extern const PreparedDefinition kDefJTBZ01AQ;
extern const PreparedDefinition kDefDJT41LM;
extern const PreparedDefinition kDefDJT51LM;
}

using namespace zhc;

namespace {

struct Row { const PreparedDefinition* def; const char* model; };

const Row kRows[] = {
    { &devices::lumi::kDefRTCGQ12LM,  "RTCGQ12LM"  },
    { &devices::lumi::kDefRTCGQ13LM,  "RTCGQ13LM"  },
    { &devices::lumi::kDefRTCGQ14LM,  "RTCGQ14LM"  },
    { &devices::lumi::kDefMCCGQ11LM,  "MCCGQ11LM"  },
    { &devices::lumi::kDefWXKG02LM,   "WXKG02LM"   },
    { &devices::lumi::kDefWXKG12LM,   "WXKG12LM"   },
    { &devices::lumi::kDefWSDCGQ11LM, "WSDCGQ11LM" },
    { &devices::lumi::kDefDJT11LM,    "DJT11LM"    },
    { &devices::lumi::kDefRTCGQ01LM,  "RTCGQ01LM"  },
    { &devices::lumi::kDefMCCGQ14LM,  "MCCGQ14LM"  },
    { &devices::lumi::kDefWXKG06LM,   "WXKG06LM"   },
    { &devices::lumi::kDefWXKG07LM,   "WXKG07LM"   },
    { &devices::lumi::kDefWXKG13LM,   "WXKG13LM"   },
    { &devices::lumi::kDefWSDCGQ01LM, "WSDCGQ01LM" },
    { &devices::lumi::kDefSJCGQ11LM,  "SJCGQ11LM"  },
    { &devices::lumi::kDefJTYJGD01LM, "JTYJGD01LM" },
    { &devices::lumi::kDefWXKG14LM,   "WXKG14LM"   },
    { &devices::lumi::kDefWXKG16LM,   "WXKG16LM"   },
    { &devices::lumi::kDefWXKG20LM,   "WXKG20LM"   },
    { &devices::lumi::kDefRTCGQ15LM,  "RTCGQ15LM"  },
    { &devices::lumi::kDefMCCGQ13LM,  "MCCGQ13LM"  },
    { &devices::lumi::kDefWSDCGQ12LM, "WSDCGQ12LM" },
    { &devices::lumi::kDefSJCGQ12LM,  "SJCGQ12LM"  },
    { &devices::lumi::kDefJYGZ01AQ,   "JYGZ01AQ"   },
    { &devices::lumi::kDefWXKG15LM,   "WXKG15LM"   },
    { &devices::lumi::kDefWXKG18LM,   "WXKG18LM"   },
    { &devices::lumi::kDefWXKG19LM,   "WXKG19LM"   },
    { &devices::lumi::kDefWXKG21LM,   "WXKG21LM"   },
    { &devices::lumi::kDefDJT12LM,    "DJT12LM"    },
    { &devices::lumi::kDefRTCGQ16LM,  "RTCGQ16LM"  },
    { &devices::lumi::kDefMCCGQ12LM,  "MCCGQ12LM"  },
    { &devices::lumi::kDefGZCGQ01LM,  "GZCGQ01LM"  },
    { &devices::lumi::kDefRTCGQ21LM,  "RTCGQ21LM"  },
    { &devices::lumi::kDefRTCGQ22LM,  "RTCGQ22LM"  },
    { &devices::lumi::kDefMCCGQ16LM,  "MCCGQ16LM"  },
    { &devices::lumi::kDefMCCGQ17LM,  "MCCGQ17LM"  },
    { &devices::lumi::kDefWXKG22LM,   "WXKG22LM"   },
    { &devices::lumi::kDefWXKG23LM,   "WXKG23LM"   },
    { &devices::lumi::kDefWSDCGQ21LM, "WSDCGQ21LM" },
    { &devices::lumi::kDefDJT21LM,    "DJT21LM"    },
    { &devices::lumi::kDefRTCGQ23LM,  "RTCGQ23LM"  },
    { &devices::lumi::kDefMCCGQ18LM,  "MCCGQ18LM"  },
    { &devices::lumi::kDefMCCGQ19LM,  "MCCGQ19LM"  },
    { &devices::lumi::kDefWXKG24LM,   "WXKG24LM"   },
    { &devices::lumi::kDefWXKG25LM,   "WXKG25LM"   },
    { &devices::lumi::kDefWXKG26LM,   "WXKG26LM"   },
    { &devices::lumi::kDefJYGZ02AQ,   "JYGZ02AQ"   },
    { &devices::lumi::kDefGZCGQ02LM,  "GZCGQ02LM"  },
    { &devices::lumi::kDefWXCJKG11LM, "WXCJKG11LM" },
    { &devices::lumi::kDefWXCJKG12LM, "WXCJKG12LM" },
    { &devices::lumi::kDefWXCJKG13LM, "WXCJKG13LM" },
    { &devices::lumi::kDefRTCGQ24LM,  "RTCGQ24LM"  },
    { &devices::lumi::kDefRTCGQ25LM,  "RTCGQ25LM"  },
    { &devices::lumi::kDefMCCGQ20LM,  "MCCGQ20LM"  },
    { &devices::lumi::kDefSJCGQ13LM,  "SJCGQ13LM"  },
    { &devices::lumi::kDefDJT31LM,    "DJT31LM"    },
    { &devices::lumi::kDefRTCGQ26LM,  "RTCGQ26LM"  },
    { &devices::lumi::kDefRTCGQ27LM,  "RTCGQ27LM"  },
    { &devices::lumi::kDefRTCGQ28LM,  "RTCGQ28LM"  },
    { &devices::lumi::kDefRTCGQ31LM,  "RTCGQ31LM"  },
    { &devices::lumi::kDefRTCGQ32LM,  "RTCGQ32LM"  },
    { &devices::lumi::kDefMCCGQ21LM,  "MCCGQ21LM"  },
    { &devices::lumi::kDefMCCGQ22LM,  "MCCGQ22LM"  },
    { &devices::lumi::kDefMCCGQ23LM,  "MCCGQ23LM"  },
    { &devices::lumi::kDefMCCGQ24LM,  "MCCGQ24LM"  },
    { &devices::lumi::kDefWXKG27LM,   "WXKG27LM"   },
    { &devices::lumi::kDefWXKG28LM,   "WXKG28LM"   },
    { &devices::lumi::kDefWXKG29LM,   "WXKG29LM"   },
    { &devices::lumi::kDefWXKG30LM,   "WXKG30LM"   },
    { &devices::lumi::kDefWXKG31LM,   "WXKG31LM"   },
    { &devices::lumi::kDefWSDCGQ22LM, "WSDCGQ22LM" },
    { &devices::lumi::kDefWSDCGQ23LM, "WSDCGQ23LM" },
    { &devices::lumi::kDefWSDCGQ24LM, "WSDCGQ24LM" },
    { &devices::lumi::kDefGZCGQ11LM,  "GZCGQ11LM"  },
    { &devices::lumi::kDefSJCGQ14LM,  "SJCGQ14LM"  },
    { &devices::lumi::kDefSJCGQ15LM,  "SJCGQ15LM"  },
    { &devices::lumi::kDefJYGZ03AQ,   "JYGZ03AQ"   },
    { &devices::lumi::kDefJTBZ01AQ,   "JTBZ01AQ"   },
    { &devices::lumi::kDefDJT41LM,    "DJT41LM"    },
    { &devices::lumi::kDefDJT51LM,    "DJT51LM"    },
};

}  // namespace

int main() {
    for (const auto& r : kRows) {
        assert(r.def != nullptr);
        assert(r.def->model && std::strcmp(r.def->model, r.model) == 0);
        assert(r.def->vendor && std::strcmp(r.def->vendor, "Xiaomi") == 0);
        assert(r.def->zigbee_models_count >= 1);
        assert(r.def->zigbee_models && r.def->zigbee_models[0] != nullptr);
        assert(r.def->from_zigbee_count >= 1);
        assert(r.def->from_zigbee != nullptr);
        for (std::uint8_t i = 0; i < r.def->from_zigbee_count; ++i) {
            assert(r.def->from_zigbee[i] != nullptr);
            assert(r.def->from_zigbee[i]->cluster != nullptr);
        }
    }
    return 0;
}
