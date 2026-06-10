// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Lifecontrol MCLH-08 — hand-rewritten (vendor parity sweep).
// Air quality sensor (temperature / humidity / eco2 / voc).
// z2m-source: lifecontrol.ts #MCLH-08 (custom airQuality() converter).
//
// LifeControl packs FOUR readings into ONE msTemperatureMeasurement
// (0x0402) report — a non-standard layout no generic converter handles:
//   measuredValue     (0x0000) → temperature (with a negative-value
//                                 wrap: v < -1000 → -(v+32767)*5/3)
//   minMeasuredValue  (0x0001) → humidity      (centi-%, downstream /100)
//   maxMeasuredValue  (0x0002) → eco2          (ppm, raw)
//   tolerance         (0x0003) → voc           (ppb, raw)
// The auto-generated port wired only kFzBattery, so all four air-quality
// channels were dead. Wire a vendor converter that mirrors z2m's
// airQuality() decode (raw pass-through, runtime scales temp/humidity
// /100 downstream like the generic temp/humidity converters).
#include "definitions/_generic/_shared.hpp"

#include "zhc/runtime/definition.hpp"
#include "zhc/types.hpp"

namespace zhc::devices::lifecontrol {
namespace {

// ── Vendor air-quality decoder (msTemperatureMeasurement 0x0402) ──────
bool fz_lifecontrol_air_quality(const ::zhc::DecodedMessage& msg,
                                const ::zhc::FzConverter&,
                                const ::zhc::PreparedDefinition&,
                                ::zhc::RuntimeContext&,
                                ::zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    // attr 0x0000 — measuredValue → temperature (centi-°C raw).
    if (const ::zhc::Value* v = msg.payload.find("0")) {
        std::int64_t raw = (v->type == ::zhc::ValueType::Int)
                               ? v->i
                               : static_cast<std::int64_t>(v->u);
        // z2m: v < -1000 ? -(v + 32767) * 5 / 3 : v  (still centi-°C).
        std::int64_t centi = (raw < -1000) ? (-(raw + 32767) * 5) / 3 : raw;
        ::zhc::Value t{}; t.type = ::zhc::ValueType::Int; t.i = centi;
        out.put("temperature", t); emitted = true;
    }
    // attr 0x0001 — minMeasuredValue → humidity (centi-% raw).
    if (const ::zhc::Value* v = msg.payload.find("1")) {
        ::zhc::Value h{}; h.type = ::zhc::ValueType::Int;
        h.i = (v->type == ::zhc::ValueType::Int)
                  ? v->i
                  : static_cast<std::int64_t>(v->u);
        out.put("humidity", h); emitted = true;
    }
    // attr 0x0002 — maxMeasuredValue → eco2 (ppm raw).
    if (const ::zhc::Value* v = msg.payload.find("2")) {
        ::zhc::Value e{}; e.type = ::zhc::ValueType::Uint;
        e.u = (v->type == ::zhc::ValueType::Uint)
                  ? v->u
                  : static_cast<std::uint64_t>(v->i);
        out.put("eco2", e); emitted = true;
    }
    // attr 0x0003 — tolerance → voc (ppb raw).
    if (const ::zhc::Value* v = msg.payload.find("3")) {
        ::zhc::Value c{}; c.type = ::zhc::ValueType::Uint;
        c.u = (v->type == ::zhc::ValueType::Uint)
                  ? v->u
                  : static_cast<std::uint64_t>(v->i);
        out.put("voc", c); emitted = true;
    }
    return emitted;
}

const ::zhc::FzConverter kFzLifecontrolAirQuality{
    .family            = ::zhc::FrameFamily::Zcl,
    .cluster           = "msTemperatureMeasurement",
    .type_mask         = ::zhc::type_bit(::zhc::MessageType::AttributeReport) |
                         ::zhc::type_bit(::zhc::MessageType::ReadResponse),
    .command_id        = ::zhc::WILDCARD_CMD_ID,
    .attr_id           = ::zhc::WILDCARD_ATTR_ID,
    .endpoint          = ::zhc::WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = ::zhc::Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_lifecontrol_air_quality },
    .user_config       = nullptr,
};

const FzConverter* const kFz_MCLH_08[] = {
    &::zhc::generic::kFzBattery,
    &kFzLifecontrolAirQuality,
};

constexpr const char* kModels_MCLH_08[] = { "VOC_Sensor" };

}  // namespace


constexpr Expose kAutoExposes[] = {
    {"battery",     ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"voltage",     ExposeType::Numeric, Access::State, "mV",  nullptr, nullptr, 0},
    {"temperature", ExposeType::Numeric, Access::State, "\xC2\xB0""C", nullptr, nullptr, 0},
    {"humidity",    ExposeType::Numeric, Access::State, "%",   nullptr, nullptr, 0},
    {"eco2",        ExposeType::Numeric, Access::State, "ppm", nullptr, nullptr, 0},
    {"voc",         ExposeType::Numeric, Access::State, "ppb", nullptr, nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {1, 0x0001},  // genPowerCfg (battery)
    {1, 0x0402},  // msTemperatureMeasurement (packed air-quality report)
};

extern const PreparedDefinition kDef_MCLH_08{
    .zigbee_models=kModels_MCLH_08, .zigbee_models_count=sizeof(kModels_MCLH_08)/sizeof(kModels_MCLH_08[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="MCLH-08", .vendor="Lifecontrol",
    .meta=nullptr, .exposes=kAutoExposes, .exposes_count=sizeof(kAutoExposes)/sizeof(kAutoExposes[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_MCLH_08, .from_zigbee_count=sizeof(kFz_MCLH_08)/sizeof(kFz_MCLH_08[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
.bindings=kAutoBindings,.bindings_count=sizeof(kAutoBindings)/sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::lifecontrol
