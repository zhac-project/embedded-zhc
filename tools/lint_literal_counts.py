#!/usr/bin/env python3
"""Guard: a definition's `_count` field must match its array's real length.

Why this exists
---------------
Definitions pair an array with a count:

    .manufacturer_names = kN, .manufacturer_names_count = sizeof(kN)/sizeof(kN[0])

Many auto-generated definitions instead hardcode the count as a literal. That
is correct the day it is written and a trap ever after:

  * count too SMALL — appending a fingerprint is silently ignored, so the
    device stays unrecognised and the diff that prompted the append looks done.
    Hit on `TS0601_6gang_switch` (v26.95 window) and again on
    `TS0601_power_monitoring_switch` (v26.101 window).
  * count too LARGE — every lookup that misses walks past the end of the array.
    Hit on two Mazda TRV definitions (v26.99 window), which declared 24
    datapoints for a 23-row table.

Three consecutive parity windows tripped over this, so it is now a test.

The check compares each literal count against the element count of the array it
names. Counts written as `sizeof(x)/sizeof(x[0])` are self-maintaining and are
skipped — the point of the guard is that converting a literal to that form is
always a valid fix.

    python3 tools/lint_literal_counts.py [definitions_dir]

Exit 0 when every literal count agrees with its array.
"""
import glob
import os
import re
import sys

# `.<array> = <name>, .<array>_count = <literal>`
_PAIRS = (
    "zigbee_models",
    "manufacturer_names",
    "exposes",
    "white_labels",
    "bindings",
    "reports",
    "from_zigbee",
    "to_zigbee",
)
# `TuyaDatapointMap kMap{ kEntries, <literal> }`
_DP_MAP = re.compile(r"TuyaDatapointMap\s+\w+\s*\{\s*(\w+)\s*,\s*(\d+)\s*\}")


def strip_comments(src: str) -> str:
    src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
    return re.sub(r"//[^\n]*", "", src)


def array_body(src: str, name: str):
    """Text between the braces of `name[] = { ... }`, or None."""
    m = re.search(r"\b" + re.escape(name) + r"\s*\[\s*\]\s*=\s*\{", src)
    if not m:
        return None
    i, depth, start, instr = m.end(), 1, m.end(), False
    while i < len(src) and depth:
        c = src[i]
        if instr:
            if c == "\\":
                i += 2
                continue
            if c == '"':
                instr = False
        elif c == '"':
            instr = True
        elif c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        i += 1
    return src[start:i - 1]


def count_elements(body: str) -> int:
    """Top-level element count: commas at brace/paren depth 0, string-aware."""
    depth = n = i = 0
    seen = instr = False
    while i < len(body):
        c = body[i]
        if instr:
            if c == "\\":
                i += 2
                continue
            if c == '"':
                instr = False
            i += 1
            continue
        if c == '"':
            instr, seen = True, True
        elif c in "{([":
            depth += 1
            seen = True
        elif c in ")]}":
            depth -= 1
        elif c == "," and depth == 0:
            n += 1
            seen = False
        elif not c.isspace():
            seen = True
        i += 1
    return n + (1 if seen else 0)


def scan(defs_dir: str):
    problems, checked = [], 0
    for path in sorted(glob.glob(os.path.join(defs_dir, "**", "*.cpp"), recursive=True)):
        src = strip_comments(open(path, encoding="utf-8", errors="ignore").read())
        rel = os.path.relpath(path, defs_dir).replace(os.sep, "/")

        for field in _PAIRS:
            pattern = (r"\." + field + r"\s*=\s*(\w+)\s*,\s*\." + field +
                       r"_count\s*=\s*(\d+)\s*,")
            for m in re.finditer(pattern, src):
                arr, declared = m.group(1), int(m.group(2))
                if arr == "nullptr" or declared == 0:
                    continue
                body = array_body(src, arr)
                if body is None:
                    continue          # array lives in a header — not our call
                checked += 1
                actual = count_elements(body)
                if actual != declared:
                    problems.append((rel, f".{field}_count", arr, declared, actual))

        for m in _DP_MAP.finditer(src):
            arr, declared = m.group(1), int(m.group(2))
            body = array_body(src, arr)
            if body is None:
                continue
            checked += 1
            actual = count_elements(body)
            if actual != declared:
                problems.append((rel, "TuyaDatapointMap", arr, declared, actual))

    return problems, checked


def main() -> int:
    defs_dir = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "definitions")
    problems, checked = scan(defs_dir)
    print(f"literal counts checked: {checked}")
    if not problems:
        print("OK: every literal count matches its array.")
        return 0
    print("\nLiteral count guard FAILED:")
    for rel, field, arr, declared, actual in problems:
        verdict = ("entries past the declared count are IGNORED"
                   if actual > declared else
                   "reads PAST THE END of the array")
        print(f"  {rel}: {field} on `{arr}` says {declared}, array has {actual}"
              f" — {verdict}.")
    print("\nFix: replace the literal with sizeof(x)/sizeof(x[0]).")
    return 1


if __name__ == "__main__":
    sys.exit(main())
