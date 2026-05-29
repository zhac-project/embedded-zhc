<!--
SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
SPDX-License-Identifier: Apache-2.0
-->
# Fuzzing the wire-frame parsers (FINDINGS.md F31)

`fuzz_parsers.cpp` is a libFuzzer harness over the four hand-rolled walkers in
`src/zcl/foundation.cpp` — `parse_report_attributes`, `parse_read_attr_response`,
`parse_mi_struct`, `parse_tuya_dp_stream`. These decode attacker-influenced
bytes straight off the radio, so they are the highest-value fuzz target.

The first input byte selects the parser; the rest is the raw payload.

## Build & run (host, clang + libFuzzer + ASan/UBSan)

```sh
cmake -B build-fuzz -DZHC_FUZZ=ON \
      -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .
cmake --build build-fuzz --target zhc_fuzz_parsers
mkdir -p corpus
./build-fuzz/zhc_fuzz_parsers -max_total_time=300 corpus/
```

`-DZHC_FUZZ=ON` compiles the harness with `-fsanitize=fuzzer,address,undefined`.
A crash drops a `crash-*` reproducer in the cwd; re-run it through the binary to
triage. Seed `corpus/` from `tests/` fixtures for faster coverage.

## CI

A short `-max_total_time=60` smoke run is wired into
`embedded-zhc/.github/workflows/ci.yml` (job `fuzz-smoke`) so regressions that
crash on trivial inputs fail the build. Long campaigns run out-of-band.

> Note: this harness was authored but **not** executed in the dev sandbox
> (host `g++`/`as` is broken there); first run it locally and commit a seed
> corpus + any reproducers found.
