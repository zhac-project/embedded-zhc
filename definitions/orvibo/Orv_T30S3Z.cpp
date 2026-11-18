// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Tier 2: ORVIBO T30S3Z three-button scene switch (zigbeeModel
// d0d2422bbf3a4982b31ea843bfedb559), new in z2m v26.126.0..v26.133.0.
// orvibo.ts fzLocal.t30s3zButton: a raw frame on cluster 23 (0x0017) whose
// bytes are [0x19, tsn, 0x08, button, 0, 0] -> action
// top_click / middle_click / bottom_click for button 1 / 2 / 3. The check
// reads the whole frame (msg.raw_data) exactly as upstream does, frame-control
// byte included. 0x0017 has no name in cluster_names.hpp, so its frames reach
// every converter; the converter's label never equals a real cluster name, so
// named-cluster frames never reach it, and the byte pattern does the rest.
// Not ported: nothing else upstream (fromZigbee is this one converter, no
// toZigbee, no configure).
// z2m-source: orvibo.ts #T30S3Z.
#include "definitions/_generic/_shared.hpp"

namespace zhc::devices::orvibo {
namespace {

constexpr const char* kT30s3zActions[] = { "top_click", "middle_click", "bottom_click" };

bool fz_t30s3z_button(const DecodedMessage& msg,
                      const FzConverter&,
                      const PreparedDefinition&,
                      RuntimeContext&,
                      FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& out) {
    const auto d = msg.raw_data;
    if (d.size() < 6 || d[0] != 0x19 || d[2] != 0x08 || d[4] != 0 || d[5] != 0) return false;
    if (d[3] < 1 || d[3] > sizeof(kT30s3zActions)/sizeof(kT30s3zActions[0])) return false;
    Value v{}; v.type = ValueType::StringRef; v.str = kT30s3zActions[d[3] - 1];
    out.put("action", v);
    return true;
}

const FzConverter kFzT30s3zButton{
    .family            = FrameFamily::Zcl,
    .cluster           = "manuSpecificOrvibo",   // 0x0017: unnamed, see header
    .type_mask         = type_bit(MessageType::Command) | type_bit(MessageType::Raw),
    .command_id        = 0x08,
    .attr_id           = WILDCARD_ATTR_ID,
    .endpoint          = WILDCARD_ENDPOINT,
    .frame_flags_mask  = 0,
    .frame_flags_value = 0,
    .direction         = Direction::ServerToClient,   // fc 0x19 has the direction bit
    .fn                = { .zcl_fn = fz_t30s3z_button },
    .user_config       = nullptr,
};

const FzConverter* const kFz_T30S3Z[] = { &kFzT30s3zButton };

constexpr const char* kModels_T30S3Z[] = { "d0d2422bbf3a4982b31ea843bfedb559" };

constexpr Expose kExposes_T30S3Z[] = {
    {"action", ExposeType::Enum, Access::State, nullptr, nullptr,
     kT30s3zActions, sizeof(kT30s3zActions)/sizeof(kT30s3zActions[0])},
};

}  // namespace

extern const PreparedDefinition kDef_T30S3Z{
    .zigbee_models=kModels_T30S3Z, .zigbee_models_count=sizeof(kModels_T30S3Z)/sizeof(kModels_T30S3Z[0]),
    .manufacturer_name_prefix=nullptr,
    .manufacturer_names=nullptr, .manufacturer_names_count=0,
    .model="T30S3Z", .vendor="Orvibo",
    .meta=nullptr, .exposes=kExposes_T30S3Z, .exposes_count=sizeof(kExposes_T30S3Z)/sizeof(kExposes_T30S3Z[0]),
    .white_labels=nullptr, .white_labels_count=0,
    .from_zigbee=kFz_T30S3Z, .from_zigbee_count=sizeof(kFz_T30S3Z)/sizeof(kFz_T30S3Z[0]),
    .to_zigbee=nullptr, .to_zigbee_count=0,
    .configure=nullptr, .on_event=nullptr,
};

}  // namespace zhc::devices::orvibo
