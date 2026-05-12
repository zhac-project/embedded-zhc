// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Hand-rewritten: this fingerprint (TS011F / _TZ3218_kwht8j5m, power sensor) is
// covered by the DP-correct port at Qa__TZ3218_kwht8j5m.cpp which encodes the Tuya
// DPs 17 (energy/1000), 18 (current/1000), 19 (power/10), 20 (voltage/10), 101
// (reactive_power_threshold), 102 (max_effective_power), 104 (status_report), 105
// (switch_status). The previous bundle wired genMetering (0x0702) and
// haElectricalMeasurement (0x0B04) — clusters this device does not expose; it speaks
// only manuSpecificTuya (0xEF00) plus the Tuya MCU sync-time call. Registry no longer
// references kDef_QASZP.
namespace zhc::devices::qa {}  // namespace
