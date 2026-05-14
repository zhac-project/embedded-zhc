// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 1: Aqara DWZTCGQ11LM — Multi-state sensor P100. Match-only stub.
//
// z2m-source: lumi.ts #DWZTCGQ11LM. Full z2m extend stack:
//   - quirkCheckinInterval(1h)
//   - addManuSpecificLumiCluster()
//   - lumiPreventReset()
//   - lumiBattery({voltageAttribute: 0x17, percentageAttribute: 0x18})
//   - lumiZigbeeOTA()
//   - 10+ enumLookup/numeric/binary writes on manuSpecificLumi (0xFCC0):
//       0x0116 device_mode (door_window=3, object=5)
//       0x01eb door_window_type
//       0x010c sensitivity (1..10)
//       0x01ec report_interval (s)
//       0x01f0 orientation_detection
//       0x01ed movement_detection
//       0x01d8 fall_detection
//       0x0107 vibration_detection
//       0x01ef triple_tap_detection
//       … (more — see z2m source for the full list)
//
// This stub matches the zigbeeModel so the device is recognised, and
// uses kFzLumiBasic to decode battery + identity via the standard Lumi
// MI-struct path. Config-attribute Tz wiring is a follow-up — needs
// per-attribute kTz* converters on the lumi manu-spec cluster, plus
// an exposes list for the SPA's Options tab.

#include "definitions/lumi/_shared.hpp"
#include "zhc/runtime/definition.hpp"

namespace zhc::devices::lumi {

namespace {

const char* const kZigbeeModels[] = { "lumi.vibration.agl002" };

const FzConverter* const kFromZigbee[] = {
    &zhc::lumi::kFzLumiBasic,
};

}  // namespace

extern const PreparedDefinition kDefDWZTCGQ11LM{
    .zigbee_models       = kZigbeeModels,
    .zigbee_models_count = sizeof(kZigbeeModels) / sizeof(kZigbeeModels[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model  = "DWZTCGQ11LM",
    .vendor = "Aqara",
    .meta = nullptr,
    .exposes = nullptr,
    .exposes_count = 0,
    .white_labels = nullptr,
    .white_labels_count = 0,
    .from_zigbee = kFromZigbee,
    .from_zigbee_count = sizeof(kFromZigbee) / sizeof(kFromZigbee[0]),
    .to_zigbee = nullptr,
    .to_zigbee_count = 0,
    .configure = nullptr,
    .on_event = nullptr,
};

}  // namespace zhc::devices::lumi
