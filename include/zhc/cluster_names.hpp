// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Canonical ZCL cluster-id → z2m cluster-name table. Both the ESP-IDF
// adapter and the host fixture runner must stay in sync here; when they
// drift, FzConverter selectors (.cluster = "foo") silently stop
// matching for any cluster the decoder can't label. The Mi-Cube rotate
// regression (missing 0x000C / genAnalogInput) was caused by exactly
// that split.
//
// Add new clusters in numeric order. Returning `nullptr` for an unknown
// id is intentional — callers then leave `msg.cluster` null, and
// dispatch falls through via `cluster_match`'s null-is-any rule.

#include <cstdint>

namespace zhc {

inline const char* cluster_id_to_name(std::uint16_t cluster_id) {
    switch (cluster_id) {
        // General / ZCL foundation
        case 0x0000: return "genBasic";
        case 0x0001: return "genPowerCfg";
        case 0x0002: return "genDeviceTempCfg";
        case 0x0003: return "genIdentify";
        case 0x0004: return "genGroups";
        case 0x0005: return "genScenes";
        case 0x0006: return "genOnOff";
        case 0x0008: return "genLevelCtrl";
        case 0x000A: return "genTime";
        case 0x000C: return "genAnalogInput";
        case 0x000D: return "genAnalogOutput";
        case 0x000F: return "genBinaryInput";
        case 0x0010: return "genBinaryOutput";
        case 0x0012: return "genMultistateInput";
        case 0x0013: return "genMultistateOutput";
        case 0x0019: return "genOta";

        // Closures
        case 0x0101: return "closuresDoorLock";
        case 0x0102: return "closuresWindowCovering";

        // HVAC
        case 0x0201: return "hvacThermostat";
        case 0x0202: return "hvacFanCtrl";
        case 0x0204: return "hvacUserInterfaceCfg";

        // Lighting
        case 0x0300: return "lightingColorCtrl";

        // Measurement & sensing
        case 0x0400: return "msIlluminanceMeasurement";
        case 0x0402: return "msTemperatureMeasurement";
        case 0x0403: return "msPressureMeasurement";
        case 0x0405: return "msRelativeHumidity";
        case 0x0406: return "msOccupancySensing";
        case 0x0408: return "msSoilMoisture";
        // 0x0409 (pHMeasurement) + 0x040A — abused by BYUN M415 gas/smoke
        // sensors as the alarm-CLEAR channel. z2m byun.ts decodes
        // `byun_smoke_false` from pHMeasurement measuredValue==0 and
        // `byun_gas_false` from the raw 0x040A frame (data[0]==0x18). Named
        // here so the vendor Fz selectors (definitions/byun/_shared.cpp)
        // match by cluster string.
        case 0x0409: return "pHMeasurement";
        case 0x040A: return "byunGasRaw";  // BYUN M415-5C gas-clear raw cluster (1034)
        case 0x040D: return "msCO2";

        // Security & safety
        case 0x0500: return "ssIasZone";
        case 0x0501: return "ssIasAce";
        case 0x0502: return "ssIasWd";

        // Smart energy
        case 0x0702: return "seMetering";

        // Home automation
        case 0x0B01: return "haMeterIdentification";
        case 0x0B04: return "haElectricalMeasurement";
        case 0x0B05: return "haDiagnostic";

        // Vendor-specific
        // 0xE001 is upstream herdsman's "manuSpecificTuya3"
        // (attributes: powerOnBehavior 0xD010, switchMode 0xD020,
        //  switchType 0xD030). The "Tuya2" string is a historical
        // misnomer in ZHC kept for binary-compat with the linptech
        // port. Upstream "Tuya2" sits at 0xE002 and is not currently
        // mapped here.
        case 0xE001: return "manuSpecificTuya2";  // (upstream: manuSpecificTuya3)
        case 0xE004: return "zosungIRControl";    // Tuya/Moes UFO-R11 — IR learn/control
        case 0xED00: return "zosungIRTransmit";   // Tuya/Moes UFO-R11 — chunked IR file transfer
        case 0xEF00: return "manuSpecificTuya";
        case 0xFC00: return "manuSpecificPhilips";
        // 0xFC11 (64529) — Woolley BSD29/BSD59 smart-plug metering cluster.
        // z2m decodes it as the raw decimal cluster "64529" with attrs
        // 0x7006/0x7005/0x7004 (power/voltage/current, each /1000). Named
        // here so the vendor Fz selector matches by cluster string.
        case 0xFC11: return "manuSpecificWoolley";
        case 0xFCC0: return "manuSpecificLumi";
        case 0xFCCC: return "manuSpecificClusterAduroSmart";  // Terncy / AduroSmart vendor cluster
        case 0xFF01: return "manuSpecificLumiFF01";
        case 0xFF02: return "manuSpecificLumiFF02";
        // 0xFE05 (65029) — J.XUAN WSZ01 wireless-switch button cluster.
        // z2m's fz.WSZ01_on_off_action reads attr id 1 (decimal key "1")
        // from this manuSpecific cluster's attributeReport and maps
        // 0..3 → release/single/double/hold into `action`. Named here so
        // the vendor Fz selector (kFzWsz01Action) matches by cluster string.
        case 0xFE05: return "manuSpecificJxuan";

        default:     return nullptr;
    }
}

}  // namespace zhc
