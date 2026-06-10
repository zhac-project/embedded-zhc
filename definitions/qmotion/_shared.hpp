// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Shared Qmotion vendor converters.
//
// Qmotion's QZR-ZIG2400 "5 channel remote" reports an `identify` action
// from the genIdentify cluster (z2m `fz.identify`). There is no generic
// genIdentify decoder in the shared library, so the vendor-local
// `kFzIdentifyAction` mirrors z2m 1:1: any genIdentify (0x0003)
// attributeReport / readResponse → `action = "identify"`.
#pragma once

#include "definitions/_generic/_shared.hpp"

namespace zhc::qmotion {

// z2m-source: fromZigbee.ts `fz.identify`
//   cluster: genIdentify, type: [attributeReport, readResponse]
//   convert: () => ({action: "identify"})
extern const ::zhc::FzConverter kFzIdentifyAction;

}  // namespace zhc::qmotion
