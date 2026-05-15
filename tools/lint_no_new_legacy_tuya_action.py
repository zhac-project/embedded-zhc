#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
#
# P4 — Legacy Tuya action-converter name guard.
#
# After P2 unified the three Tuya action decoders into one function with
# two configs (kFzTuyaActionFlat / kFzTuyaActionPerEp), the legacy names
#
#   kFzTuyaOnOffAction
#   kFzTuyaMultiAction
#   kFzTuyaButtonAction
#
# are kept as back-compat aliases so the ~70 auto-generated device files
# that still reference them keep compiling. New hand-written code and
# any regenerated device file should reach for the canonical names.
#
# This guard pins the legacy-name usage to its current baseline. Any
# NEW file using one of the legacy names, or any existing file whose
# usage COUNT goes up, fails the check. Reducing the count or removing
# a file is always allowed (the migration direction).
#
# Generator follow-up: private `zhac-tools` should emit the canonical
# names on next regen so the baseline drains naturally.
#
# Run from the embedded-zhc/ root (cwd-independent — uses script path
# to locate the definitions/ tree).

from __future__ import annotations

import os
import re
import sys

LEGACY = re.compile(r'\bkFzTuya(OnOffAction|MultiAction|ButtonAction)\b')
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
DEF_ROOT = os.path.normpath(os.path.join(SCRIPT_DIR, '..', 'definitions'))

# Definition / declaration sites for the legacy externs. These are
# allowed to reference the names — they ARE the names. Anywhere else
# is a usage that should migrate.
SHARED_EXEMPT = {
    'tuya/_shared.cpp',
    'tuya/_shared.hpp',
}

# Baseline captured 2026-05-15 (immediately after P3). Keyed by path
# relative to definitions/. Update when a file is migrated to canonical
# names: lower the count (or delete the entry).
BASELINE: dict[str, int] = {
    "moes/generated/Moe_ERS_10TZBVB_AA.cpp": 1,
    "moes/generated/Moe_ZT_B_EU1.cpp": 1,
    "moes/generated/Moe_ZT_B_EU2.cpp": 1,
    "moes/generated/Moe_ZT_B_EU3.cpp": 1,
    "moes/generated/Moes_ZG_101ZL.cpp": 1,
    "moes/generated/Moes__TZ3000_5e235jpa.cpp": 1,
    "moes/generated/Moes__TZ3000_axpdxqgu.cpp": 1,
    "moes/generated/Moes__TZ3000_egvb1p2g.cpp": 1,
    "moes/generated/Moes__TZ3000_gbm10jnj.cpp": 1,
    "moes/generated/Moes__TZ3000_ja5osu5g.cpp": 1,
    "moes/generated/Moes__TZ3000_kaflzta4.cpp": 1,
    "moes/generated/Moes__TZ3000_kjfzuycl.cpp": 1,
    "moes/generated/Moes__TZ3000_lrfvzq1e.cpp": 1,
    "tuya/TS0041.cpp": 2,
    "tuya/TS0042.cpp": 1,
    "tuya/TS0043.cpp": 1,
    "tuya/TS0044.cpp": 3,
    "tuya/TS0045.cpp": 1,
    "tuya/TS004A.cpp": 1,
    "tuya/TS004F.cpp": 2,
    "tuya/TS0211.cpp": 1,
    "tuya/TS0215A.cpp": 1,
    "tuya/TS0218.cpp": 1,
    "tuya/TS0219.cpp": 1,
    "tuya/generated/Gen_CK_BL702_AL_01_7008_Z102LG01_1_.cpp": 1,
    "tuya/generated/Gen_TS0006.cpp": 1,
    "tuya/generated/Gen_TS0026.cpp": 1,
    "tuya/generated/Gen_TS0041A.cpp": 1,
    "tuya/generated/Gen_TS0046.cpp": 1,
    "tuya/generated/Gen_TS0049.cpp": 1,
    "tuya/generated/Gen_TS0052.cpp": 1,
    "tuya/generated/Gen_TS0105.cpp": 1,
    "tuya/generated/Gen_TS0115.cpp": 1,
    "tuya/generated/Gen_TS0207.cpp": 1,
    "tuya/generated/Gen_TS020C.cpp": 1,
    "tuya/generated/Gen_TS0216.cpp": 1,
    "tuya/generated/Gen_TS0224.cpp": 1,
    "tuya/generated/Gen_TS0301.cpp": 1,
    "tuya/generated/Gen_TS0505.cpp": 1,
    "tuya/generated/Gen_TS0901.cpp": 1,
    "tuya/generated/Gen_ZG_101Z.cpp": 1,
    "tuya/generated/Gen_ZG_101ZD.cpp": 1,
    "tuya/generated/Gen_ZG_102Z.cpp": 1,
    "tuya/generated/Gen_ZG_102ZL.cpp": 1,
    "tuya/generated/Gen_ZG_102ZM.cpp": 1,
    "tuya/generated/Gen_ZG_103Z.cpp": 1,
    "tuya/generated/Gen_ZG_204ZE.cpp": 1,
    "tuya/generated/Gen_ZG_204ZH.cpp": 1,
    "tuya/generated/Gen_ZG_204ZK.cpp": 1,
    "tuya/generated/Gen_ZG_204ZL.cpp": 1,
    "tuya/generated/Gen_ZG_204ZM.cpp": 1,
    "tuya/generated/Gen_ZG_204ZQ.cpp": 1,
    "tuya/generated/Gen_ZG_204ZV.cpp": 1,
    "tuya/generated/Gen_ZG_204ZX.cpp": 1,
    "tuya/generated/Gen_ZG_205ZL.cpp": 1,
    "tuya/generated/Gen_ZG_222Z.cpp": 1,
    "tuya/generated/Gen_ZG_223Z.cpp": 1,
    "tuya/generated/Gen_ZG_225Z.cpp": 1,
    "tuya/generated/Gen_ZG_226Z.cpp": 1,
    "tuya/generated/Gen_ZG_227ZL.cpp": 1,
    "tuya/generated/Gen_ZG_228Z.cpp": 1,
    "tuya/generated/Gen_ZG_229Z.cpp": 1,
    "tuya/generated/Gen_ZG_301Z.cpp": 1,
    "tuya/generated/Gen_ZG_302Z1.cpp": 1,
    "tuya/generated/Gen_ZG_302Z2.cpp": 1,
    "tuya/generated/Gen_ZG_302Z3.cpp": 1,
    "tuya/generated/Gen_ZG_302ZL.cpp": 1,
    "tuya/generated/Gen_ZG_302ZM.cpp": 1,
    "tuya/generated/Gen_ZG_305Z.cpp": 1,
    "tuya/generated/Gen__TZ3000_abrsvsou.cpp": 1,
}


def scan() -> dict[str, int]:
    counts: dict[str, int] = {}
    for dirpath, _, filenames in os.walk(DEF_ROOT):
        for fn in filenames:
            if not fn.endswith(('.cpp', '.hpp')):
                continue
            full = os.path.join(dirpath, fn)
            rel = os.path.relpath(full, DEF_ROOT)
            if rel in SHARED_EXEMPT:
                continue
            with open(full, encoding='utf-8') as fh:
                n = sum(1 for line in fh for _ in LEGACY.finditer(line))
            if n:
                counts[rel] = n
    return counts


def main() -> int:
    current = scan()
    failures: list[str] = []

    for path, count in sorted(current.items()):
        baseline = BASELINE.get(path)
        if baseline is None:
            failures.append(
                f'{path}: new legacy use ({count}). Prefer '
                f'kFzTuyaActionFlat / kFzTuyaActionPerEp.')
        elif count > baseline:
            failures.append(
                f'{path}: legacy uses {baseline} → {count}. '
                f'Migration direction is downward only.')

    # Informational: report files that dropped off the baseline (good).
    cleaned = sorted(set(BASELINE) - set(current))
    if cleaned:
        sys.stderr.write(
            'OK: legacy uses cleared from '
            f'{len(cleaned)} file(s); update BASELINE to drop them.\n')

    if failures:
        sys.stderr.write('Legacy Tuya action-name guard failed:\n')
        for line in failures:
            sys.stderr.write(f'  {line}\n')
        sys.stderr.write(
            '\nCanonical names live in definitions/tuya/_shared.hpp.\n')
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
