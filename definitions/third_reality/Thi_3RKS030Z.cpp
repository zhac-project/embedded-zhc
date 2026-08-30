// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 3: Third Reality 3RKS030Z smart kitchen scale.
// New upstream in z2m v26.100.0; deferred at the time because the entire
// device rides one manufacturer cluster with nothing generic to reuse.
//
// Everything lives on `3rScaleSensorcluster` (0xFF0C), newly named in
// zhc/cluster_names.hpp — an unnamed cluster id decodes to a null name and
// no converter can ever match it.
//
//   attribute 0x0001 `readWeight`  INT16, grams  -> `weight`
//                                                -> `weight_pound_ounce`
//   command   0x00 reset                         (tare)
//   command   0x01 startReport
//   command   0x02 stopReport
//   command   0x03 setWeight        u8 parameter
//   command   0x04 convertGramToPoundOunce
//
// The pound/ounce string mirrors z2m exactly, including its rounding:
// total ounces = grams / 28.3495, pounds = floor(oz / 16), remainder ounces
// to two decimals, formatted "<lb>lb <oz>oz". It is built in the dispatch
// arena via ctx.alloc_str so the StringRef outlives the converter call.
//
// The five commands are exposed as write-only buttons. Four take no payload;
// `set_weight_button` carries a single u8, which bounds a manual weight entry
// to 255 — that is upstream's parameter type, not a choice made here.
//
// z2m-source: third_reality.ts #3RKS030Z (fzLocal.scale_weight).
#include "definitions/_generic/_shared.hpp"
#include "zhc/runtime/dispatch.hpp"   // RuntimeContext::alloc_str

#include <cmath>
#include <cstdint>
#include <cstdio>

namespace zhc::devices::third_reality {
namespace {

constexpr std::uint16_t kScaleCluster = 0xFF0C;

bool fz_scale_weight(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext& ctx,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    // readWeight = attribute 0x0001; the parser formats non-standard
    // attribute ids as decimal strings.
    const Value* v = msg.payload.find("1");
    if (!v) return false;

    std::int64_t gram = 0;
    if      (v->type == ValueType::Int)  gram = v->i;
    else if (v->type == ValueType::Uint) gram = static_cast<std::int64_t>(v->u);
    else return false;

    Value w{}; w.type = ValueType::Int; w.i = gram;
    out.put("weight", w);

    // z2m: totalOunces = gram / 28.3495; pound = floor(totalOunces / 16);
    //      ounce = (totalOunces % 16) to 2 decimals.
    const double total_oz = static_cast<double>(gram) / 28.3495;
    const double pounds   = std::floor(total_oz / 16.0);
    double ounces         = total_oz - pounds * 16.0;
    // Round to two decimals the same way toFixed(2) does.
    ounces = std::round(ounces * 100.0) / 100.0;

    char buf[48];
    const int n = std::snprintf(buf, sizeof(buf), "%lldlb %.2foz",
                                 static_cast<long long>(pounds), ounces);
    if (n > 0) {
        if (const char* stable = ctx.alloc_str(buf, static_cast<std::size_t>(n))) {
            Value s{}; s.type = ValueType::StringRef; s.str = stable;
            out.put("weight_pound_ounce", s);
        }
    }
    return true;
}

const FzConverter kFzScaleWeight{
    .family            = FrameFamily::Zcl,
    .cluster           = "3rScaleSensorcluster",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_scale_weight },
    .user_config       = nullptr,
};

// ── command buttons ──────────────────────────────────────────────────
//
// Each button emits a cluster-specific command on 0xFF0C. The frame is
// [FC=0x01 (cluster-specific, client->server), TSN (adapter fills), cmd]
// plus the command's parameters.
struct ScaleCmd {
    std::uint8_t id;
    bool         takes_u8;
};

constexpr ScaleCmd kCmdReset        {0x00, false};
constexpr ScaleCmd kCmdStartReport  {0x01, false};
constexpr ScaleCmd kCmdStopReport   {0x02, false};
constexpr ScaleCmd kCmdSetWeight    {0x03, true };
constexpr ScaleCmd kCmdConvertUnits {0x04, false};

bool tz_scale_command(std::string_view,
                       const Value& input,
                       const TzConverter& self,
                       const PreparedDefinition&,
                       RuntimeContext&,
                       std::span<std::uint8_t> out_frame,
                       std::size_t& out_size) {
    const auto* cmd = static_cast<const ScaleCmd*>(self.user_config);
    if (!cmd) return false;

    const std::size_t need = cmd->takes_u8 ? 4u : 3u;
    if (out_frame.size() < need) return false;

    out_frame[0] = 0x01;        // cluster-specific, client->server
    out_frame[1] = 0;           // TSN — adapter fills
    out_frame[2] = cmd->id;

    if (cmd->takes_u8) {
        // setWeight's payload is a single u8, so a manual entry is capped at
        // 255 grams by the device's own command definition.
        std::int64_t grams = 0;
        if      (input.type == ValueType::Int)   grams = input.i;
        else if (input.type == ValueType::Uint)  grams = static_cast<std::int64_t>(input.u);
        else if (input.type == ValueType::Float) grams = static_cast<std::int64_t>(input.f);
        else return false;
        if (grams < 0)   grams = 0;
        if (grams > 255) grams = 255;
        out_frame[3] = static_cast<std::uint8_t>(grams);
    }
    out_size = need;
    return true;
}

#define ZHC_SCALE_TZ(sym, keyname, spec)        \
    const TzConverter sym{                      \
        .key         = keyname,                 \
        .cluster     = "3rScaleSensorcluster",  \
        .cluster_id  = kScaleCluster,           \
        .command_id  = spec.id,                 \
        .fn          = tz_scale_command,        \
        .user_config = &spec,                   \
    }

ZHC_SCALE_TZ(kTzReset,        "reset_button",                 kCmdReset);
ZHC_SCALE_TZ(kTzStartReport,  "start_report_button",          kCmdStartReport);
ZHC_SCALE_TZ(kTzStopReport,   "stop_report_button",           kCmdStopReport);
ZHC_SCALE_TZ(kTzSetWeight,    "set_weight_button",            kCmdSetWeight);
ZHC_SCALE_TZ(kTzConvertUnits, "convert_gram_to_pound_ounce",  kCmdConvertUnits);

#undef ZHC_SCALE_TZ

const FzConverter* const kFz_3RKS030Z[] = { &kFzScaleWeight };
const TzConverter* const kTz_3RKS030Z[] = {
    &kTzReset, &kTzStartReport, &kTzStopReport, &kTzSetWeight, &kTzConvertUnits,
};

constexpr const char* kModels_3RKS030Z[] = { "3RKS030Z" };

constexpr const char* kResetOpts[]   = { "RESET" };
constexpr const char* kStartOpts[]   = { "START" };
constexpr const char* kStopOpts[]    = { "STOP" };
constexpr const char* kConvertOpts[] = { "CONVERT" };

constexpr Expose kExposes_3RKS030Z[] = {
    {"weight",             ExposeType::Numeric, Access::State, "g",
     "Current weight", nullptr, 0},
    {"weight_pound_ounce", ExposeType::String,  Access::State, nullptr,
     "Weight in pounds and ounces", nullptr, 0},
    {"reset_button",        ExposeType::Enum, Access::Set, nullptr,
     "Reset weight (tare)", kResetOpts, 1, ExposeCategory::Config},
    {"start_report_button", ExposeType::Enum, Access::Set, nullptr,
     "Start automatic weight reporting", kStartOpts, 1, ExposeCategory::Config},
    {"stop_report_button",  ExposeType::Enum, Access::Set, nullptr,
     "Stop automatic weight reporting", kStopOpts, 1, ExposeCategory::Config},
    {"set_weight_button",   ExposeType::Numeric, Access::Set, "g",
     "Manually set the weight", nullptr, 0, ExposeCategory::Config, 0, 255, 1},
    {"convert_gram_to_pound_ounce", ExposeType::Enum, Access::Set, nullptr,
     "Convert the displayed unit", kConvertOpts, 1, ExposeCategory::Config},
};

constexpr BindingSpec kBindings_3RKS030Z[] = {
    {1, kScaleCluster},
};

}  // namespace

extern const PreparedDefinition kDef_3RKS030Z{
    .zigbee_models=kModels_3RKS030Z,
    .zigbee_models_count=sizeof(kModels_3RKS030Z)/sizeof(kModels_3RKS030Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="3RKS030Z", .vendor="Third Reality",
    .meta=nullptr,
    .exposes=kExposes_3RKS030Z,
    .exposes_count=sizeof(kExposes_3RKS030Z)/sizeof(kExposes_3RKS030Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_3RKS030Z, .from_zigbee_count=sizeof(kFz_3RKS030Z)/sizeof(kFz_3RKS030Z[0]),
    .to_zigbee=kTz_3RKS030Z, .to_zigbee_count=sizeof(kTz_3RKS030Z)/sizeof(kTz_3RKS030Z[0]),
    .configure=nullptr, .on_event=nullptr,
    .bindings=kBindings_3RKS030Z,
    .bindings_count=sizeof(kBindings_3RKS030Z)/sizeof(kBindings_3RKS030Z[0]),
};

}  // namespace zhc::devices::third_reality
