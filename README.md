# embedded-zhc

Host-testable C++20 static-memory library that consolidates the
Zigbee device-definition layer used by [ZHAC]. Hubs built on it recognise
4 989 device definitions from 400 manufacturers, adapted from
[zigbee-herdsman-converters] into C++ records with no heap allocation
([searchable list](https://zhac.org/devices/)).

[ZHAC]: https://github.com/zhac-project/zhac-docs
[zigbee-herdsman-converters]: https://github.com/Koenkk/zigbee-herdsman-converters

## What this library gives you

- `zhc::PreparedDefinition` — static-memory device record (fingerprint,
  exposes, fromZigbee / toZigbee converters).
- `zhc::DefinitionRegistry` — O(1) lookup by ZCL model + manufacturer.
- `zhc::TzConverter` — write-back pipeline for attributes.
- Exposes-bindings generator — build JSON descriptions of a device's
  controllable surface.
- No dynamic allocation. No exceptions. No RTTI. Fits on an MCU.

## Building standalone (host tests)

```bash
git clone https://github.com/zhac-project/embedded-zhc.git
cd embedded-zhc
cmake -B build -S .
cmake --build build -j
ctest --test-dir build
```

## Using as an ESP-IDF component

`embedded-zhc` is **not** published to the ESP Component Registry — it
ships its own CMake tree, which makes it useful on host too. Consumers
integrate it via `EXTRA_COMPONENT_DIRS` or `FetchContent`:

```cmake
# Option A — pinned submodule
set(EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/../embedded-zhc/include")

# Option B — FetchContent pin
include(FetchContent)
FetchContent_Declare(
    embedded_zhc
    GIT_REPOSITORY https://github.com/zhac-project/embedded-zhc.git
    GIT_TAG        v2026051101
)
FetchContent_MakeAvailable(embedded_zhc)
set(EXTRA_COMPONENT_DIRS ${embedded_zhc_SOURCE_DIR}/include)
```

## Tree

```
embedded-zhc/
├── include/                (public headers — device records, runtime)
├── src/                    (library implementation)
├── definitions/            (per-vendor device ports)
│   ├── tuya/
│   ├── lumi/
│   ├── philips/
│   └── ... ~70 more vendors
├── tests/                  (host-side Catch2 tests)
├── tools/                  (lint, audit and test helpers the build runs)
└── CMakeLists.txt
```

Device definitions are ordinary C++: read them, change them and send a
pull request, files under `generated/` included. Those are imported from
zigbee-herdsman-converters by the maintainer's import scripts, which are
not part of this repo and not needed to build or change anything here.
When a pull request changes one, the maintainer moves that definition to
a hand-kept file next to it (marked `// Tier 2`), so the next import
keeps the change.

## License

Apache License 2.0 — see `LICENSE` and `LICENSES/Apache-2.0.txt`.

Device-definition ports under `definitions/` derive from the MIT-licensed
[zigbee-herdsman-converters][z2m] project. Adapted material is
redistributed under Apache-2.0; original upstream attribution is in
`NOTICE`, and the full upstream MIT license text is preserved in
`LICENSES/MIT.txt`.

## Contributing

See `CONTRIBUTING.md`. All contributions require signing `CLA.md`.

## Versioning

Releases tagged `vYYYYMMDDVV` (date + 2-digit revision).
