// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Hand-rewritten: this fingerprint (TS0601 / _TZE284_nthhgkd6, 4 channel dimmer module)
// is covered by the DP-correct port at Qa__TZE284_nthhgkd6.cpp which encodes all 14
// DPs (state_l1..l4, brightness_l1..l4, min_brightness_l1..l4, power_on_behavior,
// switch_type). The previous bundle emitted kFzBattery on a mains-powered dimmer plus
// generic on/off (0x0006) which this Tuya device doesn't speak. Registry no longer
// references kDef_QADZ4DIN.
namespace zhc::devices::qa {}  // namespace
