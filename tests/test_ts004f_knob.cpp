// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Fixture: Tuya TS004F smart knob (z2m ERS-10TZBVK-AA), manu-specific port
// `kDef_TS004F_knob` (definitions/tuya/TS004F_knob.cpp).
//
// Pins the v2 expose-parity additions over the v1 hand-port:
//   1. def shape — action enum gained hue_move/hue_stop/saturation_move;
//      exposes gained action_rate + operation_mode (read+write);
//      to_zigbee + from_zigbee wired with the operation_mode + move-hue
//      converters.
//   2. kFzCommandMoveHue dispatch — lightingColorCtrl MoveHue (cmd 0x01):
//      move_mode 1 → hue_move (+action_rate), move_mode 0 → hue_stop.
//   3. kFzTuyaOperationMode dispatch — genOnOff attr 0x8004 → operation_mode.
//
// See extra/docs/TS004F_KNOB_PARITY_TICKET.md.

// Keep assert() live even under -DNDEBUG (Release) — otherwise every check
// is elided and the test reports a false pass.
#undef NDEBUG
#include <cassert>
#include <cstdint>
#include <cstring>
#include <span>

#include "zhc/types.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/zcl/decoder.hpp"
#include "definitions/_generic/_shared.hpp"
#include "definitions/tuya/_shared.hpp"

using namespace zhc;

namespace zhc::devices::tuya {
extern const PreparedDefinition kDef_TS004F_knob;
}  // namespace zhc::devices::tuya

namespace {

const Expose* find_expose(const PreparedDefinition& def, const char* name) {
    for (std::size_t i = 0; i < def.exposes_count; ++i)
        if (std::strcmp(def.exposes[i].name, name) == 0) return &def.exposes[i];
    return nullptr;
}

bool action_has(const Expose& action, const char* value) {
    for (std::uint8_t i = 0; i < action.enum_count; ++i)
        if (std::strcmp(action.enum_values[i], value) == 0) return true;
    return false;
}

}  // namespace

// ── 1. Static def-shape parity ─────────────────────────────────────────
static void test_def_shape_parity() {
    const PreparedDefinition& def = devices::tuya::kDef_TS004F_knob;

    const Expose* action = find_expose(def, "action");
    assert(action && action->type == ExposeType::Enum);
    // v2 additions
    assert(action_has(*action, "hue_move"));
    assert(action_has(*action, "hue_stop"));
    assert(action_has(*action, "saturation_move"));
    // v1 originals still present
    assert(action_has(*action, "single"));
    assert(action_has(*action, "rotate_left"));
    assert(action_has(*action, "toggle"));
    assert(action_has(*action, "color_temperature_step_up"));

    const Expose* action_rate = find_expose(def, "action_rate");
    assert(action_rate && action_rate->type == ExposeType::Numeric);

    const Expose* op_mode = find_expose(def, "operation_mode");
    assert(op_mode && op_mode->type == ExposeType::Enum);
    assert(op_mode->access == Access::StateSet);
    assert(op_mode->enum_count == 2);
    assert(std::strcmp(op_mode->enum_values[0], "command") == 0);
    assert(std::strcmp(op_mode->enum_values[1], "event") == 0);

    // write path: to_zigbee has the operation_mode converter
    assert(def.to_zigbee && def.to_zigbee_count >= 1);
    bool tz_op = false;
    for (std::size_t i = 0; i < def.to_zigbee_count; ++i)
        if (std::strcmp(def.to_zigbee[i]->key, "operation_mode") == 0) tz_op = true;
    assert(tz_op);

    // read path: from_zigbee includes the move-hue + operation-mode converters
    bool fz_hue = false, fz_op = false;
    for (std::size_t i = 0; i < def.from_zigbee_count; ++i) {
        if (def.from_zigbee[i] == &generic::kFzCommandMoveHue) fz_hue = true;
        if (def.from_zigbee[i] == &tuya::kFzTuyaOperationMode)  fz_op  = true;
    }
    assert(fz_hue && fz_op);
}

// ── 2. MoveHue command dispatch ────────────────────────────────────────
namespace {
DispatchResult dispatch_zcl(std::uint16_t cluster_id, const char* cluster_name,
                            std::span<const std::uint8_t> bytes) {
    InboundApsFrame raw{};
    raw.cluster_id   = cluster_id;
    raw.src_endpoint = 1;
    raw.dst_endpoint = 1;
    raw.linkquality  = 0xC8;
    raw.data         = bytes;

    DecodedMessage msg{};
    assert(decode_frame(raw, {}, msg));
    msg.cluster = cluster_name;
    RuntimeContext ctx{};
    return dispatch_from_zigbee(msg, {}, devices::tuya::kDef_TS004F_knob, raw, ctx);
}
}  // namespace

static void test_move_hue_actions() {
    // lightingColorCtrl (0x0300) MoveHue (cmd 0x01).
    //   ZCL: fc=0x01 (cluster-specific, C->S), tsn, cmd=0x01, [move_mode, rate]
    // move_mode 1 = move -> "hue_move"; rate echoed as action_rate.
    const std::uint8_t move[] = { 0x01, 0x0C, 0x01, 0x01, 0x50 };
    DispatchResult r1 = dispatch_zcl(0x0300, "lightingColorCtrl",
                                     std::span<const std::uint8_t>(move, sizeof(move)));
    assert(r1.any_matched);
    const Value* a1 = r1.merged.find("action");
    assert(a1 && a1->type == ValueType::StringRef);
    assert(std::strcmp(a1->str, "hue_move") == 0);
    const Value* rate = r1.merged.find("action_rate");
    assert(rate && rate->type == ValueType::Uint && rate->u == 0x50);

    // move_mode 0 = stop -> "hue_stop".
    const std::uint8_t stop[] = { 0x01, 0x0D, 0x01, 0x00, 0x00 };
    DispatchResult r2 = dispatch_zcl(0x0300, "lightingColorCtrl",
                                     std::span<const std::uint8_t>(stop, sizeof(stop)));
    assert(r2.any_matched);
    const Value* a2 = r2.merged.find("action");
    assert(a2 && a2->type == ValueType::StringRef);
    assert(std::strcmp(a2->str, "hue_stop") == 0);
}

// ── 3. operation_mode attribute report ─────────────────────────────────
// genOnOff attr 0x8004 (decimal key "32772") -> operation_mode enum.
// The DecodedMessage is built directly (the decoder's attr-report path is
// covered by test_decoder); this isolates the converter + def wiring.
static void test_operation_mode_fz() {
    struct Case { std::uint8_t raw; const char* expect; };
    const Case cases[] = { {1, "event"}, {0, "command"} };

    for (const Case& c : cases) {
        DecodedMessage msg{};
        msg.family       = FrameFamily::Zcl;
        msg.type         = MessageType::AttributeReport;
        msg.direction    = Direction::ServerToClient;
        msg.cluster      = "genOnOff";
        msg.src_endpoint = 1;
        msg.dst_endpoint = 1;
        Value v{}; v.type = ValueType::Uint; v.u = c.raw;
        msg.payload.put("32772", v);  // 0x8004

        InboundApsFrame raw{};
        raw.cluster_id   = 0x0006;
        raw.src_endpoint = 1;
        raw.dst_endpoint = 1;
        raw.linkquality  = 0xC8;

        RuntimeContext ctx{};
        DispatchResult r =
            dispatch_from_zigbee(msg, {}, devices::tuya::kDef_TS004F_knob, raw, ctx);
        assert(r.any_matched);
        const Value* op = r.merged.find("operation_mode");
        assert(op && op->type == ValueType::StringRef);
        assert(std::strcmp(op->str, c.expect) == 0);
    }
}

int main() {
    test_def_shape_parity();
    test_move_hue_actions();
    test_operation_mode_fz();
    return 0;
}
