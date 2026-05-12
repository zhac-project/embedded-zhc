# embedded-zhc

Host-testable C++20 static-memory library that consolidates the
Zigbee device-definition layer used by [ZHAC]. Ports 373 vendors and
~6 600 devices adapted from [zigbee-herdsman-converters] into
C++ records with no heap allocation.

[ZHAC]: https://github.com/zhac-project/zhac-platform
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
├── tools/                  (port generators — optional)
└── CMakeLists.txt
```

The generator scripts under `tools/` may move to a separate private
repo later; the `definitions/` tree itself is fully self-contained.

## License

Apache License 2.0 — see `LICENSE` and `LICENSES/Apache-2.0.txt`.

Device-definition ports under `definitions/` derive from the MIT-licensed
[zigbee-herdsman-converters][z2m] project. Adapted material is
redistributed under Apache-2.0; original upstream attribution is in
`NOTICE`.

## Contributing

See `CONTRIBUTING.md`. All contributions require signing `CLA.md`.

## Versioning

Releases tagged `vYYYYMMDDVV` — see `zhac-platform` README for the
full scheme.
