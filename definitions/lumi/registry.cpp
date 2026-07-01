// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lumi device registry — auto-aggregated.
// Every file under definitions/lumi/*.cpp (except _shared/registry) contributes
// one extern PreparedDefinition; this file collects them into a single
// fixed-size array for platform adapters.

#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {
extern const PreparedDefinition kDefC200;
extern const PreparedDefinition kDefFP310;
extern const PreparedDefinition kDefSSWQD22LM;
extern const PreparedDefinition kDefPS_S04D;
extern const PreparedDefinition kDefUT_A01E;
extern const PreparedDefinition kDefWT_A03E;
extern const PreparedDefinition kDefWS_K03E;
extern const PreparedDefinition kDefTH_S04D;
extern const PreparedDefinition kDefWS_K02E;
extern const PreparedDefinition kDefWS_K04E;
extern const PreparedDefinition kDefWS_K05E;
extern const PreparedDefinition kDefKD_R01D;
extern const PreparedDefinition kDefDS_K02D_DS_K02E;
extern const PreparedDefinition kDefDWZTCGQ11LM;
extern const PreparedDefinition kDefWS_K08D;
extern const PreparedDefinition kDefT2_E27_CCT;
extern const PreparedDefinition kDefT2_E27;
extern const PreparedDefinition kDefWS_K07E;
extern const PreparedDefinition kDefVC_X01D;
extern const PreparedDefinition kDefCL_L02D;
extern const PreparedDefinition kDefWS_K01D;
extern const PreparedDefinition kDefZNQBKG45LM;
extern const PreparedDefinition kDefZNQBKG44LM;
extern const PreparedDefinition kDefZNQBKG43LM;
extern const PreparedDefinition kDefZNQBKG42LM;
extern const PreparedDefinition kDefZNQBKG41LM;
extern const PreparedDefinition kDefZNQBKG40LM;
extern const PreparedDefinition kDefZNQBKG39LM;
extern const PreparedDefinition kDefZNQBKG38LM;
extern const PreparedDefinition kDefZNCWWSQ01LM;
extern const PreparedDefinition kDefSRTS_A01;
extern const PreparedDefinition kDefWXKG17LM;
extern const PreparedDefinition kDefQBCZ14LM;
extern const PreparedDefinition kDefQBCZ15LM;
extern const PreparedDefinition kDefVOCKQJK11LM;
extern const PreparedDefinition kDefQBKG39LM;
extern const PreparedDefinition kDefQBKG38LM;
extern const PreparedDefinition kDefZNCJMB14LM;
extern const PreparedDefinition kDefSSWQD03LM;
extern const PreparedDefinition kDefZNDDQDQ13LM;
extern const PreparedDefinition kDefZNDDQDQ12LM;
extern const PreparedDefinition kDefZNDDQDQ11LM;
extern const PreparedDefinition kDefHLQDQ01LM;
extern const PreparedDefinition kDefZNTGMK12LM;
extern const PreparedDefinition kDefTDL01LM;
extern const PreparedDefinition kDefLLKZMK12LM;
extern const PreparedDefinition kDefLLKZMK11LM;
extern const PreparedDefinition kDefZNCLDJ01LM;
extern const PreparedDefinition kDefZNGZDJ16LM;
extern const PreparedDefinition kDefZNGZDJ11LM;
extern const PreparedDefinition kDefJTQJ_BF_01LM_BW;
extern const PreparedDefinition kDefZNQBCZ11LM;
extern const PreparedDefinition kDefFP1E;
extern const PreparedDefinition kDefQBKG20LM;
extern const PreparedDefinition kDefQBKG19LM;
extern const PreparedDefinition kDefQBKG18LM;
extern const PreparedDefinition kDefQBKG17LM;
extern const PreparedDefinition kDefQBKG26LM;
extern const PreparedDefinition kDefQBKG28LM;
extern const PreparedDefinition kDefQBKG27LM;
extern const PreparedDefinition kDefWS_USC02;
extern const PreparedDefinition kDefWS_USC01;
extern const PreparedDefinition kDefLGYCDD01LM;
extern const PreparedDefinition kDefJWDL001A;
extern const PreparedDefinition kDefJWSP001A;
extern const PreparedDefinition kDefKQJCMB11LM;

extern const PreparedDefinition kDefCTPR01;
extern const PreparedDefinition kDefDJT11LM;
extern const PreparedDefinition kDefDJT12LM;
extern const PreparedDefinition kDefDJT21LM;
extern const PreparedDefinition kDefDJT31LM;
extern const PreparedDefinition kDefDJT41LM;
extern const PreparedDefinition kDefDJT51LM;
extern const PreparedDefinition kDefGZCGQ01LM;
extern const PreparedDefinition kDefGZCGQ11LM;
extern const PreparedDefinition kDefJTBZ01AQ;
extern const PreparedDefinition kDefJTYJGD01LM;
extern const PreparedDefinition kDefJYGZ01AQ;
extern const PreparedDefinition kDefJYGZ03AQ;
extern const PreparedDefinition kDefLGZCDD01LM;
extern const PreparedDefinition kDefMCCGQ01LM;
extern const PreparedDefinition kDefMCCGQ11LM;
extern const PreparedDefinition kDefMCCGQ12LM;
extern const PreparedDefinition kDefMCCGQ13LM;
extern const PreparedDefinition kDefMCCGQ14LM;
extern const PreparedDefinition kDefMCCGQ16LM;
extern const PreparedDefinition kDefMCCGQ17LM;
extern const PreparedDefinition kDefMCCGQ18LM;
extern const PreparedDefinition kDefMCCGQ19LM;
extern const PreparedDefinition kDefMCCGQ20LM;
extern const PreparedDefinition kDefMCCGQ21LM;
extern const PreparedDefinition kDefMCCGQ22LM;
extern const PreparedDefinition kDefMCCGQ23LM;
extern const PreparedDefinition kDefMCCGQ24LM;
extern const PreparedDefinition kDefMFKZQ01LM;
extern const PreparedDefinition kDefQBCZ11LM;
extern const PreparedDefinition kDefQBCZ13LM;
extern const PreparedDefinition kDefQBKG03LM;
extern const PreparedDefinition kDefQBKG04LM;
extern const PreparedDefinition kDefQBKG11LM;
extern const PreparedDefinition kDefQBKG12LM;
extern const PreparedDefinition kDefQBKG21LM;
extern const PreparedDefinition kDefQBKG22LM;
extern const PreparedDefinition kDefQBKG23LM;
extern const PreparedDefinition kDefQBKG24LM;
extern const PreparedDefinition kDefQBKG25LM;
extern const PreparedDefinition kDefQBKG29LM;
extern const PreparedDefinition kDefQBKG30LM;
extern const PreparedDefinition kDefQBKG31LM;
extern const PreparedDefinition kDefQBKG32LM;
extern const PreparedDefinition kDefQBKG33LM;
extern const PreparedDefinition kDefQBKG34LM;
extern const PreparedDefinition kDefQBKG35LM;
extern const PreparedDefinition kDefQBKG40LM;
extern const PreparedDefinition kDefQBKG41LM;
extern const PreparedDefinition kDefRTCGQ01LM;
extern const PreparedDefinition kDefRTCGQ11LM;
extern const PreparedDefinition kDefRTCGQ12LM;
extern const PreparedDefinition kDefRTCGQ13LM;
extern const PreparedDefinition kDefRTCGQ14LM;
extern const PreparedDefinition kDefRTCGQ15LM;
extern const PreparedDefinition kDefRTCGQ16LM;
extern const PreparedDefinition kDefRTCGQ21LM;
extern const PreparedDefinition kDefRTCGQ22LM;
extern const PreparedDefinition kDefRTCGQ24LM;
extern const PreparedDefinition kDefRTCGQ25LM;
extern const PreparedDefinition kDefRTCGQ26LM;
extern const PreparedDefinition kDefRTCGQ27LM;
extern const PreparedDefinition kDefRTCGQ28LM;
extern const PreparedDefinition kDefRTCGQ31LM;
extern const PreparedDefinition kDefRTCGQ32LM;
extern const PreparedDefinition kDefRTCZCGQ11LM;
extern const PreparedDefinition kDefRTCZCGQ12LM;
extern const PreparedDefinition kDefSJCGQ11LM;
extern const PreparedDefinition kDefSJCGQ12LM;
extern const PreparedDefinition kDefSJCGQ13LM;
extern const PreparedDefinition kDefSJCGQ14LM;
extern const PreparedDefinition kDefSJCGQ15LM;
extern const PreparedDefinition kDefSPEUC01;
extern const PreparedDefinition kDefSPEUC02;
extern const PreparedDefinition kDefSPPDC01;
extern const PreparedDefinition kDefSPWP01;
extern const PreparedDefinition kDefSRSCM01;
extern const PreparedDefinition kDefSSMU01;
extern const PreparedDefinition kDefSSMU02;
extern const PreparedDefinition kDefWGLZ00LM;
extern const PreparedDefinition kDefWPP01D;
extern const PreparedDefinition kDefWPP02D;
extern const PreparedDefinition kDefWPP03D;
extern const PreparedDefinition kDefWSDCGQ01LM;
extern const PreparedDefinition kDefWSDCGQ11LM;
extern const PreparedDefinition kDefWSDCGQ12LM;
extern const PreparedDefinition kDefWSDCGQ21LM;
extern const PreparedDefinition kDefWSDCGQ22LM;
extern const PreparedDefinition kDefWSDCGQ23LM;
extern const PreparedDefinition kDefWSDCGQ24LM;
extern const PreparedDefinition kDefWSEUK01;
extern const PreparedDefinition kDefWSEUK03;
extern const PreparedDefinition kDefWSEUK04;
extern const PreparedDefinition kDefWXCJKG11LM;
extern const PreparedDefinition kDefWXCJKG12LM;
extern const PreparedDefinition kDefWXCJKG13LM;
extern const PreparedDefinition kDefWXKG01LM;
extern const PreparedDefinition kDefWXKG02LM;
extern const PreparedDefinition kDefWXKG03LM;
extern const PreparedDefinition kDefWXKG04LM;
extern const PreparedDefinition kDefWXKG06LM;
extern const PreparedDefinition kDefWXKG07LM;
extern const PreparedDefinition kDefWXKG11LM;
extern const PreparedDefinition kDefWXKG12LM;
extern const PreparedDefinition kDefWXKG13LM;
extern const PreparedDefinition kDefWXKG14LM;
extern const PreparedDefinition kDefWXKG15LM;
extern const PreparedDefinition kDefWXKG16LM;
extern const PreparedDefinition kDefWXKG18LM;
extern const PreparedDefinition kDefWXKG20LM;
extern const PreparedDefinition kDefWXKG21LM;
extern const PreparedDefinition kDefWXKG22LM;
extern const PreparedDefinition kDefWXKG23LM;
extern const PreparedDefinition kDefWXKG24LM;
extern const PreparedDefinition kDefWXKG25LM;
extern const PreparedDefinition kDefWXKG26LM;
extern const PreparedDefinition kDefWXKG27LM;
extern const PreparedDefinition kDefWXKG28LM;
extern const PreparedDefinition kDefWXKG29LM;
extern const PreparedDefinition kDefWXKG30LM;
extern const PreparedDefinition kDefWXKG31LM;
extern const PreparedDefinition kDefZNCLBL01LM;
extern const PreparedDefinition kDefZNCLDJ11LM;
extern const PreparedDefinition kDefZNCLDJ12LM;
extern const PreparedDefinition kDefZNCLDJ14LM;
extern const PreparedDefinition kDefZNCLDJ15LM;
extern const PreparedDefinition kDefZNCZ02LM;
extern const PreparedDefinition kDefZNCZ03LM;
extern const PreparedDefinition kDefZNCZ04LM;
extern const PreparedDefinition kDefZNCZ11LM;
extern const PreparedDefinition kDefZNCZ12LM;
extern const PreparedDefinition kDefZNCZ14LM;
extern const PreparedDefinition kDefZNCZ15LM;
extern const PreparedDefinition kDefZNLDP12LM;
extern const PreparedDefinition kDefZNLDP13LM;
extern const PreparedDefinition kDefZNLDP14LM;
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
extern const PreparedDefinition kDefZNLDP30LM;
extern const PreparedDefinition kDefZNLDP31LM;
extern const PreparedDefinition kDefZNMCPZ01LM;
extern const PreparedDefinition kDefZNMCPZ02LM;
extern const PreparedDefinition kDefZNMS11LM;
extern const PreparedDefinition kDefZNMS12LM;
extern const PreparedDefinition kDefZNMS13LM;
extern const PreparedDefinition kDefZNQBKG11LM;
extern const PreparedDefinition kDefZNQBKG12LM;
extern const PreparedDefinition kDefZNQBKG13LM;
extern const PreparedDefinition kDefZNQBKG14LM;
extern const PreparedDefinition kDefZNQBKG15LM;
extern const PreparedDefinition kDefZNQBKG16LM;
extern const PreparedDefinition kDefZNQBKG17LM;
extern const PreparedDefinition kDefZNQPKG01LM;
extern const PreparedDefinition kDefZNWXKG01LM;
extern const PreparedDefinition kDefZNXJKG01LM;
extern const PreparedDefinition kDefZNXNKG01LM;
extern const PreparedDefinition kDefZNXNKG02LM;
extern const PreparedDefinition kDefZNXNKG03LM;
extern const PreparedDefinition kDefZNXNKG04LM;
extern const PreparedDefinition kDefZNXNKG05LM;

extern const PreparedDefinition* const kLumiRegistry[] = {
    &kDefCTPR01,
    &kDefDJT11LM,
    &kDefDJT12LM,
    &kDefDJT21LM,
    &kDefDJT31LM,
    &kDefDJT41LM,
    &kDefDJT51LM,
    &kDefDWZTCGQ11LM,
    &kDefGZCGQ01LM,
    &kDefGZCGQ11LM,
    &kDefJTBZ01AQ,
    &kDefJTYJGD01LM,
    &kDefJYGZ01AQ,
    &kDefJYGZ03AQ,
    &kDefLGZCDD01LM,
    &kDefMCCGQ01LM,
    &kDefMCCGQ11LM,
    &kDefMCCGQ12LM,
    &kDefMCCGQ13LM,
    &kDefMCCGQ14LM,
    &kDefMCCGQ16LM,
    &kDefMCCGQ17LM,
    &kDefMCCGQ18LM,
    &kDefMCCGQ19LM,
    &kDefMCCGQ20LM,
    &kDefMCCGQ21LM,
    &kDefMCCGQ22LM,
    &kDefMCCGQ23LM,
    &kDefMCCGQ24LM,
    &kDefMFKZQ01LM,
    &kDefQBCZ11LM,
    &kDefQBCZ13LM,
    &kDefQBKG03LM,
    &kDefQBKG04LM,
    &kDefQBKG11LM,
    &kDefQBKG12LM,
    &kDefQBKG21LM,
    &kDefQBKG22LM,
    &kDefQBKG23LM,
    &kDefQBKG24LM,
    &kDefQBKG25LM,
    &kDefQBKG29LM,
    &kDefQBKG30LM,
    &kDefQBKG31LM,
    &kDefQBKG32LM,
    &kDefQBKG33LM,
    &kDefQBKG34LM,
    &kDefQBKG35LM,
    &kDefQBKG40LM,
    &kDefQBKG41LM,
    &kDefRTCGQ01LM,
    &kDefRTCGQ11LM,
    &kDefRTCGQ12LM,
    &kDefRTCGQ13LM,
    &kDefRTCGQ14LM,
    &kDefRTCGQ15LM,
    &kDefRTCGQ16LM,
    &kDefRTCGQ21LM,
    &kDefRTCGQ22LM,
    &kDefRTCGQ24LM,
    &kDefRTCGQ25LM,
    &kDefRTCGQ26LM,
    &kDefRTCGQ27LM,
    &kDefRTCGQ28LM,
    &kDefRTCGQ31LM,
    &kDefRTCGQ32LM,
    &kDefRTCZCGQ11LM,
    &kDefRTCZCGQ12LM,
    &kDefSJCGQ11LM,
    &kDefSJCGQ12LM,
    &kDefSJCGQ13LM,
    &kDefSJCGQ14LM,
    &kDefSJCGQ15LM,
    &kDefSPEUC01,
    &kDefSPEUC02,
    &kDefSPPDC01,
    &kDefSPWP01,
    &kDefSRSCM01,
    &kDefSSMU01,
    &kDefSSMU02,
    &kDefWGLZ00LM,
    &kDefWPP01D,
    &kDefWPP02D,
    &kDefWPP03D,
    &kDefWSDCGQ01LM,
    &kDefWSDCGQ11LM,
    &kDefWSDCGQ12LM,
    &kDefWSDCGQ21LM,
    &kDefWSDCGQ22LM,
    &kDefWSDCGQ23LM,
    &kDefWSDCGQ24LM,
    &kDefWSEUK01,
    &kDefWSEUK03,
    &kDefWSEUK04,
    &kDefWXCJKG11LM,
    &kDefWXCJKG12LM,
    &kDefWXCJKG13LM,
    &kDefWXKG01LM,
    &kDefWXKG02LM,
    &kDefWXKG03LM,
    &kDefWXKG04LM,
    &kDefWXKG06LM,
    &kDefWXKG07LM,
    &kDefWXKG11LM,
    &kDefWXKG12LM,
    &kDefWXKG13LM,
    &kDefWXKG14LM,
    &kDefWXKG15LM,
    &kDefWXKG16LM,
    &kDefWXKG18LM,
    &kDefWXKG20LM,
    &kDefWXKG21LM,
    &kDefWXKG22LM,
    &kDefWXKG23LM,
    &kDefWXKG24LM,
    &kDefWXKG25LM,
    &kDefWXKG26LM,
    &kDefWXKG27LM,
    &kDefWXKG28LM,
    &kDefWXKG29LM,
    &kDefWXKG30LM,
    &kDefWXKG31LM,
    &kDefZNCLBL01LM,
    &kDefZNCLDJ11LM,
    &kDefZNCLDJ12LM,
    &kDefZNCLDJ14LM,
    &kDefZNCLDJ15LM,
    &kDefZNCZ02LM,
    &kDefZNCZ03LM,
    &kDefZNCZ04LM,
    &kDefZNCZ11LM,
    &kDefZNCZ12LM,
    &kDefZNCZ14LM,
    &kDefZNCZ15LM,
    &kDefZNLDP12LM,
    &kDefZNLDP13LM,
    &kDefZNLDP14LM,
    &kDefZNLDP16LM,
    &kDefZNLDP17LM,
    &kDefZNLDP18LM,
    &kDefZNLDP19LM,
    &kDefZNLDP20LM,
    &kDefZNLDP21LM,
    &kDefZNLDP22LM,
    &kDefZNLDP23LM,
    &kDefZNLDP24LM,
    &kDefZNLDP25LM,
    &kDefZNLDP26LM,
    &kDefZNLDP27LM,
    &kDefZNLDP28LM,
    &kDefZNLDP30LM,
    &kDefZNLDP31LM,
    &kDefZNMCPZ01LM,
    &kDefZNMCPZ02LM,
    &kDefZNMS11LM,
    &kDefZNMS12LM,
    &kDefZNMS13LM,
    &kDefZNQBKG11LM,
    &kDefZNQBKG12LM,
    &kDefZNQBKG13LM,
    &kDefZNQBKG14LM,
    &kDefZNQBKG15LM,
    &kDefZNQBKG16LM,
    &kDefZNQBKG17LM,
    &kDefZNQPKG01LM,
    &kDefZNWXKG01LM,
    &kDefZNXJKG01LM,
    &kDefZNXNKG01LM,
    &kDefZNXNKG02LM,
    &kDefZNXNKG03LM,
    &kDefZNXNKG04LM,
    &kDefZNXNKG05LM,
    &kDefKQJCMB11LM,
    &kDefJWSP001A,
    &kDefJWDL001A,
    &kDefLGYCDD01LM,
    &kDefWS_USC01,
    &kDefWS_USC02,
    &kDefQBKG27LM,
    &kDefQBKG28LM,
    &kDefQBKG26LM,
    &kDefQBKG17LM,
    &kDefQBKG18LM,
    &kDefQBKG19LM,
    &kDefQBKG20LM,
    &kDefFP1E,
    &kDefZNQBCZ11LM,
    &kDefJTQJ_BF_01LM_BW,
    &kDefZNGZDJ11LM,
    &kDefZNGZDJ16LM,
    &kDefZNCLDJ01LM,
    &kDefLLKZMK11LM,
    &kDefLLKZMK12LM,
    &kDefTDL01LM,
    &kDefZNTGMK12LM,
    &kDefHLQDQ01LM,
    &kDefZNDDQDQ11LM,
    &kDefZNDDQDQ12LM,
    &kDefZNDDQDQ13LM,
    &kDefSSWQD03LM,
    &kDefZNCJMB14LM,
    &kDefQBKG38LM,
    &kDefQBKG39LM,
    &kDefVOCKQJK11LM,
    &kDefQBCZ15LM,
    &kDefQBCZ14LM,
    &kDefWXKG17LM,
    &kDefSRTS_A01,
    &kDefZNCWWSQ01LM,
    &kDefZNQBKG38LM,
    &kDefZNQBKG39LM,
    &kDefZNQBKG40LM,
    &kDefZNQBKG41LM,
    &kDefZNQBKG42LM,
    &kDefZNQBKG43LM,
    &kDefZNQBKG44LM,
    &kDefZNQBKG45LM,
    &kDefWS_K01D,
    &kDefCL_L02D,
    &kDefVC_X01D,
    &kDefWS_K07E,
    &kDefT2_E27,
    &kDefT2_E27_CCT,
    &kDefWS_K08D,
    &kDefDS_K02D_DS_K02E,
    &kDefKD_R01D,
    &kDefWS_K05E,
    &kDefWS_K04E,
    &kDefWS_K02E,
    &kDefTH_S04D,
    &kDefWS_K03E,
    &kDefWT_A03E,
    &kDefUT_A01E,
    &kDefPS_S04D,
    &kDefC200,
    &kDefFP310,
    &kDefSSWQD22LM,
};

extern const std::size_t kLumiRegistryCount =
    sizeof(kLumiRegistry) / sizeof(kLumiRegistry[0]);

}  // namespace zhc::devices::lumi
