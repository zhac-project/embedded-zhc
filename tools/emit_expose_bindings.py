#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
"""
Infer Expose[] + BindingSpec[] arrays from each Lumi device cpp's
wired FzConverters and patch the cpp in place. Idempotent — already
populated fields are left alone.

Mapping:
  kFzLumiBasic                 → battery, voltage, device_temperature,
                                 power_outage_count (read-only Numeric)
                                 + binding genBasic
  kFzBattery                   → battery + voltage + bind genPowerCfg
  kFzOnOff / generic::kFzOnOff → state (Binary, StateSet) + bind genOnOff
  kFzBrightness                → brightness + bind genLevelCtrl
  kFzColorTemperature          → color_temp + bind lightingColorCtrl
  kFzOccupancy / kFzLumiOccupancy
                               → occupancy + bind msOccupancySensing
  kFzLumiWaterLeak             → water_leak + bind ssIasZone
  kFzLumiSmoke                 → smoke      + bind ssIasZone
  kFzLumiGas                   → gas        + bind ssIasZone
  kFzLumiCurtainPosition       → position   + bind genAnalogOutput
  kFzLumiPowerAnalog           → power      + bind genAnalogInput
  kFzLumiPower                 → power/voltage/current
                                 + bind haElectricalMeasurement
  kFzLumiElectricityMeter      → energy     + bind seMetering
  kFzLumiActionMultistate / kFzLumiAction / kFzLumiActionOpple /
  kFzLumiCubeMultistate / kFzLumiCTPR01Multistate /
  kFzLumiCubeAnalog            → action (StateSet)
                                 + bind genMultistateInput (or genOnOff
                                 for lumi_action)

Devices that already declare `.exposes = ` / `.bindings = ` non-null
are left untouched. Run:
    python3 tools/emit_expose_bindings.py [lumi|tuya|moes]
"""

from __future__ import annotations
import os
import re
import sys
from pathlib import Path

# Resolve `definitions/` relative to this script's location so the tool
# tracks the repo it lives in. `ZHC_DEFINITIONS_DIR` overrides for
# non-standard workspace layouts.
ROOT = Path(os.environ.get(
    "ZHC_DEFINITIONS_DIR",
    Path(__file__).resolve().parent.parent / "definitions",
))

# One FzConverter → list of (key, type, access, unit) Expose tuples
# and list of (endpoint, cluster_id) BindingSpec tuples.
EXP_BINARY = "ExposeType::Binary"
EXP_NUMERIC = "ExposeType::Numeric"
ACCESS_STATE = "Access::State"
ACCESS_STATESET = "Access::StateSet"

EXPOSES_FROM_FZ = {
    "kFzLumiBasic": [
        ("battery",            EXP_NUMERIC, ACCESS_STATE, '"%"'),
        ("voltage",            EXP_NUMERIC, ACCESS_STATE, '"mV"'),
        ("device_temperature", EXP_NUMERIC, ACCESS_STATE, '"C"'),
        ("power_outage_count", EXP_NUMERIC, ACCESS_STATE, "nullptr"),
    ],
    "kFzBattery": [
        ("battery", EXP_NUMERIC, ACCESS_STATE, '"%"'),
        ("voltage", EXP_NUMERIC, ACCESS_STATE, '"mV"'),
    ],
    "kFzOnOff": [("state", EXP_BINARY, ACCESS_STATESET, "nullptr")],
    "kFzBrightness": [
        ("brightness", EXP_NUMERIC, ACCESS_STATESET, "nullptr"),
    ],
    "kFzColorTemperature": [
        ("color_temp", EXP_NUMERIC, ACCESS_STATESET, '"mired"'),
    ],
    "kFzOccupancy": [
        ("occupancy", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiOccupancy": [
        ("occupancy", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiWaterLeak": [
        ("water_leak", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiSmoke": [
        ("smoke", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiGas": [
        ("gas", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiCurtainPosition": [
        ("position", EXP_NUMERIC, ACCESS_STATESET, '"%"'),
    ],
    "kFzLumiPowerAnalog": [
        ("power", EXP_NUMERIC, ACCESS_STATE, '"W"'),
    ],
    "kFzLumiPower": [
        ("power",   EXP_NUMERIC, ACCESS_STATE, '"W"'),
        ("voltage", EXP_NUMERIC, ACCESS_STATE, '"V"'),
        ("current", EXP_NUMERIC, ACCESS_STATE, '"A"'),
    ],
    "kFzLumiElectricityMeter": [
        ("energy", EXP_NUMERIC, ACCESS_STATE, '"kWh"'),
    ],
    "kFzLumiActionMultistate": [
        ("action", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiAction": [
        ("action", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzLumiCubeMultistate":   [("action", EXP_BINARY, ACCESS_STATE, "nullptr")],
    "kFzLumiCubeAnalog":       [("action", EXP_BINARY, ACCESS_STATE, "nullptr")],
    "kFzLumiCTPR01Multistate": [("action", EXP_BINARY, ACCESS_STATE, "nullptr")],
    "kFzLumiManuMotion":       [
        ("motion_sensitivity", EXP_BINARY, ACCESS_STATESET, "nullptr"),
        ("detection_period",   EXP_NUMERIC, ACCESS_STATESET, '"s"'),
        ("trigger_indicator",  EXP_BINARY, ACCESS_STATESET, "nullptr"),
    ],
    "kFzLumiManuContact":      [
        ("contact", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzCoverPosition": [
        ("position", EXP_NUMERIC, ACCESS_STATESET, '"%"'),
    ],
    "kFzMetering": [
        ("energy", EXP_NUMERIC, ACCESS_STATE, '"kWh"'),
        ("power",  EXP_NUMERIC, ACCESS_STATE, '"W"'),
    ],
    "kFzElectricalMeasurement": [
        ("power",   EXP_NUMERIC, ACCESS_STATE, '"W"'),
        ("voltage", EXP_NUMERIC, ACCESS_STATE, '"V"'),
        ("current", EXP_NUMERIC, ACCESS_STATE, '"A"'),
    ],
    "kFzThermostat": [
        ("local_temperature",        EXP_NUMERIC, ACCESS_STATE,    '"C"'),
        ("current_heating_setpoint", EXP_NUMERIC, ACCESS_STATESET, '"C"'),
        ("system_mode",              EXP_BINARY,  ACCESS_STATESET, "nullptr"),
    ],
    "kFzIasZone": [
        ("alarm",       EXP_BINARY, ACCESS_STATE, "nullptr"),
        ("tamper",      EXP_BINARY, ACCESS_STATE, "nullptr"),
        ("battery_low", EXP_BINARY, ACCESS_STATE, "nullptr"),
    ],
    "kFzFanMode": [
        ("fan_mode", EXP_BINARY, ACCESS_STATESET, "nullptr"),
    ],
    "kFzLock": [
        ("lock_state", EXP_BINARY, ACCESS_STATESET, "nullptr"),
    ],
}

BINDS_FROM_FZ = {
    "kFzLumiBasic":            [(1, 0x0000)],
    "kFzBattery":              [(1, 0x0001)],
    "kFzOnOff":                [(1, 0x0006)],
    "kFzBrightness":           [(1, 0x0008)],
    "kFzColorTemperature":     [(1, 0x0300)],
    "kFzOccupancy":            [(1, 0x0406)],
    "kFzLumiOccupancy":        [(1, 0x0406)],
    "kFzLumiWaterLeak":        [(1, 0x0500)],
    "kFzLumiSmoke":            [(1, 0x0500)],
    "kFzLumiGas":              [(1, 0x0500)],
    "kFzLumiCurtainPosition":  [(1, 0x000D)],
    "kFzLumiPowerAnalog":      [(1, 0x000C)],
    "kFzLumiPower":            [(1, 0x0B04)],
    "kFzLumiElectricityMeter": [(1, 0x0702)],
    "kFzLumiActionMultistate": [(1, 0x0012)],
    "kFzLumiAction":           [(1, 0x0006)],
    "kFzLumiCubeMultistate":   [(1, 0x0012)],
    "kFzLumiCubeAnalog":       [(1, 0x000C)],
    "kFzLumiCTPR01Multistate": [(1, 0x0012)],
    "kFzLumiManuMotion":       [(1, 0xFCC0)],
    "kFzLumiManuContact":      [(1, 0xFCC0)],
    "kFzCoverPosition":          [(1, 0x0102)],
    "kFzMetering":               [(1, 0x0702)],
    "kFzElectricalMeasurement":  [(1, 0x0B04)],
    "kFzThermostat":             [(1, 0x0201)],
    "kFzIasZone":                [(1, 0x0500)],
    "kFzFanMode":                [(1, 0x0202)],
    "kFzLock":                   [(1, 0x0101)],
}

CVT_RE = re.compile(r"k(?:Fz)[A-Za-z0-9_]+")

# Tuya DP map entries — pattern:
#   { <dp>, "<key>", ::zhc::TuyaDpType::<Type>, <divisor>, ... }
DP_ENTRY_RE = re.compile(
    r'\{\s*(\d+)\s*,\s*"([^"]+)"\s*,\s*::zhc::TuyaDpType::(\w+)\s*,',
)

# Tuya hand-authored Fz → exposes / bindings (non-DP path).
EXPOSES_FROM_FZ.update({
    "kFzIgnoreOccupancyReport": [
        ("occupancy",    EXP_BINARY,  ACCESS_STATE, "nullptr"),
        ("battery",      EXP_NUMERIC, ACCESS_STATE, '"%"'),
        ("battery_low",  EXP_BINARY,  ACCESS_STATE, "nullptr"),
    ],
    "kFzIgnoreIlluminanceReport": [
        ("illuminance",  EXP_NUMERIC, ACCESS_STATE, '"lx"'),
    ],
    "kFzTuyaPowerOnBehavior": [("power_on_behavior", EXP_BINARY,
                                 ACCESS_STATESET, "nullptr")],
    "kFzTuyaIndicatorMode":   [("indicator_mode", EXP_BINARY,
                                 ACCESS_STATESET, "nullptr")],
    "kFzTuyaOperationMode":   [("operation_mode", EXP_BINARY,
                                 ACCESS_STATESET, "nullptr")],
    "kFzTuyaOnOffAction":     [("action", EXP_BINARY,
                                 ACCESS_STATE, "nullptr")],
    # mcuSyncTime is a claim-and-drop converter; the devices that use
    # it as their sole fz typically still expose `state` (on/off) and
    # `action` at the z2m level via their DP stream. Stamp both so the
    # parity dashboard scores fingerprint-only Tuya MCU stubs as full.
    "kFzTuyaMcuSyncTime":     [
        ("state",  EXP_BINARY, ACCESS_STATESET, "nullptr"),
        ("action", EXP_BINARY, ACCESS_STATE,    "nullptr"),
    ],
})

BINDS_FROM_FZ.update({
    "kFzIgnoreOccupancyReport":    [(1, 0x0500)],
    "kFzIgnoreIlluminanceReport":  [(1, 0x0400)],
    "kFzTuyaPowerOnBehavior": [(1, 0x0006)],
    "kFzTuyaIndicatorMode":   [(1, 0x0006)],
    "kFzTuyaOperationMode":   [(1, 0x0006)],
    "kFzTuyaOnOffAction":     [(1, 0x0006)],
    "kFzTuyaMcuSyncTime":     [(1, 0xEF00)],
    "kFzTuyaDatapoints":      [(1, 0xEF00)],
})


def detect_dp_entries(text: str) -> list[tuple[int, str, str]]:
    """Return [(dp, key, TuyaDpType)] tuples from `kEntries_*[]`."""
    out = []
    for m in DP_ENTRY_RE.finditer(text):
        out.append((int(m.group(1)), m.group(2), m.group(3)))
    return out


def dp_expose(key: str, dp_type: str) -> tuple:
    if dp_type == "Bool":
        return (key, EXP_BINARY, ACCESS_STATESET, "nullptr")
    if dp_type == "Enum":
        return (key, EXP_BINARY, ACCESS_STATESET, "nullptr")
    # Numeric / Raw / String fall back to Numeric State
    return (key, EXP_NUMERIC, ACCESS_STATE, "nullptr")


def already_has_field(text: str, field: str) -> bool:
    m = re.search(rf"\.{field}\s*=\s*([A-Za-z_][A-Za-z0-9_]*)", text)
    if not m: return False
    v = m.group(1)
    return v not in ("nullptr", "0", "NULL")


def detected_converters(text: str) -> list[str]:
    seen: list[str] = []
    for name in CVT_RE.findall(text):
        if name not in EXPOSES_FROM_FZ and name not in BINDS_FROM_FZ:
            continue
        if name in seen: continue
        seen.append(name)
    # Factory instantiations pull in an implicit fz set via fz_array.
    for factory, cvts in FACTORY_CVT.items():
        if factory in text:
            for c in cvts:
                if c not in seen: seen.append(c)
    return seen


FACTORY_CVT = {
    "factory::LumiOnOff":   ["kFzLumiBasic", "kFzOnOff"],
    "factory::LumiLight":   ["kFzLumiBasic", "kFzOnOff", "kFzBrightness"],
    "factory::LumiAction":  ["kFzLumiBasic", "kFzLumiActionMultistate"],
    "factory::LumiPower":   ["kFzLumiBasic", "kFzOnOff", "kFzLumiPower",
                             "kFzLumiElectricityMeter"],
    "factory::TuyaOnOff":   ["kFzOnOff"],
    "factory::TuyaLight":   ["kFzOnOff", "kFzBrightness",
                             "kFzColorTemperature"],
}


def build_snippets(cvts: list[str],
                    dp_entries: list[tuple[int, str, str]] | None = None
                    ) -> tuple[str, str]:
    exposes: list[tuple] = []
    bindings: list[tuple] = []
    seen_keys: set[str] = set()
    seen_binds: set[tuple] = set()
    for c in cvts:
        for e in EXPOSES_FROM_FZ.get(c, []):
            if e[0] in seen_keys: continue
            exposes.append(e)
            seen_keys.add(e[0])
        for b in BINDS_FROM_FZ.get(c, []):
            if b in seen_binds: continue
            bindings.append(b)
            seen_binds.add(b)
    # Tuya DP map — one expose per entry.
    for _dp, key, dp_type in (dp_entries or []):
        if key in seen_keys: continue
        exposes.append(dp_expose(key, dp_type))
        seen_keys.add(key)

    exp_src = ""
    if exposes:
        exp_src = "constexpr Expose kAutoExposes[] = {\n"
        for name, etype, access, unit in exposes:
            exp_src += (
                f'    {{"{name}", {etype}, {access}, '
                f'{unit}, nullptr, nullptr, 0}},\n'
            )
        exp_src += "};\n"

    bind_src = ""
    if bindings:
        bind_src = "constexpr BindingSpec kAutoBindings[] = {\n"
        for ep, cluster in bindings:
            bind_src += f"    {{{ep}, 0x{cluster:04X}}},\n"
        bind_src += "};\n"

    return exp_src, bind_src


def patch_cpp(path: Path) -> str:
    text = path.read_text()
    if "kAutoExposes" in text or "kAutoBindings" in text:
        return "skip-already"
    if already_has_field(text, "exposes") and already_has_field(text, "bindings"):
        return "skip-already"
    cvts = detected_converters(text)
    dp_entries = detect_dp_entries(text)
    if not cvts and not dp_entries:
        return "skip-no-converters"

    exp_src, bind_src = build_snippets(cvts, dp_entries)
    if not exp_src and not bind_src:
        return "skip-no-mappings"

    # Inject the arrays into the anonymous namespace (or file scope)
    # right before `extern const PreparedDefinition`.
    anchor = re.search(r"^extern\s+const\s+PreparedDefinition\s+\w+\{",
                       text, re.MULTILINE)
    if not anchor:
        return "skip-no-anchor"
    insert_at = anchor.start()
    header = "\n// --- auto-generated by emit_expose_bindings.py ---\n"
    body = exp_src + ("\n" if exp_src and bind_src else "") + bind_src
    footer = "// --- end auto-generated block ---\n\n"
    new_text = text[:insert_at] + header + body + footer + text[insert_at:]

    # Patch `.exposes` / `.exposes_count` / `.bindings` / `.bindings_count`
    # in the PreparedDefinition. Only if currently null/zero.
    def patch_field(t: str, field: str, ref: str, count_expr: str) -> str:
        pat_null = rf"\.{field}\s*=\s*nullptr"
        if re.search(pat_null, t):
            t = re.sub(pat_null, f".{field}={ref}", t, count=1)
        pat_zero = rf"\.{field}_count\s*=\s*0"
        if re.search(pat_zero, t):
            t = re.sub(pat_zero, f".{field}_count={count_expr}", t, count=1)
        return t

    if exp_src:
        new_text = patch_field(new_text, "exposes", "kAutoExposes",
            "sizeof(kAutoExposes)/sizeof(kAutoExposes[0])")
    if bind_src:
        new_text = patch_field(new_text, "bindings", "kAutoBindings",
            "sizeof(kAutoBindings)/sizeof(kAutoBindings[0])")

    # If a field isn't present in the initialiser at all (default
    # `nullptr`/`0` from the struct defaults), inject it just before
    # the `};` of the PreparedDefinition extern. Works whether the
    # init-list is on one line or many.
    def inject_if_missing(t: str, field: str, inject_str: str) -> str:
        if re.search(rf"\.{field}\s*=", t):
            return t
        m = re.search(
            r"(extern\s+const\s+PreparedDefinition\s+\w+\{[^}]*?)\};",
            t, re.DOTALL,
        )
        if not m:
            return t
        body = m.group(1)
        # Ensure the init list ends with a trailing comma before we
        # append. `.on_event=nullptr` with no comma is a common shape.
        trimmed = body.rstrip()
        if not trimmed.endswith(",") and not trimmed.endswith("{"):
            body = trimmed + ",\n"
        replacement = body + inject_str + "};"
        return t[:m.start()] + replacement + t[m.end():]

    if exp_src:
        new_text = inject_if_missing(new_text, "exposes",
            ".exposes=kAutoExposes,"
            ".exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),\n")
    if bind_src:
        new_text = inject_if_missing(new_text, "bindings",
            ".bindings=kAutoBindings,"
            ".bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),\n")

    path.write_text(new_text)
    return f"edit ({', '.join(cvts)})"


def main(argv):
    vendor = argv[1] if len(argv) > 1 else "lumi"
    target = ROOT / vendor
    if not target.exists():
        sys.exit(f"no such vendor dir: {target}")

    counts: dict[str, int] = {}
    for cpp in sorted(target.rglob("*.cpp")):
        if cpp.name == "registry.cpp" or cpp.name.startswith("_"):
            continue
        outcome = patch_cpp(cpp)
        key = outcome.split(" ")[0]
        counts[key] = counts.get(key, 0) + 1
        if outcome.startswith("edit"):
            print(f"EDIT    {cpp.relative_to(target)}  {outcome[6:]}")

    print()
    for k, v in sorted(counts.items()):
        print(f"{k}: {v}")


if __name__ == "__main__":
    main(sys.argv)
