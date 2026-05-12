# Contributing to embedded-zhc

Thanks for considering a contribution! This repo holds the
host-testable C++20 ZHC device library.

## License and CLA — read this first

`embedded-zhc` is licensed under **Apache-2.0**. Your contribution is
submitted under the same license as the file you are editing.

Every contributor must sign the Contributor License Agreement in
`CLA.md` before their first patch is merged. The CLA grants the
maintainer broad rights including the ability to relicense; you keep
copyright in your contributions.

### How to sign

1. Add your name, email, and GitHub handle to
   `CONTRIBUTORS.md` as part of your first PR.
2. Include in the PR description:

   > I have read and agree to the Contributor License Agreement
   > in `CLA.md`. This and all future contributions I make to this
   > repository are submitted under its terms.

Signing is one-time across the whole ZHAC project — signing in one
sub-repo covers you for all of them.

## Building

```bash
cmake -B build -S .
cmake --build build -j
ctest --test-dir build
```

Requires C++20, CMake ≥3.20, a host compiler (gcc ≥12 or clang ≥15).

## SPDX headers for new files

```cpp
// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
```

Python (generator scripts):
```python
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
```

CMake:
```cmake
# SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
# SPDX-License-Identifier: Apache-2.0
```

## Adding a new Zigbee device

Device definitions live under `definitions/<vendor>/` as C++20
`PreparedDefinition` records.

1. Check whether the device exists in an upstream z2m file — most
   work is mechanical adaptation from the TypeScript definition.
2. Fast path: use the port generators under `tools/`. They produce a
   skeleton from the z2m source. (These scripts may be private —
   upstream contributors coordinate with the maintainer.)
3. Always preserve the `// z2m-source:` comment pointing at the
   upstream origin — keeps attribution intact.
4. Add a host test under `tests/` covering at least the
   fingerprint + one from-Zigbee path.
5. Build + run tests locally. Open the PR with a one-line
   CHANGELOG entry.

## Style

- 4-space indent.
- `snake_case` for variables/functions, `UPPER_CASE` for macros,
  `PascalCase` for structs/enums.
- No `std::string`, `std::vector`, or other heap-using containers.
  Library is static-memory to run on MCU.
- No exceptions, no RTTI.

## Reporting bugs

Open an issue with:
- Version tag (e.g., `v2026042301`)
- Minimal reproduction — ideally a host-side test case that fails
- Expected vs. observed behaviour
