#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
"""Guard: every definition object must be constant-initialised.

Why this exists
---------------
A PreparedDefinition whose initialiser reads a value the compiler cannot see
-- typically a bundle count declared `extern const` in a shared header and
defined in another .cpp -- gets a static constructor. On the ESP32 that moves
the whole object out of flash .rodata into internal RAM .data. 1,093
definitions did that on the S31 (~124 KB of its tightest resource, plus
~1,090 constructors run at boot).

Fix: make the value visible to the compiler, e.g. `inline constexpr` in the
shared header with a `static_assert` next to the array it counts (see
definitions/philips/_shared.{hpp,cpp}).

    python3 tools/check_defs_const_init.py <objdump> <object-list-file>

<object-list-file> lists the zhc_definitions objects, one per line (CMake
writes it). Fails when any of them
  * defines a static constructor (`_GLOBAL__sub_I_*`), or
  * places a kDef* object in a writable section (.data / .bss; the read-only
    .data.rel.ro that PIE hosts use for pointer-carrying constants is fine).
"""
import re
import subprocess
import sys

# addr, 7 flag chars, section, size/alignment, [.hidden] name
_SYM = re.compile(r"^[0-9a-f]+ (.{7}) (\S+)\s+[0-9a-f]+\s+(.+)$")
_WRITABLE = re.compile(r"^\.(s?data|s?bss|tdata|tbss)(?!\.rel\.ro)")


def main() -> int:
    objdump, list_file = sys.argv[1], sys.argv[2]
    objs = [line.strip() for line in open(list_file) if line.strip()]
    problems, kdefs = [], 0
    for i in range(0, len(objs), 200):
        out = subprocess.run([objdump, "-t", *objs[i:i + 200]], check=True,
                             capture_output=True, text=True).stdout
        obj = "?"
        for line in out.splitlines():
            if ":     file format " in line:
                obj = line.split(":     file format ")[0]
                continue
            m = _SYM.match(line)
            if not m:
                continue
            flags, section, name = m.group(1), m.group(2), m.group(3).split()[-1]
            if "_GLOBAL__sub_I_" in name:
                problems.append(f"{obj}: static constructor {name}")
            elif "O" in flags and "kDef" in name and not name.startswith("__odr_asan"):
                kdefs += 1
                if _WRITABLE.match(section):
                    problems.append(f"{obj}: {name} in writable {section}")

    print(f"definition objects scanned: {len(objs)}, kDef objects: {kdefs}")
    if kdefs == 0:
        print("FAILED: no kDef objects found -- wrong object list?")
        return 1
    if not problems:
        print("OK: every definition is constant-initialised.")
        return 0
    print(f"\nConstant-init guard FAILED ({len(problems)}):")
    for p in problems[:50]:
        print("  " + p)
    print("\nFix: make whatever the initialiser reads visible at compile time, "
          "e.g. an `inline constexpr` count in the shared header "
          "(see this script's docstring).")
    return 1


if __name__ == "__main__":
    sys.exit(main())
