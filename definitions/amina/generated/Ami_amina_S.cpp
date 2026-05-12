// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: Amina amina S — hand-rewritten.
// Amina S EV Charger.
//
// z2m-source: amina.ts #amina S.
//
// Wire format notes:
//   * Default device endpoint is 10 (TS: `endpoint: () => ({default: 10})`).
//     Single endpoint, so no `endpoint_map` is wired.
//   * `aminaControlCluster` (manu-specific cluster id 0xFEE7, manuCode
//     0x143B). Carries 10 attrs; key payload entries use the
//     decimal-stringified attr id (e.g. attr 0x02 → "2").
//   * `genLevelCtrl::currentLevel` doubles as `charge_limit` (A 6..32).
//     `charge_limit_with_on_off` issues `moveToLevelWithOnOff` (cmd 0x04).
//   * `haElectricalMeasurement::totalActivePower` produces "power" via
//     the generic Fz; the TS poll-on-power side-effect (re-reading
//     totalActiveEnergy) is a runtime concern handled at the gateway,
//     not modelled here.
//   * `alarms` (BITMAP16) is surfaced as a raw uint plus the derived
//     `alarm_active` boolean; the TS's `aminaAlarms`-string-list view
//     can't be expressed in ZHC's static expose model.

#include "definitions/_generic/_shared.hpp"

#include <cstdint>
#include <cstring>

namespace zhc::devices::amina {

namespace {

using ::zhc::Access;
using ::zhc::BindingSpec;
using ::zhc::DecodedMessage;
using ::zhc::Direction;
using ::zhc::Expose;
using ::zhc::ExposeType;
using ::zhc::FixedPayload;
using ::zhc::FrameFamily;
using ::zhc::FzConverter;
using ::zhc::MessageType;
using ::zhc::PreparedDefinition;
using ::zhc::RuntimeContext;
using ::zhc::TzConverter;
using ::zhc::Value;
using ::zhc::ValueType;
using ::zhc::generic::ZclWriteSpec;
using ::zhc::type_bit;

constexpr std::uint16_t kAminaCluster = 0xFEE7;
constexpr std::uint16_t kAminaMfgCode = 0x143B;

// aminaControlCluster attribute ids — mirror amina.ts/aminaControlAttributes.
constexpr std::uint16_t kAttrAlarms             = 0x02;
constexpr std::uint16_t kAttrEvStatus           = 0x03;
constexpr std::uint16_t kAttrSinglePhase        = 0x05;
constexpr std::uint16_t kAttrOfflineCurrent     = 0x06;
constexpr std::uint16_t kAttrOfflineSinglePhase = 0x07;
constexpr std::uint16_t kAttrTimeToOffline      = 0x08;
constexpr std::uint16_t kAttrEnableOffline      = 0x09;
constexpr std::uint16_t kAttrTotalActiveEnergy  = 0x10;
constexpr std::uint16_t kAttrLastSessionEnergy  = 0x11;

// ── fz_amina_ev_status — aminaControlCluster attr 0x03 (BITMAP16) ───
//
// Mirrors amina.ts `fzLocal.ev_status`:
//   bit 0 → ev_connected
//   bit 1 → ready_to_charge (drives ev_status text only)
//   bit 2 → charging
//   bit 3 → charging_paused (drives ev_status text only)
//   bit 4 → derated
// We never publish on `readResponse` for the ev_status text (TS only
// emits text on `attributeReport`); the booleans are safe on either.
bool fz_amina_ev_status(const DecodedMessage& msg, const FzConverter&,
                        const PreparedDefinition&, RuntimeContext&,
                        FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("3");        // attr 0x0003
    if (!v) return false;
    std::uint64_t s = 0;
    if      (v->type == ValueType::Uint) s = v->u;
    else if (v->type == ValueType::Int)  s = static_cast<std::uint64_t>(v->i);
    else return false;

    const bool ev_connected = (s & (1u << 0)) != 0;
    const bool charging     = (s & (1u << 2)) != 0;
    const bool derated      = (s & (1u << 4)) != 0;

    Value vc{};  vc.type = ValueType::Bool; vc.b = ev_connected;
    out.put("ev_connected", vc);
    Value vh{};  vh.type = ValueType::Bool; vh.b = charging;
    out.put("charging", vh);
    Value vd{};  vd.type = ValueType::Bool; vd.b = derated;
    out.put("derated", vd);

    if (msg.type == MessageType::AttributeReport) {
        const char* status = "Not connected";
        if (ev_connected)              status = "EV connected";
        if ((s & (1u << 1)) != 0)      status = "Ready to charge";
        if (charging)                  status = "Charging";
        if ((s & (1u << 3)) != 0)      status = "Charging Paused";
        // The ", Derated" suffix from TS is dropped — emitting a single
        // canonical label keeps the SPA's text widget honest; the
        // `derated` boolean above already exposes the modifier.
        Value vs{};  vs.type = ValueType::StringRef; vs.str = status;
        out.put("ev_status", vs);
    }
    return true;
}

// ── fz_amina_alarms — aminaControlCluster attr 0x02 (BITMAP16) ──────
//
// Emits `alarm_active` (Bool) plus the raw bitmap as `alarms` (Uint).
// The named-bit list expose from TS isn't expressible in ZHC's static
// expose model; rules can mask the raw value if they need bit-level
// reasoning.
bool fz_amina_alarms(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("2");        // attr 0x0002
    if (!v) return false;
    std::uint64_t s = 0;
    if      (v->type == ValueType::Uint) s = v->u;
    else if (v->type == ValueType::Int)  s = static_cast<std::uint64_t>(v->i);
    else return false;

    Value va{};  va.type = ValueType::Bool; va.b = (s != 0);
    out.put("alarm_active", va);
    Value vr{};  vr.type = ValueType::Uint;  vr.u = s;
    out.put("alarms", vr);
    return true;
}

// ── fz_amina_energy — aminaControlCluster attrs 0x10 / 0x11 (UINT32) ─
//
// totalActiveEnergy (0x10) → "energy"
// lastSessionEnergy (0x11) → "last_session_energy"
// Raw value forwarded as Uint; SPA presentation does the /1000 → kWh
// formatting per the expose's unit string.
bool fz_amina_energy(const DecodedMessage& msg, const FzConverter&,
                     const PreparedDefinition&, RuntimeContext&,
                     FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool emitted = false;
    if (const Value* v = msg.payload.find("16")) {           // 0x10
        if (v->type == ValueType::Uint) {
            Value e{};  e.type = ValueType::Uint;  e.u = v->u;
            out.put("energy", e);
            emitted = true;
        }
    }
    if (const Value* v = msg.payload.find("17")) {           // 0x11
        if (v->type == ValueType::Uint) {
            Value e{};  e.type = ValueType::Uint;  e.u = v->u;
            out.put("last_session_energy", e);
            emitted = true;
        }
    }
    return emitted;
}

// ── fz_amina_offline_cfg — manuSpec config attrs ────────────────────
//
// Single fan-out converter for the writable config attrs that come
// back via readAttributesResponse (singlePhase / offlineCurrent /
// offlineSinglePhase / timeToOffline / enableOffline). Returns true
// when at least one of them was emitted.
bool fz_amina_offline_cfg(const DecodedMessage& msg, const FzConverter&,
                          const PreparedDefinition&, RuntimeContext&,
                          FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    bool any = false;
    auto pull = [&](const char* dec_key, const char* expose_key) {
        if (const Value* v = msg.payload.find(dec_key)) {
            if (v->type == ValueType::Uint) {
                Value e{};  e.type = ValueType::Uint;  e.u = v->u;
                out.put(expose_key, e);
                any = true;
            }
        }
    };
    pull("5",  "single_phase");          // 0x05
    pull("6",  "offline_current");       // 0x06
    pull("7",  "offline_single_phase");  // 0x07
    pull("8",  "time_to_offline");       // 0x08
    pull("9",  "enable_offline");        // 0x09
    return any;
}

// ── fz_amina_level — genLevelCtrl::currentLevel → "charge_limit" ────
bool fz_amina_level(const DecodedMessage& msg, const FzConverter&,
                    const PreparedDefinition&, RuntimeContext&,
                    FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const Value* v = msg.payload.find("0");   // attr 0x0000 currentLevel
    if (!v || v->type != ValueType::Uint) return false;
    Value e{};  e.type = ValueType::Uint;  e.u = v->u;
    out.put("charge_limit", e);
    return true;
}

// FzConverter records ────────────────────────────────────────────────

extern const FzConverter kFzAminaEvStatus{
    .family            = FrameFamily::Zcl,
    .cluster           = "aminaControlCluster",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_amina_ev_status },
    .user_config       = nullptr,
};

extern const FzConverter kFzAminaAlarms{
    .family            = FrameFamily::Zcl,
    .cluster           = "aminaControlCluster",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_amina_alarms },
    .user_config       = nullptr,
};

extern const FzConverter kFzAminaEnergy{
    .family            = FrameFamily::Zcl,
    .cluster           = "aminaControlCluster",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_amina_energy },
    .user_config       = nullptr,
};

extern const FzConverter kFzAminaOfflineCfg{
    .family            = FrameFamily::Zcl,
    .cluster           = "aminaControlCluster",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_amina_offline_cfg },
    .user_config       = nullptr,
};

extern const FzConverter kFzAminaLevel{
    .family            = FrameFamily::Zcl,
    .cluster           = "genLevelCtrl",
    .type_mask         = type_bit(MessageType::AttributeReport) |
                         type_bit(MessageType::ReadResponse),
    .command_id        = WILDCARD_CMD_ID,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,
    .fn                = { .zcl_fn = fz_amina_level },
    .user_config       = nullptr,
};

// ── Tz: charge_limit + charge_limit_with_on_off ─────────────────────
//
// Both target genLevelCtrl (0x0008). `charge_limit` issues `moveToLevel`
// (cmd 0x00); `charge_limit_with_on_off` issues `moveToLevelWithOnOff`
// (cmd 0x04). Frame body: u8 level | u16 trans | u8 optionsMask |
// u8 optionsOverride. Total 5 bytes payload after the 3-byte ZCL header.

bool tz_amina_level_cmd(std::string_view key, const Value& input,
                        const TzConverter& self,
                        const PreparedDefinition&, RuntimeContext&,
                        std::span<std::uint8_t> out_frame,
                        std::size_t& out_size) {
    out_size = 0;
    if (key != self.key) return false;
    if (input.type != ValueType::Uint) return false;
    if (input.u > 0xFF) return false;

    constexpr std::size_t kHdr     = 3;   // fc | tsn | cmd
    constexpr std::size_t kBodyLen = 5;
    if (out_frame.size() < kHdr + kBodyLen) return false;

    out_frame[0] = 0x10;                                 // fc cluster-cmd
    out_frame[1] = 0x00;                                 // tsn placeholder
    out_frame[2] = static_cast<std::uint8_t>(self.command_id & 0xFF);
    out_frame[3] = static_cast<std::uint8_t>(input.u);   // level
    out_frame[4] = 0x00;                                 // transtime LSB
    out_frame[5] = 0x00;                                 // transtime MSB
    out_frame[6] = 0x00;                                 // optionsMask
    out_frame[7] = 0x00;                                 // optionsOverride
    out_size = kHdr + kBodyLen;
    return true;
}

extern const TzConverter kTzAminaChargeLimit{
    .key         = "charge_limit",
    .cluster     = "genLevelCtrl",
    .cluster_id  = 0x0008,
    .command_id  = 0x00,           // moveToLevel
    .fn          = tz_amina_level_cmd,
    .user_config = nullptr,
};

extern const TzConverter kTzAminaChargeLimitWithOnOff{
    .key         = "charge_limit_with_on_off",
    .cluster     = "genLevelCtrl",
    .cluster_id  = 0x0008,
    .command_id  = 0x04,           // moveToLevelWithOnOff
    .fn          = tz_amina_level_cmd,
    .user_config = nullptr,
};

// ── ZclWriteSpec instances for manu-specific config writes ──────────
//
// All target aminaControlCluster (0xFEE7), manufacturer code 0x143B,
// command 0x02 (writeAttributes). Types per amina.ts custom-cluster
// declaration: singlePhase u8, offlineCurrent u8, offlineSinglePhase u8,
// timeToOffline u16, enableOffline u8.

constexpr ZclWriteSpec kSpecSinglePhase{
    "single_phase",         kAttrSinglePhase,        0x20, kAminaMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecOfflineCurrent{
    "offline_current",      kAttrOfflineCurrent,     0x20, kAminaMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecOfflineSinglePhase{
    "offline_single_phase", kAttrOfflineSinglePhase, 0x20, kAminaMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecTimeToOffline{
    "time_to_offline",      kAttrTimeToOffline,      0x21, kAminaMfgCode, nullptr, 0,
};
constexpr ZclWriteSpec kSpecEnableOffline{
    "enable_offline",       kAttrEnableOffline,      0x20, kAminaMfgCode, nullptr, 0,
};

#define ZHC_AMINA_TZ(var, spec_ref, key_str)                          \
    extern const TzConverter var{                                     \
        .key         = key_str,                                       \
        .cluster     = "aminaControlCluster",                         \
        .cluster_id  = kAminaCluster,                                 \
        .command_id  = 0x02,                                          \
        .fn          = &::zhc::generic::tz_zcl_write_attr,            \
        .user_config = &spec_ref,                                     \
    }
ZHC_AMINA_TZ(kTzAminaSinglePhase,        kSpecSinglePhase,        "single_phase");
ZHC_AMINA_TZ(kTzAminaOfflineCurrent,     kSpecOfflineCurrent,     "offline_current");
ZHC_AMINA_TZ(kTzAminaOfflineSinglePhase, kSpecOfflineSinglePhase, "offline_single_phase");
ZHC_AMINA_TZ(kTzAminaTimeToOffline,      kSpecTimeToOffline,      "time_to_offline");
ZHC_AMINA_TZ(kTzAminaEnableOffline,      kSpecEnableOffline,      "enable_offline");
#undef ZHC_AMINA_TZ

// ── Converter tables ────────────────────────────────────────────────

const FzConverter* const kFz_amina_S[] = {
    &::zhc::generic::kFzOnOff,                  // genOnOff::onOff → "state"
    &::zhc::generic::kFzElectricalMeasurement,  // totalActivePower → "power"
    &kFzAminaLevel,                             // currentLevel → "charge_limit"
    &kFzAminaEvStatus,
    &kFzAminaAlarms,
    &kFzAminaEnergy,
    &kFzAminaOfflineCfg,
};

const TzConverter* const kTz_amina_S[] = {
    &::zhc::generic::kTzOnOff,
    &kTzAminaChargeLimit,
    &kTzAminaChargeLimitWithOnOff,
    &kTzAminaSinglePhase,
    &kTzAminaOfflineCurrent,
    &kTzAminaOfflineSinglePhase,
    &kTzAminaTimeToOffline,
    &kTzAminaEnableOffline,
};

constexpr const char* kModels_amina_S[] = { "amina S" };

constexpr Expose kAutoExposes[] = {
    {"state",                    ExposeType::Binary,  Access::StateSet, nullptr, "Charger relay",                        nullptr, 0},
    {"power",                    ExposeType::Numeric, Access::State,    "W",     "Total active power",                   nullptr, 0},
    {"energy",                   ExposeType::Numeric, Access::State,    "kWh",   "Cumulative active energy",             nullptr, 0},
    {"last_session_energy",      ExposeType::Numeric, Access::State,    "kWh",   "Energy delivered last charge session", nullptr, 0},
    {"charge_limit",             ExposeType::Numeric, Access::StateSet, "A",     "Maximum allowed amperage draw",        nullptr, 0},
    {"charge_limit_with_on_off", ExposeType::Numeric, Access::StateSet, "A",     "Charge amperage + on/off",             nullptr, 0},
    {"ev_status",                ExposeType::String,  Access::State,    nullptr, "Current charging status",              nullptr, 0},
    {"ev_connected",             ExposeType::Binary,  Access::State,    nullptr, "EV is connected to the charger",       nullptr, 0},
    {"charging",                 ExposeType::Binary,  Access::State,    nullptr, "Power is being delivered",             nullptr, 0},
    {"derated",                  ExposeType::Binary,  Access::State,    nullptr, "Charging derated due to temperature",  nullptr, 0},
    {"alarm_active",             ExposeType::Binary,  Access::State,    nullptr, "An active alarm is present",           nullptr, 0},
    {"alarms",                   ExposeType::Numeric, Access::State,    nullptr, "Active alarm bitmap",                  nullptr, 0},
    {"single_phase",             ExposeType::Binary,  Access::StateSet, nullptr, "Force single-phase charging",          nullptr, 0},
    {"offline_current",          ExposeType::Numeric, Access::StateSet, "A",     "Offline-mode current limit",           nullptr, 0},
    {"offline_single_phase",     ExposeType::Binary,  Access::StateSet, nullptr, "Offline-mode single-phase",            nullptr, 0},
    {"time_to_offline",          ExposeType::Numeric, Access::StateSet, "s",     "Seconds until offline mode kicks in",  nullptr, 0},
    {"enable_offline",           ExposeType::Binary,  Access::StateSet, nullptr, "Enable offline mode on link loss",     nullptr, 0},
};

constexpr BindingSpec kAutoBindings[] = {
    {10, 0x0006},          // genOnOff
    {10, 0x0008},          // genLevelCtrl
    {10, kAminaCluster},   // aminaControlCluster
    {10, 0x0B04},          // haElectricalMeasurement
};

}  // namespace

extern const PreparedDefinition kDef_amina_S{
    .zigbee_models            = kModels_amina_S,
    .zigbee_models_count      = sizeof(kModels_amina_S) / sizeof(kModels_amina_S[0]),
    .manufacturer_name_prefix = nullptr,
    .manufacturer_names       = nullptr,
    .manufacturer_names_count = 0,
    .model                    = "amina S",
    .vendor                   = "Amina Distribution AS",
    .meta                     = nullptr,
    .exposes                  = kAutoExposes,
    .exposes_count            = sizeof(kAutoExposes) / sizeof(kAutoExposes[0]),
    .white_labels             = nullptr,
    .white_labels_count       = 0,
    .from_zigbee              = kFz_amina_S,
    .from_zigbee_count        = sizeof(kFz_amina_S) / sizeof(kFz_amina_S[0]),
    .to_zigbee                = kTz_amina_S,
    .to_zigbee_count          = sizeof(kTz_amina_S) / sizeof(kTz_amina_S[0]),
    .configure                = nullptr,
    .on_event                 = nullptr,
    .bindings                 = kAutoBindings,
    .bindings_count           = sizeof(kAutoBindings) / sizeof(kAutoBindings[0]),
};

}  // namespace zhc::devices::amina
