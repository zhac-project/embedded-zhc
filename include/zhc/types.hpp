// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/config.hpp"

namespace zhc {

// Wire framing discriminator. `Zcl` is the standard ZCL attribute /
// command path; `TuyaDp` is the Tuya `0xEF00` cluster carrying the
// DP record list that doesn't fit the ZCL attribute shape.
enum class FrameFamily : uint8_t { Zcl, TuyaDp };

enum class MessageType : uint8_t {
    AttributeReport,
    ReadResponse,
    Read,
    Write,
    Command,
    Raw,
};

enum class Direction : uint8_t { ClientToServer, ServerToClient };

// Neutral frame delivered by the platform adapter to the portable core.
struct InboundApsFrame {
    uint16_t cluster_id;
    uint16_t src_addr;
    uint16_t group_id;
    uint8_t  src_endpoint;
    uint8_t  dst_endpoint;
    uint8_t  linkquality;
    std::span<const uint8_t> data;
};

template <std::size_t N>
struct FixedPayload;

// Handle to a nested structured payload (e.g. Xiaomi MI-struct).
// The pointer aliases storage owned by `DecodedMessage` — no heap.
struct ObjectView {
    const FixedPayload<ZHC_MI_STRUCT_CAP>* payload;
};

// Tuya DP record view: { id, type, raw bytes } — no value decode yet.
struct TuyaDpRecord {
    uint8_t dp_id;
    uint8_t dp_type;
    std::span<const uint8_t> value;
};

enum class ValueType : uint8_t {
    None,
    Bool,
    Uint,
    Int,
    Float,
    StringRef,
    BytesRef,
    ObjectRef,
};

// Compact tagged union. Union inactive slots are left uninitialised.
struct Value {
    ValueType type{ValueType::None};
    union {
        bool b;
        std::uint64_t u;
        std::int64_t  i;
        float f;
        const char* str;
        std::span<const std::uint8_t> bytes;
        const ObjectView* obj;
    };
};

struct KeyValue {
    const char* key;
    Value value;
};

// Fixed-capacity key/value store. `put` returns false when the backing
// array is full; callers must not assume the operation succeeded.
template <std::size_t N>
struct FixedPayload {
    std::array<KeyValue, N> items{};
    std::uint8_t count{0};

    bool put(const char* key, Value value) {
        if (count >= N) return false;
        items[count++] = KeyValue{key, value};
        return true;
    }

    const Value* find(const char* key) const {
        for (std::uint8_t i = 0; i < count; ++i) {
            if (items[i].key == key) return &items[i].value;  // fast pointer eq
            if (std::strcmp(items[i].key, key) == 0) return &items[i].value;
        }
        return nullptr;
    }
};

// Strict bound: at 255, `for (uint8_t i = 0; i < count; ++i)` never
// terminates because `++i` wraps. The runtime walkers below sit one
// off the cap, so reserve one slot.
static_assert(ZHC_FIXED_PAYLOAD_CAP < 255,
              "FixedPayload.count + loop iterators are uint8_t; lower "
              "ZHC_FIXED_PAYLOAD_CAP below 255 or widen count");

// ── WhiteLabel ──────────────────────────────────────────────────────
// Alternate (vendor, model) labels for the same physical device — sold
// under multiple SKUs but identified by the same Zigbee fingerprint
// and driven by the same converters. Pure metadata: discovery /
// inventory UIs surface these to the user, the runtime ignores them.
struct WhiteLabel {
    const char* vendor;
    const char* model;
};

// ── Expose ──────────────────────────────────────────────────────────
// Static schema descriptor per device attribute — drives validation
// and future MQTT discovery. Owned by `PreparedDefinition`.
// String — free-form text (e.g. base64-encoded IR codes). New in
// 2026-04: introduced for the Zosung IR runtime so `learned_ir_code`
// and `ir_code_to_send` exposes can carry an opaque payload that the
// shadow stores verbatim. Adapter exposes_json maps to "text" to stay
// aligned with z2m's `exposes.text(...)` shape.
enum class ExposeType : std::uint8_t { Numeric, Enum, Binary, String };
enum class Access     : std::uint8_t { State, Set, StateSet };

// z2m-style expose classification. Mirrors exposes.ts `category`.
//   State      — live device state attribute (shown on the primary
//                "States" tab). Default when unspecified.
//   Config     — device-specific configuration (shown on the
//                "Options" tab). Examples: power_on_behavior,
//                motion_debounce, no_motion_timeout.
//   Diagnostic — device health / debug (battery, linkquality).
enum class ExposeCategory : std::uint8_t { State, Config, Diagnostic };

struct Expose {
    const char*  name;
    ExposeType   type;
    Access       access;
    const char*  unit;
    const char*  description;
    const char* const* enum_values;
    std::uint8_t enum_count;
    // Default-initialised trailing field: existing positional
    // aggregate initialisers without this member stay valid and
    // default to State.
    ExposeCategory category = ExposeCategory::State;
};

// Decoded form of an inbound frame after the core parser runs. Members:
//   unknown_key_scratch — storage for decimal-formatted unknown attr ids.
//     Keys returned by the parser point into this buffer; valid for the
//     lifetime of the DecodedMessage. ~192 bytes at default capacity.
//   mi_struct_arena — storage for Xiaomi `0xFF01` / MI-struct payload.
//     ObjectView pointers from `Value::obj` alias this member.
struct DecodedMessage {
    FrameFamily  family;
    const char*  cluster;
    MessageType  type;
    Direction    direction;
    std::uint16_t command_id;
    std::uint8_t  transaction_sequence;
    bool          manufacturer_specific;
    std::uint16_t manufacturer_code;
    std::uint8_t  src_endpoint;
    std::uint8_t  dst_endpoint;
    std::uint8_t  linkquality;
    std::uint16_t group_id;
    std::span<const std::uint8_t> raw_data;   // full frame incl. ZCL header
    std::span<const std::uint8_t> raw_body;   // post-header cluster-cmd body
    FixedPayload<ZHC_FIXED_PAYLOAD_CAP> payload;
    // Scratch arena for Xiaomi MI-struct TLV (attr 0xFF01). `mutable` so
    // converters that take `const DecodedMessage&` can reuse it without
    // paying the 700 B+ cost of a local on a ~4 KB RTOS task stack.
    mutable FixedPayload<ZHC_MI_STRUCT_CAP> mi_struct_arena;
    char unknown_key_scratch[8 * ZHC_FIXED_PAYLOAD_CAP];
};

}  // namespace zhc
