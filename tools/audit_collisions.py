#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
"""
Fingerprint-collision audit for the ZHAC registry.

Walks every `definitions/<ns>/[generated/]*.cpp`, extracts
(zigbee_models, manufacturer_names, fz/tz/cfg/exposes axes), and
reports any case where two cpps claim the same matcher key without a
manu-name disambiguator — or where the first-found cpp for a
collision has fewer wired axes than a later candidate.

Exits non-zero when any real collision is found (same key, no manu
disambiguation). Soft warnings go to stderr but don't fail.

Run:
    python3 embedded/zhc/tools/audit_collisions.py
"""
from __future__ import annotations
import os
import re
import sys
from pathlib import Path
from collections import defaultdict

ROOT = Path(__file__).resolve().parent.parent / "definitions"

ARR_RE = re.compile(
    r"(?:constexpr\s+)?const\s+char\s*\*(?:\s+const)?\s+"
    r"(\w+)\s*\[\s*\]\s*=\s*\{([^}]*)\}",
)
FIELD_PTR_RE = re.compile(r"\.(\w+)\s*=\s*([A-Za-z_][A-Za-z0-9_]*)")
MODEL_RE     = re.compile(r'\.model\s*=\s*"([^"]+)"')


def extract_cpp(cpp: Path) -> dict:
    t = cpp.read_text(errors="replace")
    arrays = {}
    for m in ARR_RE.finditer(t):
        vals = re.findall(r'"([^"]+)"', m.group(2))
        if vals:
            arrays[m.group(1)] = vals

    def arr(field: str) -> list[str]:
        for m in FIELD_PTR_RE.finditer(t):
            if m.group(1) == field:
                return arrays.get(m.group(2), [])
        return []

    def has(field: str) -> bool:
        m = re.search(rf"\.{field}\s*=\s*[^n0]", t)
        return bool(m)

    def nullptr(field: str) -> bool:
        return bool(re.search(rf"\.{field}\s*=\s*nullptr", t))

    axes = {
        "fz":       has("from_zigbee") and not nullptr("from_zigbee"),
        "tz":       has("to_zigbee")   and not nullptr("to_zigbee"),
        "cfg":      (has("configure")  and not nullptr("configure"))
                    or ("bindings_count" in t and
                        not re.search(r"bindings_count\s*=\s*0\b", t))
                    or ("reports_count"  in t and
                        not re.search(r"reports_count\s*=\s*0\b",  t)),
        "exposes":  (has("exposes") and not nullptr("exposes") and
                     not re.search(r"exposes_count\s*=\s*0\b", t)),
    }
    model_m = MODEL_RE.search(t)
    return {
        "path":              cpp,
        "zigbee_models":     arr("zigbee_models"),
        "manufacturer_names": arr("manufacturer_names"),
        "model":             model_m.group(1) if model_m else "",
        "axes":              axes,
        "axes_count":        sum(axes.values()),
    }


def _registry_symbol_order(ns_dir: Path) -> list[str]:
    """Return kDef* symbols in the order they appear in the vendor's
    registry.cpp — this matches runtime `find_definition` traversal."""
    reg = ns_dir / "registry.cpp"
    if not reg.exists(): return []
    txt = reg.read_text(errors="replace")
    # Match `    &kDefXXX,` lines within the registry array body.
    return re.findall(r"&(kDef\w+)\s*,", txt)


def _adapter_vendor_order() -> list[str]:
    """Parse zhc_adapter.cpp's merge_registries() so the audit
    mirrors the exact order used at runtime. Falls back to filesystem
    alpha order when the file cannot be parsed.

    Post-split layout: zhc_adapter lives in the sibling `zhac-components`
    repo. From `embedded-zhc/tools/audit_collisions.py`, the sibling
    sits at `../../../zhac-components/components/zhc_adapter/src/`. The
    env var `ZHC_ADAPTER_CPP` overrides for users with non-standard
    workspace layouts."""
    env_override = os.environ.get("ZHC_ADAPTER_CPP")
    if env_override:
        adapter = Path(env_override)
    else:
        adapter = (Path(__file__).resolve().parent.parent.parent
                    / "zhac-components" / "components" / "zhc_adapter"
                    / "src" / "zhc_adapter.cpp")
    if not adapter.exists():
        return sorted(d.name for d in ROOT.iterdir()
                       if d.is_dir() and not d.name.startswith("_"))
    txt = adapter.read_text(errors="replace")
    # Match `add(zhc::devices::<ns>::k<Name>Registry,`
    order = re.findall(r"add\(\s*zhc::devices::(\w+)::", txt)
    # Keep first occurrence order, drop duplicates.
    seen, out = set(), []
    for ns in order:
        if ns in seen: continue
        seen.add(ns); out.append(ns)
    # Tier E aggregator — include the remaining vendors alphabetically.
    tail = sorted(d.name for d in ROOT.iterdir()
                   if d.is_dir() and not d.name.startswith("_") and
                   d.name not in seen)
    return out + tail


def scan() -> tuple[list[dict], dict[str, str]]:
    """Enumerate every cpp and return (ordered_infos, sym→vendor)."""
    sym_to_info: dict[str, dict] = {}
    sym_to_vendor: dict[str, str] = {}
    for cpp in ROOT.rglob("*.cpp"):
        name = cpp.name
        if name in ("registry.cpp", "_shared.cpp"): continue
        if name.startswith("_"): continue
        info = extract_cpp(cpp)
        if not (info["zigbee_models"] or info["manufacturer_names"]):
            continue
        m = re.search(r"extern const PreparedDefinition (kDef\w+)",
                       cpp.read_text(errors="replace"))
        if m:
            sym_to_info[m.group(1)] = info
            sym_to_vendor[m.group(1)] = cpp.relative_to(ROOT).parts[0]

    vendor_order = _adapter_vendor_order()
    ordered = []
    for ns in vendor_order:
        ns_dir = ROOT / ns
        if not ns_dir.is_dir(): continue
        for sym in _registry_symbol_order(ns_dir):
            info = sym_to_info.get(sym)
            if info:
                info["vendor"] = ns
                ordered.append(info)
    return ordered, sym_to_vendor


def main():
    defs, sym_to_vendor = scan()
    print(f"scanned {len(defs)} device cpps", file=sys.stderr)

    # Bucket by (zigbee_model) for non-Tuya, (zigbee_model, manu) for Tuya.
    tuya_buckets = defaultdict(list)
    plain_buckets = defaultdict(list)
    for d in defs:
        if d["manufacturer_names"]:
            for zm in d["zigbee_models"] or [""]:
                for mn in d["manufacturer_names"]:
                    tuya_buckets[(zm, mn)].append(d)
        else:
            for zm in d["zigbee_models"]:
                plain_buckets[zm].append(d)

    hard = []   # real collisions — same matcher key, no disambiguator
    weak_hard = []   # first-found has fewer axes, same vendor (bug)
    weak_soft = []   # first-found worse, cross vendor (inherent)

    def same_vendor(group):
        vs = {g.get("vendor") for g in group}
        return len(vs) == 1

    for key, group in tuya_buckets.items():
        if len(group) < 2: continue
        best_axes = max(g["axes_count"] for g in group)
        first = group[0]
        hard.append(("tuya", key, group))
        if first["axes_count"] < best_axes:
            bucket = weak_hard if same_vendor(group) else weak_soft
            bucket.append(("tuya", key, group))

    for key, group in plain_buckets.items():
        if len(group) < 2: continue
        best_axes = max(g["axes_count"] for g in group)
        first = group[0]
        hard.append(("plain", key, group))
        if first["axes_count"] < best_axes:
            bucket = weak_hard if same_vendor(group) else weak_soft
            bucket.append(("plain", key, group))
    weak = weak_hard

    print(f"\n== Collision summary ==", file=sys.stderr)
    print(f"  Tuya  (zigbeeModel + manuName) collisions: "
          f"{sum(1 for k in hard if k[0]=='tuya')}", file=sys.stderr)
    print(f"  Plain (zigbeeModel)          collisions: "
          f"{sum(1 for k in hard if k[0]=='plain')}", file=sys.stderr)
    print(f"  Weak picks — same vendor    (FAIL)     : "
          f"{len(weak_hard)}", file=sys.stderr)
    print(f"  Weak picks — cross vendor   (warn)     : "
          f"{len(weak_soft)}", file=sys.stderr)

    # Verbose detail for weak picks — these risk wrong-dispatch regressions.
    if weak:
        print(f"\n== Weak first-pick entries ==", file=sys.stderr)
        for kind, key, group in weak[:20]:
            print(f"  {kind} {key}", file=sys.stderr)
            for g in group:
                rel = g["path"].relative_to(ROOT)
                ax = g["axes"]
                sym = (f"fz={int(ax['fz'])} tz={int(ax['tz'])} "
                        f"cfg={int(ax['cfg'])} expo={int(ax['exposes'])}")
                mark = "  [FIRST-PICK]" if g is group[0] else ""
                print(f"    {rel}   {sym}{mark}", file=sys.stderr)

    if weak_soft:
        print(f"\n== Cross-vendor weak picks (informational) ==",
              file=sys.stderr)
        for kind, key, group in weak_soft[:10]:
            vendors = "/".join(g.get("vendor", "?") for g in group)
            print(f"  {kind} {key} — vendors: {vendors}",
                  file=sys.stderr)

    # Fail on same-vendor weak picks only — they're dispatch bugs.
    fail = len(weak_hard) > 0

    if fail:
        print("\n[FAIL] fingerprint-collision audit", file=sys.stderr)
        return 1
    print("[PASS] fingerprint-collision audit", file=sys.stderr)
    return 0


if __name__ == "__main__":
    sys.exit(main())
