#!/usr/bin/env python3
"""Guard: a Celsius unit is spelled "°C", never the bare ASCII "C".

Why this exists
---------------
Half the temperature exposes (827 units in 543 definitions, 2026-10-03) were
declared with unit "C" while the other half used "°C", as z2m's
`e.temperature()` does. The hub's web UI and the cloud print the unit as
given, so an air-quality sensor read "25.4 C", and the Home Assistant bridge
passes it on as `unit_of_measurement`, which Home Assistant does not accept
for a temperature sensor.

Three shapes carry the unit:

    {"temperature", ExposeType::Numeric, Access::State, "C", nullptr, ...}
    {"local_temperature", ExposeType::Numeric, Access::State,
      "C", nullptr, ...}                       (continuation line)
    static constexpr const char* unit = "C";   (modern-extend option struct)

`"\\xC2\\xB0""C"` and a literal "°C" both pass.

    python3 tools/lint_celsius_unit.py [definitions_dir]

Exit 0 when no definition uses the bare "C".
"""
import pathlib
import re
import sys

ACCESS = r'(?:::zhc::)?Access::\w+(?:\s*\|\s*(?:::zhc::)?Access::\w+)*'
EXPOSE = re.compile(r'ExposeType::\w+,\s*' + ACCESS + r',\s*"C",')
CONT_PREV = re.compile(r'ExposeType::\w+,\s*' + ACCESS + r',\s*$')
CONT = re.compile(r'^\s*"C",')
OPTS = re.compile(r'\bunit\s*=\s*"C"')


def main():
    root = pathlib.Path(sys.argv[1] if len(sys.argv) > 1 else
                        pathlib.Path(__file__).resolve().parent.parent / 'definitions')
    bad = []
    for path in sorted(root.rglob('*.[ch]pp')):
        lines = path.read_text(encoding='utf-8').split('\n')
        for i, line in enumerate(lines):
            cont = i > 0 and CONT.match(line) and CONT_PREV.search(lines[i - 1])
            if EXPOSE.search(line) or cont or OPTS.search(line):
                bad.append(f'{path.relative_to(root.parent)}:{i + 1}: {line.strip()}')
    for b in bad[:40]:
        print(b)
    if bad:
        print(f'\n{len(bad)} Celsius unit(s) spelled "C"; use "°C".')
        return 1
    print('every Celsius unit is "°C"')
    return 0


if __name__ == '__main__':
    sys.exit(main())
