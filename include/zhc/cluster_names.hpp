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
        case 0xFCC0: return "manuSpecificLumi";
        case 0xFCCC: return "manuSpecificClusterAduroSmart";  // Terncy / AduroSmart vendor cluster
        case 0xFF01: return "manuSpecificLumiFF01";
        case 0xFF02: return "manuSpecificLumiFF02";

        default:     return nullptr;
    }
}

}  // namespace zhc
