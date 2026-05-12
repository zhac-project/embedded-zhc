// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Konke-specific shared converters.
//
//   kFzKonkeAction       — genOnOff attr 0x0000 lookup 128/129/130 →
//                          single / double / hold. Used by 2AJZ4KPKEY
//                          multi-function button (z2m fz.konke_action).
//   kFzKonkeSceneAction  — genScenes commandRecall scenes 241/242/243/244 →
//                          hexagon / square / triangle / circle. Used by
//                          KK-TQ-J01W 4-key scene switch
//                          (z2m fzLocal.command_recall_konke).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::konke {

extern const ::zhc::FzConverter kFzKonkeAction;
extern const ::zhc::FzConverter kFzKonkeSceneAction;

}  // namespace zhc::devices::konke
