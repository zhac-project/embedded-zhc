// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Woolley BSD29/BSD59 — graduated 2026-06-10 parity pass.
// Zigbee 3.0 smart plug (SONOFF-OEM, fz.on_off + manuSpecific metering).
//
// z2m-source: woolley.ts #BSD29/BSD59.
//
// 2026-06-10 parity pass — MISSING decoder/channel (gap class b).
// The generated port wired the generic kFzElectricalMeasurement
// (haElectricalMeasurement 0x0B04) for the power/voltage/current exposes.
// But this device does NOT report on 0x0B04 — z2m's `fzLocal.BSD29`
// decodes a manufacturer-specific cluster 0xFC11 (decimal 64529) with
// three vendor attributes, each scaled /1000:
//     attr 0x7006 (28678) -> power   (W)
//     attr 0x7005 (28677) -> voltage (V)
//     attr 0x7004 (28676) -> current (A)
// Wiring 0x0B04 meant every metering report was dropped on read. The
// fix re-points the metering channel at a vendor Fz that selects cluster
// "manuSpecificWoolley" (0xFC11, newly named in cluster_names.hpp) and
// decodes the three vendor attrs as Float = raw/1000, matching z2m
// exactly. The 0x0B04 binding is also dropped — z2m's configure() binds
// ONLY genOnOff + onOff reporting; the device pushes 0xFC11 metering
// unsolicited (no bind/report config).

#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::woolley {

namespace {

// ── vendor metering Fz: cluster 0xFC11 (manuSpecificWoolley) ─────────
//
// Mirrors z2m woolley.ts fzLocal.BSD29. The device reports power/voltage/
// current on a manufacturer-specific cluster as raw milli-units; divide by
// 1000 and emit as Float (matches z2m's `msg.data[key] / 1000`). Attribute
// keys are the decimal attr ids the foundation parser produces (snprintf
// "%u"): 28678 / 28677 / 28676.
bool fz_woolley_electricity(
        const ::zhc::DecodedMessage& msg,
        const ::zhc::FzConverter&,
        const ::zhc::PreparedDefinition&,
        ::zhc::RuntimeContext&,
        ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    struct Lookup { const char* attr; const char* key; };
    static constexpr Lookup kLookup[] = {
        {"28678", "power"},     // attr 0x7006
        {"28677", "voltage"},   // attr 0x7005
        {"28676", "current"},   // attr 0x7004
    };
    bool emitted = false;
    for (const auto& entry : kLookup) {
        const ::zhc::Value* v = msg.payload.find(entry.attr);
        if (!v) continue;
        double raw;
        if (v->type == ::zhc::ValueType::Uint) {
            raw = static_cast<double>(v->u);
        } else if (v->type == ::zhc::ValueType::Int) {
            raw = static_cast<double>(v->i);
        } else {
            continue;
        }
        ::zhc::Value o{};
        o.type = ::zhc::ValueType::Float;
        o.f = static_cast<float>(raw / 1000.0);
        out.put(entry.key, o);
        emitted = true;
    }
    return emitted;
}

extern const FzConverter kFzWoolleyElectricity{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "manuSpecificWoolley",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_woolley_electricity },
    .user_config       = nullptr,
};

const FzConverter* const kFz_BSD29_BSD59[] = {
    &::zhc::generic::kFzOnOff,
    // z2m fzLocal.BSD29 — manuSpecific cluster 0xFC11, NOT 0x0B04.
    &kFzWoolleyElectricity,
};
const TzConverter* const kTz_BSD29_BSD59[] = {
    &::zhc::generic::kTzOnOff,
};
constexpr const char* kModels_BSD29_BSD59[] = { "CK-BL702-SWP-01(7020)" };

}  // namespace


// --- exposes (hand-aligned to z2m: switch + power + voltage + current) ---
constexpr Expose kAutoExposes[] = {
    {"state", ExposeType::Binary, Access::StateSet, nullptr, nullptr, nullptr, 0},
    {"power", ExposeType::Numeric, Access::State, "W", nullptr, nullptr, 0},
    {"voltage", ExposeType::Numeric, Access::State, "V", nullptr, nullptr, 0},
    {"current", ExposeType::Numeric, Access::State, "A", nullptr, nullptr, 0},
};

// z2m configure(): bind genOnOff + onOff reporting only. Metering rides
// the unsolicited 0xFC11 manuSpecific reports (no bind). The phantom
// 0x0B04 binding the generator emitted is dropped.
constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0006},
};

extern const PreparedDefinition kDef_BSD29_BSD59{
    .zigbee_models=kModels_BSD29_BSD59, .zigbee_models_count=sizeof(kModels_BSD29_BSD59)/sizeof(kModels_BSD29_BSD59[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="BSD29/BSD59", .vendor="Woolley",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_BSD29_BSD59, .from_zigbee_count=sizeof(kFz_BSD29_BSD59)/sizeof(kFz_BSD29_BSD59[0]),
    .to_zigbee=kTz_BSD29_BSD59, .to_zigbee_count=sizeof(kTz_BSD29_BSD59)/sizeof(kTz_BSD29_BSD59[0]),
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::woolley
