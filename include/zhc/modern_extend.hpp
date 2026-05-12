// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// ── modernExtend helper pack ────────────────────────────────────────
//
// C++20 constexpr factories that mirror z2m's `modernExtend` builders.
// Every helper produces `.rodata`-resident converters or metadata so
// `PreparedDefinition`s stay zero-heap.
//
// The three converter factories (Numeric, EnumLookup, Binary) take a
// type template parameter — the caller defines a local struct with
// `static constexpr` members and passes the type in. This avoids the
// C++20 NTTP-with-`const char*` restriction (string literals don't
// have linkage, so class-type NTTPs with pointer fields reject them on
// GCC) without dragging in `fixed_string` boilerplate at every call
// site.
//
// Device usage example:
//
//   struct temperature_opts {
//       static constexpr const char*   name    = "temperature";
//       static constexpr const char*   unit    = "\xC2\xB0""C";
//       static constexpr const char*   cluster = "msTemperatureMeasurement";
//       static constexpr std::uint16_t attr    = 0x0000;
//       static constexpr std::uint32_t divisor = 100;
//   };
//   using Temp = m::Numeric<temperature_opts>;
//   static const FzConverter* const fz_list[] = { &Temp::converter };
//
// z2m-source: zigbee-herdsman-converters/src/lib/modernExtend.ts.

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::m {

// Stringify an attribute id into a small fixed buffer. The core decoder
// uses decimal-string fallback keys when no `known` attr_keys table is
// plumbed through, so all factory-generated converters lookup on the
// decimal representation.
inline std::size_t write_attr_key(std::uint16_t attr, char (&buf)[8]) {
    if (attr == 0) { buf[0] = '0'; buf[1] = '\0'; return 1; }
    char tmp[8];
    int  n = 0;
    std::uint16_t v = attr;
    while (v > 0 && n < 7) { tmp[n++] = '0' + (v % 10); v /= 10; }
    int i = 0;
    while (n > 0) buf[i++] = tmp[--n];
    buf[i] = '\0';
    return static_cast<std::size_t>(i);
}

// Common attr-report selector — matches AttributeReport + ReadResponse
// on any endpoint, server→client. Used by every passive helper below.
constexpr std::uint32_t kAttrReportMask =
    type_bit(MessageType::AttributeReport) |
    type_bit(MessageType::ReadResponse);

// ── Numeric ────────────────────────────────────────────────────────
//
// Opts members:
//   const char*   name
//   const char*   unit         (informational; unused by the converter)
//   const char*   cluster
//   std::uint16_t attr
//   std::uint32_t divisor      (0 or 1 → integer pass-through)

template <typename Opts>
struct Numeric {
    static bool fn(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
        char key[8];
        write_attr_key(Opts::attr, key);
        const Value* v = msg.payload.find(key);
        if (!v) return false;

        std::int64_t raw;
        if      (v->type == ValueType::Uint) raw = static_cast<std::int64_t>(v->u);
        else if (v->type == ValueType::Int)  raw = v->i;
        else return false;

        constexpr std::uint32_t d = Opts::divisor == 0 ? 1 : Opts::divisor;
        if (d == 1) {
            Value o{}; o.type = ValueType::Int; o.i = raw;
            out.put(Opts::name, o);
        } else {
            Value o{}; o.type = ValueType::Float;
            o.f = static_cast<float>(raw) / static_cast<float>(d);
            out.put(Opts::name, o);
        }
        return true;
    }

    static inline constexpr FzConverter converter{
        .family            = FrameFamily::Zcl,
        .cluster           = Opts::cluster,
        .type_mask         = kAttrReportMask,
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &Numeric::fn },
        .user_config       = nullptr,
    };
};

// ── Binary ─────────────────────────────────────────────────────────
//
// Opts members:
//   const char*   name
//   const char*   cluster
//   std::uint16_t attr

template <typename Opts>
struct Binary {
    static bool fn(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
        char key[8];
        write_attr_key(Opts::attr, key);
        const Value* v = msg.payload.find(key);
        if (!v) return false;

        bool state;
        if (v->type == ValueType::Bool)      state = v->b;
        else if (v->type == ValueType::Uint) state = v->u != 0;
        else return false;

        Value o{}; o.type = ValueType::Bool; o.b = state;
        out.put(Opts::name, o);
        return true;
    }

    static inline constexpr FzConverter converter{
        .family            = FrameFamily::Zcl,
        .cluster           = Opts::cluster,
        .type_mask         = kAttrReportMask,
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &Binary::fn },
        .user_config       = nullptr,
    };
};

// ── EnumLookup ─────────────────────────────────────────────────────
//
// Opts members:
//   const char*                        name
//   const char*                        cluster
//   std::uint16_t                      attr
//   std::array<EnumEntry, N> entries
//
// Fall-through when raw value matches no entry: converter returns
// `false` so downstream converters get a chance to claim the frame.

struct EnumEntry {
    std::uint64_t value;
    const char*   label;
};

template <typename Opts>
struct EnumLookup {
    static bool fn(const DecodedMessage& msg,
                    const FzConverter&,
                    const PreparedDefinition&,
                    RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
        char key[8];
        write_attr_key(Opts::attr, key);
        const Value* v = msg.payload.find(key);
        if (!v) return false;

        std::uint64_t raw;
        if      (v->type == ValueType::Uint) raw = v->u;
        else if (v->type == ValueType::Int)  raw = static_cast<std::uint64_t>(v->i);
        else if (v->type == ValueType::Bool) raw = v->b ? 1 : 0;
        else return false;

        for (const auto& e : Opts::entries) {
            if (e.value == raw) {
                Value o{}; o.type = ValueType::StringRef; o.str = e.label;
                out.put(Opts::name, o);
                return true;
            }
        }
        return false;
    }

    static inline constexpr FzConverter converter{
        .family            = FrameFamily::Zcl,
        .cluster           = Opts::cluster,
        .type_mask         = kAttrReportMask,
        .command_id        = WILDCARD_CMD_ID,
        .attr_id           = WILDCARD_ATTR_ID,
        .endpoint          = WILDCARD_ENDPOINT,
        .frame_flags_mask  = 0,
        .frame_flags_value = 0,
        .direction         = Direction::ServerToClient,
        .fn                = { .zcl_fn = &EnumLookup::fn },
        .user_config       = nullptr,
    };
};

// ── Configure helpers ──────────────────────────────────────────────
//
// On-device these would issue bind / reporting configuration; on the
// host they do nothing. A device pulls them by address into
// `PreparedDefinition::configure` — the platform adapter is free to
// look up the helper identity and call into its own Zigbee stack.
//
// The identity of the helper is part of its function pointer: pointer
// equality with the `k*` sentinels below is how the adapter recognises
// which canned behaviour to apply.

namespace detail {
inline void noop_configure(std::uint16_t, RuntimeContext&) {}
}  // namespace detail

// `m::identify()` → ConfigureFn stub. On-device: bind genIdentify +
// enable attr 0x0000 reporting.
constexpr ConfigureFn identify() { return &detail::noop_configure; }

// `m::bind_cluster<CLUSTER_ID>()` → ConfigureFn stub keyed by the
// cluster id. The per-cluster template parameter gives each call site a
// unique function pointer so the adapter can distinguish them.
template <std::uint16_t CLUSTER_ID>
struct BindCluster {
    static void fn(std::uint16_t, RuntimeContext&) {
        // host no-op — adapter reads CLUSTER_ID via function identity.
    }
    static constexpr std::uint16_t cluster_id = CLUSTER_ID;
};

template <std::uint16_t CLUSTER_ID>
constexpr ConfigureFn bind_cluster() { return &BindCluster<CLUSTER_ID>::fn; }

// ── DeviceEndpoints ────────────────────────────────────────────────
//
// Compile-time endpoint → label map. Devices with multiple buttons /
// relays use this to translate an inbound `src_endpoint` into a human
// name via constexpr lookup. Template-parameter shape keeps the table
// rodata-resident.

struct EndpointEntry {
    std::uint8_t endpoint;
    const char*  label;
};

template <std::size_t N>
struct DeviceEndpoints {
    std::array<EndpointEntry, N> entries;

    constexpr const char* label_for(std::uint8_t ep) const {
        for (std::size_t i = 0; i < N; ++i) {
            if (entries[i].endpoint == ep) return entries[i].label;
        }
        return nullptr;
    }
};

// ── Metadata tags ──────────────────────────────────────────────────
//
// `.meta` in a PreparedDefinition is `const void*`. Each tag below is a
// literal struct: devices stash the address of a constexpr instance
// into `def.meta` and the platform adapter fishes it back out by
// sniffing a type marker word. Keeping these as plain structs avoids a
// large class hierarchy while staying constexpr-friendly.

enum class PowerSource : std::uint8_t { Mains, Battery, BatteryRechargeable };

struct ForcePowerSource {
    PowerSource source;
};

struct ForceDeviceType {
    // Matches z2m's `Router` / `EndDevice` classification — an adapter
    // is free to surface this as a discovery hint.
    enum class Kind : std::uint8_t { Router, EndDevice } kind;
};

// `m::quirk_checkin_interval(ms)` — formalised constexpr holder for
// "the device phones home every N ms; platform should expect reports."
struct QuirkCheckinInterval {
    std::uint32_t interval_ms;
};

// `m::ota_passthrough()` — marker that OTA traffic should flow through
// the vendor's `lumiZigbeeOTA` path unchanged. No runtime behaviour on
// the host; adapter-side only.
struct OtaPassthrough {
    // Intentionally empty — presence of the tag is the signal.
};

}  // namespace zhc::m
