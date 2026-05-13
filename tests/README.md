# ZHC Host Tests

Behavioural-parity tests for the static-memory ZCL library. They run on the
host (Linux/macOS) inside the `embedded-zhc/build/` CMake tree, untouched by
ESP-IDF.

## Layout

```
tests/
├── test_main.cpp                — sanity bootstrap (capacity macros, FixedPayload)
├── test_foundation.cpp          — parser primitives
├── test_decoder.cpp             — ZCL decoder
├── test_dispatch.cpp            — converter selection / merging
├── test_tuya_dp.cpp             — raw Tuya DP value decode
├── test_tuya_devices.cpp        — registry shape ("N ported" smoke)
├── test_tuya_base_device.cpp    — full-stack DP-map decode of a real device
├── test_tuya_tz.cpp             — write-path encoding
├── test_tuya_mcu_sync_time.cpp  — Tuya MCU sync-time hook capture
├── test_lumi_*.cpp              — Aqara/Lumi device family
├── test_moes_bht002.cpp         — Moes BHT-002 thermostat (fixture, 4 cases)
├── test_moes_zht_sr.cpp         — Moes ZHT-SR smart-ring thermostat (fixture, 4 cases)
├── test_zosung_ir.cpp           — Zosung IR runtime — UFO-R11 (fixture, 4 cases)
├── …
└── fixtures/                    — JSON fixture inputs consumed by zhc_fixture_runner
```

Every test file pairs with one CMake target:

```cmake
add_executable(zhc_<name>_tests tests/test_<name>.cpp)
target_link_libraries(zhc_<name>_tests PRIVATE zhc_definitions)
add_test(NAME zhc_<name>_tests COMMAND zhc_<name>_tests)
```

Tests assert with plain `<cassert>`. A `main()` returning 0 = pass; an
`assert` failure = fail. No GTest, no JUnit.

## Adding a fixture test for a new device

The pattern below is what `test_moes_bht002.cpp`, `test_moes_zht_sr.cpp`,
and `test_zosung_ir.cpp` use. It captures z2m parity at the
`dispatch_from_zigbee` boundary — i.e. exactly the layer that the firmware
adapter hands traffic to.

### 1. Pick a device, find its `kDef_*` symbol

Devices live under `embedded-zhc/definitions/<vendor>/`. Each
`extern const PreparedDefinition kDef_<MODEL>` is the testable entry point.

### 2. Pick datapoints / wire frames worth pinning

Read the device's `.cpp` and the matching z2m source (commit referenced in
the file header). For each test case decide:

- The cluster + command / DP id you're feeding in.
- The exact wire bytes (hex literal — don't reverse-engineer in the test).
- The expected output key + `ValueType` + value, taken from z2m.

For Tuya devices, frames are described as a `TuyaDpRecord[]`:

```cpp
const std::uint8_t kRaw235[] = { 0x00, 0x00, 0x00, 0xEB }; // BE u32 = 235
const TuyaDpRecord recs[] = {
    { /*dp_id=*/24, /*type=*/0x02, std::span{kRaw235, 4} },
};
```

Type byte cheat-sheet (`zhc/zcl/tuya_dp.hpp`):

| Tuya DP type | Wire byte | Body shape       |
|--------------|-----------|------------------|
| Raw          | `0x00`    | bytes            |
| Bool         | `0x01`    | 1 byte (0/1)     |
| Numeric      | `0x02`    | 4 BE bytes (s32) |
| String       | `0x03`    | bytes            |
| Enum         | `0x04`    | 1 byte           |
| Bitmap       | `0x05`    | 1 / 2 / 4 bytes  |

For non-Tuya ZCL frames feed `InboundApsFrame.data` with a hand-rolled
ZCL header (FC, TSN, cmd id, body) and let `decode_zcl_frame` populate
`DecodedMessage` — see `test_tuya_mcu_sync_time.cpp` for a worked example.

### 3. Build the dispatch context and call

```cpp
DecodedMessage msg{};
msg.family       = FrameFamily::TuyaDp;
msg.type         = MessageType::Command;
msg.cluster      = "manuSpecificTuya";
msg.direction    = Direction::ServerToClient;
msg.command_id   = 0x02;
msg.src_endpoint = 1;
msg.dst_endpoint = 1;

InboundApsFrame raw{};
raw.cluster_id   = 0xEF00;
raw.src_endpoint = 1;
raw.dst_endpoint = 1;
raw.linkquality  = 0xC8;

RuntimeContext ctx{};
const auto result = dispatch_from_zigbee(
    msg, std::span<const TuyaDpRecord>(recs, 1),
    devices::moes::kDef_BHT_002, raw, ctx);

assert(result.any_matched);
const Value* v = result.merged.find("local_temperature");
assert(v && v->type == ValueType::Float);
assert(approx(v->f, 23.5f, 0.001f));
```

### 4. Capturing emitted commands (response-builder tests)

For converters that emit cluster commands (Zosung IR, configure hooks,
etc.), wire `RuntimeContext::configure_cmd` to a capture lambda /
free function. The captured tuple matches `ConfigureCmdFn`:

```cpp
struct CapturedCmd {
    std::uint16_t device_index;
    std::uint8_t  endpoint;
    std::uint16_t cluster_id;
    std::uint8_t  cmd_id;
    std::vector<std::uint8_t> payload;
    std::uint8_t  flags;
};
std::vector<CapturedCmd> g_captured;

bool capture_cmd(std::uint16_t idx, std::uint8_t ep,
                 std::uint16_t cluster, std::uint8_t cmd,
                 const std::uint8_t* payload, std::uint8_t payload_len,
                 std::uint8_t flags) {
    g_captured.push_back({idx, ep, cluster, cmd,
                          std::vector<std::uint8_t>(payload, payload + payload_len),
                          flags});
    return true;
}

RuntimeContext ctx{};
ctx.configure_cmd = &capture_cmd;
```

See `test_zosung_ir.cpp` for the full pattern (cmd 0x04 → cmd 0x05 ack).

### 5. Compare merged Value table to expected dict

`DispatchResult::merged` is a `FixedPayload<N>` — an array-backed map.
Use `find(key)` to retrieve, then assert `type` + the union member that
matches z2m's emitted JSON shape:

| z2m JSON                     | ZHC `Value`                         |
|------------------------------|-------------------------------------|
| `"key": true / false`        | `type=Bool`, `b`                    |
| `"key": 23.5`                | `type=Float`, `f`                   |
| `"key": 5` (integer)         | `type=Uint` or `Int`, `u` / `i`     |
| `"key": "Eco"`               | `type=StringRef`, `str` (compare with `strcmp`) |
| `"key": "<base64>"`          | `type=StringRef`                    |
| `"key": <bytes>`             | `type=BytesRef`, `bytes` (span)     |

Enum DPs that have an `enum_table` in the device map emit `StringRef`;
plain Bool DPs emit `Bool`. The adapter does the LOCK/UNLOCK / ON/OFF
friendly-name mapping when shadow-formatting — don't try to assert
those strings here.

### 6. Wire up CMake

Append to `embedded-zhc/CMakeLists.txt` next to the other test
executables (alphabetically grouped):

```cmake
add_executable(zhc_<name>_tests tests/test_<name>.cpp)
target_link_libraries(zhc_<name>_tests PRIVATE zhc_definitions)
```

…and to the `add_test` block further down:

```cmake
add_test(NAME zhc_<name>_tests COMMAND zhc_<name>_tests)
```

Then:

```bash
cd embedded-zhc/build
cmake .              # picks up the new target
cmake --build . --target zhc_<name>_tests
./zhc_<name>_tests
```

## Honesty constraints — runtime gaps

Some converters are wired faithfully to z2m's algorithm but rely on
runtime affordances that haven't landed yet (e.g. the Zosung handlers
need raw-APS-payload access — `msg.payload` doesn't surface custom
cluster-command bodies). Tests that hit those paths can:

1. Assert structural correctness — number of frames, cluster id, cmd
   id, payload size — without asserting payload bytes derived from the
   inbound frame.
2. Mark the gap with `// TODO(fixture):` so it's grep-able.
3. Mention the limitation in the test header comment.

`test_zosung_ir.cpp` is the reference example.

## Running the suite

```bash
cd embedded-zhc/build
ctest --output-on-failure
```

Or selectively:

```bash
./zhc_moes_bht002_tests && ./zhc_moes_zht_sr_tests && ./zhc_zosung_ir_tests
```
