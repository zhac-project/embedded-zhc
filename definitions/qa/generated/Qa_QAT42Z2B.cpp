// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Hand-rewritten: this fingerprint (TS0601 / _TZE284_1aqlsquf, 2 channel scene switch)
// is covered by the DP-correct port at Qa__TZE284_1aqlsquf.cpp. The previous bundle
// emitted generic on/off (0x0006) which this Tuya device doesn't speak — it speaks
// 0xEF00 with DPs 24/25 (state_l1/l2), 5/6 (action), 101 (backlight_brightness).
// Registry no longer references kDef_QAT42Z2B.
namespace zhc::devices::qa {}  // namespace
