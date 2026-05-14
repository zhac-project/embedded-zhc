// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
// Replay harness for captured frame fixtures.
//
// Two fixture shapes:
//
//   ZCL frame (existing):
//     {
//       "model": "WXKG01LM",
//       "manufacturer": "LUMI",
//       "frame": {
//         "cluster": "0x0006",
//         "endpoint": 1,
//         "hex": "180A0A…"
//       },
//       "expected": { "state": true }
//     }
//
//   Tuya DP-stream frame:
//     {
//       "model": "TS0601_plug_bjawzodf",
//       "manufacturer": "Tuya",
//       "manufacturer_name": "_TZE200_bjawzodf",
//       "dp_frame": {
//         "dps": [
//           { "dp": 1,  "type": 1, "value_hex": "01" },
//           { "dp": 18, "type": 2, "value_hex": "000007D0" }
//         ]
//       },
//       "expected": { "state": true, "current": 2.0 }
//     }
//
// Device lookup walks `kLumiRegistry` + `kTuyaRegistry` + `kMoesRegistry`
// via `zhc::find_definition`, which accepts `manufacturer_name` — fixtures
// for Tuya TS0601 devices MUST provide it.
//
// `expected` block with `__todo__` sentinel → SKIP (runner exits 0).

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "json_mini.hpp"

#include "zhc/devices/lumi_registry.hpp"
#include "zhc/devices/moes_registry.hpp"
#include "zhc/devices/philips_registry.hpp"
#include "zhc/devices/adeo_registry.hpp"
#include "zhc/devices/develco_registry.hpp"
#include "zhc/devices/efekta_registry.hpp"
#include "zhc/devices/feibit_registry.hpp"
#include "zhc/devices/gledopto_registry.hpp"
#include "zhc/devices/heiman_registry.hpp"
#include "zhc/devices/hive_registry.hpp"
#include "zhc/devices/ikea_registry.hpp"
#include "zhc/devices/iluminize_registry.hpp"
#include "zhc/devices/innr_registry.hpp"
#include "zhc/devices/ledvance_registry.hpp"
#include "zhc/devices/legrand_registry.hpp"
#include "zhc/devices/muller_licht_registry.hpp"
#include "zhc/devices/namron_registry.hpp"
#include "zhc/devices/nue_3a_registry.hpp"
#include "zhc/devices/orvibo_registry.hpp"
#include "zhc/devices/osram_registry.hpp"
#include "zhc/devices/paulmann_registry.hpp"
#include "zhc/devices/robb_registry.hpp"
#include "zhc/devices/saswell_registry.hpp"
#include "zhc/devices/schneider_registry.hpp"
#include "zhc/devices/shinasystem_registry.hpp"
#include "zhc/devices/slacky_diy_registry.hpp"
#include "zhc/devices/sonoff_registry.hpp"
#include "zhc/devices/sunricher_registry.hpp"
#include "zhc/devices/third_reality_registry.hpp"
// Tier D
#include "zhc/devices/adurosmart_registry.hpp"
#include "zhc/devices/tier_e_registries.hpp"
#include "zhc/devices/aurora_lighting_registry.hpp"
#include "zhc/devices/bitron_registry.hpp"
#include "zhc/devices/bosch_registry.hpp"
#include "zhc/devices/candeo_registry.hpp"
#include "zhc/devices/ewelink_registry.hpp"
#include "zhc/devices/immax_registry.hpp"
#include "zhc/devices/lincukoo_registry.hpp"
#include "zhc/devices/owon_registry.hpp"
#include "zhc/devices/qa_registry.hpp"
#include "zhc/devices/sengled_registry.hpp"
#include "zhc/devices/shelly_registry.hpp"
#include "zhc/devices/sinope_registry.hpp"
#include "zhc/devices/smartthings_registry.hpp"
#include "zhc/devices/sylvania_registry.hpp"
#include "zhc/devices/yale_registry.hpp"
#include "zhc/devices/yokis_registry.hpp"
#include "zhc/devices/zemismart_registry.hpp"
#include "zhc/devices/tuya_registry.hpp"
#include "zhc/runtime/definition.hpp"
#include "zhc/runtime/definition_runtime.hpp"
#include "zhc/runtime/dispatch.hpp"
#include "zhc/runtime/store.hpp"
#include "zhc/types.hpp"
#include "zhc/cluster_names.hpp"
#include "zhc/zcl/decoder.hpp"
#include "zhc/zcl/foundation.hpp"
#include "zhc/zcl/header.hpp"

namespace fs = std::filesystem;

namespace {

// ── Combined registry view ────────────────────────────────────────
// Lumi + Tuya + Moes + Philips concatenated into a single vector at
// startup.
std::vector<const zhc::PreparedDefinition*> build_combined_registry() {
    std::vector<const zhc::PreparedDefinition*> v;
    v.reserve(zhc::devices::lumi::kLumiRegistryCount +
              zhc::devices::tuya::kTuyaRegistryCount +
              zhc::devices::moes::kMoesRegistryCount +
              zhc::devices::philips::kPhilipsRegistryCount +
              zhc::devices::sonoff::kSonoffRegistryCount);
    for (std::size_t i = 0; i < zhc::devices::lumi::kLumiRegistryCount; ++i) {
        v.push_back(zhc::devices::lumi::kLumiRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::tuya::kTuyaRegistryCount; ++i) {
        v.push_back(zhc::devices::tuya::kTuyaRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::moes::kMoesRegistryCount; ++i) {
        v.push_back(zhc::devices::moes::kMoesRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::philips::kPhilipsRegistryCount; ++i) {
        v.push_back(zhc::devices::philips::kPhilipsRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::sonoff::kSonoffRegistryCount; ++i) {
        v.push_back(zhc::devices::sonoff::kSonoffRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::heiman::kHeimanRegistryCount; ++i) {
        v.push_back(zhc::devices::heiman::kHeimanRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::saswell::kSaswellRegistryCount; ++i) {
        v.push_back(zhc::devices::saswell::kSaswellRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::efekta::kEfektaRegistryCount; ++i) {
        v.push_back(zhc::devices::efekta::kEfektaRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::schneider::kSchneiderRegistryCount; ++i) {
        v.push_back(zhc::devices::schneider::kSchneiderRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::ikea::kIkeaRegistryCount; ++i) {
        v.push_back(zhc::devices::ikea::kIkeaRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::innr::kInnrRegistryCount; ++i) {
        v.push_back(zhc::devices::innr::kInnrRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::gledopto::kGledoptoRegistryCount; ++i) {
        v.push_back(zhc::devices::gledopto::kGledoptoRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::sunricher::kSunricherRegistryCount; ++i) {
        v.push_back(zhc::devices::sunricher::kSunricherRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::namron::kNamronRegistryCount; ++i) {
        v.push_back(zhc::devices::namron::kNamronRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::ledvance::kLedvanceRegistryCount; ++i) {
        v.push_back(zhc::devices::ledvance::kLedvanceRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::osram::kOsramRegistryCount; ++i) {
        v.push_back(zhc::devices::osram::kOsramRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::adeo::kAdeoRegistryCount; ++i) {
        v.push_back(zhc::devices::adeo::kAdeoRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::legrand::kLegrandRegistryCount; ++i) {
        v.push_back(zhc::devices::legrand::kLegrandRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::shinasystem::kShinasystemRegistryCount; ++i) {
        v.push_back(zhc::devices::shinasystem::kShinasystemRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::third_reality::kThirdRealityRegistryCount; ++i) {
        v.push_back(zhc::devices::third_reality::kThirdRealityRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::robb::kRobbRegistryCount; ++i) {
        v.push_back(zhc::devices::robb::kRobbRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::paulmann::kPaulmannRegistryCount; ++i) {
        v.push_back(zhc::devices::paulmann::kPaulmannRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::orvibo::kOrviboRegistryCount; ++i) {
        v.push_back(zhc::devices::orvibo::kOrviboRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::slacky_diy::kSlackyDiyRegistryCount; ++i) {
        v.push_back(zhc::devices::slacky_diy::kSlackyDiyRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::nue_3a::kNue3aRegistryCount; ++i) {
        v.push_back(zhc::devices::nue_3a::kNue3aRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::muller_licht::kMullerLichtRegistryCount; ++i) {
        v.push_back(zhc::devices::muller_licht::kMullerLichtRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::develco::kDevelcoRegistryCount; ++i) {
        v.push_back(zhc::devices::develco::kDevelcoRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::iluminize::kIluminizeRegistryCount; ++i) {
        v.push_back(zhc::devices::iluminize::kIluminizeRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::hive::kHiveRegistryCount; ++i) {
        v.push_back(zhc::devices::hive::kHiveRegistry[i]);
    }
    for (std::size_t i = 0; i < zhc::devices::feibit::kFeibitRegistryCount; ++i) {
        v.push_back(zhc::devices::feibit::kFeibitRegistry[i]);
    }
    // Tier D.
    auto push = [&](const zhc::PreparedDefinition* const* arr, std::size_t n) {
        for (std::size_t i = 0; i < n; ++i) v.push_back(arr[i]);
    };
    push(zhc::devices::yale::kYaleRegistry,               zhc::devices::yale::kYaleRegistryCount);
    push(zhc::devices::sengled::kSengledRegistry,         zhc::devices::sengled::kSengledRegistryCount);
    push(zhc::devices::smartthings::kSmartthingsRegistry, zhc::devices::smartthings::kSmartthingsRegistryCount);
    push(zhc::devices::yokis::kYokisRegistry,             zhc::devices::yokis::kYokisRegistryCount);
    push(zhc::devices::zemismart::kZemismartRegistry,     zhc::devices::zemismart::kZemismartRegistryCount);
    push(zhc::devices::ewelink::kEwelinkRegistry,         zhc::devices::ewelink::kEwelinkRegistryCount);
    push(zhc::devices::candeo::kCandeoRegistry,           zhc::devices::candeo::kCandeoRegistryCount);
    push(zhc::devices::sylvania::kSylvaniaRegistry,       zhc::devices::sylvania::kSylvaniaRegistryCount);
    push(zhc::devices::sinope::kSinopeRegistry,           zhc::devices::sinope::kSinopeRegistryCount);
    push(zhc::devices::qa::kQaRegistry,                   zhc::devices::qa::kQaRegistryCount);
    push(zhc::devices::lincukoo::kLincukooRegistry,       zhc::devices::lincukoo::kLincukooRegistryCount);
    push(zhc::devices::bitron::kBitronRegistry,           zhc::devices::bitron::kBitronRegistryCount);
    push(zhc::devices::aurora_lighting::kAuroraLightingRegistry,
         zhc::devices::aurora_lighting::kAuroraLightingRegistryCount);
    push(zhc::devices::immax::kImmaxRegistry,             zhc::devices::immax::kImmaxRegistryCount);
    push(zhc::devices::bosch::kBoschRegistry,             zhc::devices::bosch::kBoschRegistryCount);
    push(zhc::devices::shelly::kShellyRegistry,           zhc::devices::shelly::kShellyRegistryCount);
    push(zhc::devices::owon::kOwonRegistry,               zhc::devices::owon::kOwonRegistryCount);
    push(zhc::devices::adurosmart::kAdurosmartRegistry,   zhc::devices::adurosmart::kAdurosmartRegistryCount);
    // Tier E — 323 long-tail vendors.
    for (std::size_t i = 0;
         i < zhc::devices::tier_e::kTierERegistriesCount; ++i) {
        const auto& e = zhc::devices::tier_e::kTierERegistries[i];
        for (std::size_t j = 0; j < e.count; ++j) v.push_back(e.reg[j]);
    }
    return v;
}

// ── JSON helpers ───────────────────────────────────────────────────
int hex_nib(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

std::vector<std::uint8_t> parse_hex(const std::string& s) {
    std::vector<std::uint8_t> out;
    out.reserve(s.size() / 2);
    for (std::size_t i = 0; i + 1 < s.size(); i += 2) {
        const int hi = hex_nib(s[i]);
        const int lo = hex_nib(s[i + 1]);
        if (hi < 0 || lo < 0) throw std::runtime_error("bad hex char");
        out.push_back(static_cast<std::uint8_t>((hi << 4) | lo));
    }
    return out;
}

const zhc::json::Value& required(const zhc::json::Value& v, const char* key) {
    const auto* p = v.get(key);
    if (!p) throw std::runtime_error(std::string("missing key '") + key + "'");
    return *p;
}

// Cluster-name table is shared via `zhc/cluster_names.hpp` so the host
// runner and the ESP-IDF adapter cannot drift.
using zhc::cluster_id_to_name;

struct Fixture {
    std::string       model;
    std::string       manufacturer;
    std::string       manufacturer_name;   // optional — Tuya-style match

    // Exactly one of these is populated:
    bool              has_zcl{false};
    std::uint16_t     cluster{0};
    std::uint8_t      endpoint{0};
    std::vector<std::uint8_t> hex;

    bool              has_dp{false};
    struct DpEntry {
        std::uint8_t  dp_id;
        std::uint8_t  dp_type;
        std::vector<std::uint8_t> value;
    };
    std::vector<DpEntry> dps;

    zhc::json::Value  expected;
    fs::path          path;
};

Fixture parse_fixture(const fs::path& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("cannot open " + path.string());
    std::stringstream buf;
    buf << f.rdbuf();
    const auto v = zhc::json::parse(buf.str());

    Fixture fx{};
    fx.path         = path;
    fx.model        = required(v, "model").as_string();
    fx.manufacturer = required(v, "manufacturer").as_string();
    if (const auto* m = v.get("manufacturer_name")) {
        fx.manufacturer_name = m->as_string();
    }

    if (const auto* zcl = v.get("frame")) {
        fx.has_zcl = true;
        const auto& cluster_s = required(*zcl, "cluster").as_string();
        fx.cluster  = static_cast<std::uint16_t>(std::strtoul(
            cluster_s.c_str(), nullptr, 0));
        fx.endpoint = static_cast<std::uint8_t>(required(*zcl, "endpoint").as_int());
        fx.hex      = parse_hex(required(*zcl, "hex").as_string());
    } else if (const auto* dp = v.get("dp_frame")) {
        fx.has_dp = true;
        const auto& dps = required(*dp, "dps");
        if (!dps.is_array()) throw std::runtime_error("dp_frame.dps not array");
        for (const auto& item : dps.arr) {
            Fixture::DpEntry e{};
            e.dp_id   = static_cast<std::uint8_t>(required(item, "dp").as_int());
            e.dp_type = static_cast<std::uint8_t>(required(item, "type").as_int());
            e.value   = parse_hex(required(item, "value_hex").as_string());
            fx.dps.push_back(std::move(e));
        }
    } else {
        throw std::runtime_error("fixture has neither 'frame' nor 'dp_frame'");
    }

    fx.expected = required(v, "expected");
    return fx;
}

std::vector<Fixture> load_fixtures(const fs::path& root) {
    std::vector<Fixture> out;
    if (!fs::exists(root)) return out;
    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file()) continue;
        if (entry.path().extension() != ".json") continue;
        out.push_back(parse_fixture(entry.path()));
    }
    return out;
}

// ── Diff ───────────────────────────────────────────────────────────
std::string format_value(const zhc::Value& v) {
    char buf[64];
    switch (v.type) {
        case zhc::ValueType::Bool: return v.b ? "true" : "false";
        case zhc::ValueType::Uint:
            std::snprintf(buf, sizeof(buf), "%llu", (unsigned long long)v.u);
            return buf;
        case zhc::ValueType::Int:
            std::snprintf(buf, sizeof(buf), "%lld", (long long)v.i);
            return buf;
        case zhc::ValueType::Float:
            std::snprintf(buf, sizeof(buf), "%g", (double)v.f);
            return buf;
        case zhc::ValueType::StringRef:
            return std::string("\"") + (v.str ? v.str : "") + "\"";
        case zhc::ValueType::BytesRef:
            std::snprintf(buf, sizeof(buf), "bytes(%zu)", v.bytes.size());
            return buf;
        case zhc::ValueType::ObjectRef: return "object{…}";
        case zhc::ValueType::None:      return "none";
    }
    return "?";
}

std::string diff_one(const std::string& key,
                     const zhc::json::Value& want,
                     const zhc::FixedPayload<ZHC_FIXED_PAYLOAD_CAP>& merged) {
    const zhc::Value* got = merged.find(key.c_str());
    if (!got) {
        std::string w;
        if      (want.is_string()) w = "\"" + want.as_string() + "\"";
        else if (want.is_bool())   w = want.as_bool() ? "true" : "false";
        else if (want.is_int())    w = std::to_string(want.as_int());
        else                        w = "?";
        return "want=" + w + " got=(absent)";
    }
    switch (want.kind) {
        case zhc::json::Kind::Bool:
            if (got->type == zhc::ValueType::Bool && got->b == want.as_bool())
                return {};
            break;
        case zhc::json::Kind::Int: {
            const auto w = want.as_int();
            if (got->type == zhc::ValueType::Uint && w >= 0 &&
                got->u == static_cast<std::uint64_t>(w)) return {};
            if (got->type == zhc::ValueType::Int && got->i == w) return {};
            // Also accept Float that's approximately the integer.
            if (got->type == zhc::ValueType::Float &&
                got->f == static_cast<float>(w)) return {};
            break;
        }
        case zhc::json::Kind::Float: {
            const double w = want.as_float();
            if (got->type == zhc::ValueType::Float) {
                const double d = static_cast<double>(got->f) - w;
                if (d < 0.001 && d > -0.001) return {};
            }
            if (got->type == zhc::ValueType::Int &&
                static_cast<double>(got->i) == w) return {};
            if (got->type == zhc::ValueType::Uint &&
                static_cast<double>(got->u) == w) return {};
            break;
        }
        case zhc::json::Kind::String:
            if (got->type == zhc::ValueType::StringRef && got->str &&
                std::strcmp(got->str, want.as_string().c_str()) == 0)
                return {};
            break;
        case zhc::json::Kind::Null:
        case zhc::json::Kind::Array:
        case zhc::json::Kind::Object:
            return "want=(unsupported expected type) got=" + format_value(*got);
    }
    std::string w;
    if      (want.is_string()) w = "\"" + want.as_string() + "\"";
    else if (want.is_bool())   w = want.as_bool() ? "true" : "false";
    else if (want.is_int())    w = std::to_string(want.as_int());
    else if (want.is_float())  w = std::to_string(want.as_float());
    else                        w = "?";
    return "want=" + w + " got=" + format_value(*got);
}

// ── Replay ─────────────────────────────────────────────────────────
struct ReplayOutcome {
    bool passed{false};
    bool skipped{false};
    std::string first_failure;
};

const zhc::PreparedDefinition* find_by_model_or_fingerprint(
    const Fixture& fx,
    const std::vector<const zhc::PreparedDefinition*>& reg) {
    // First try exact `.model = fx.model` — fixtures for hand-authored
    // devices carry the human-readable device model, not the wire
    // modelIdentifier. Fall back to `find_definition` which walks
    // zigbee_models + manufacturer discriminators.
    for (const auto* d : reg) {
        if (d && d->model && fx.model == d->model) return d;
    }
    std::span<const zhc::PreparedDefinition* const> view(
        reg.data(), reg.size());
    const char* manu = fx.manufacturer_name.empty() ? nullptr
                                                     : fx.manufacturer_name.c_str();
    return zhc::find_definition(fx.model.c_str(), manu, view);
}

ReplayOutcome replay(const Fixture& fx,
                      const std::vector<const zhc::PreparedDefinition*>& reg) {
    ReplayOutcome r{};

    if (!fx.expected.is_object() || fx.expected.obj.empty()) {
        r.skipped = true;
        return r;
    }
    if (fx.expected.obj.size() == 1 &&
        fx.expected.obj.find("__todo__") != fx.expected.obj.end()) {
        r.skipped = true;
        return r;
    }

    const zhc::PreparedDefinition* def = find_by_model_or_fingerprint(fx, reg);
    if (!def) {
        r.first_failure = "no definition for model '" + fx.model + "'";
        return r;
    }

    zhc::RuntimeStore<4> store{};
    zhc::RuntimeContext ctx{};
    ctx.store        = &store;
    ctx.store_get    = &zhc::RuntimeStore<4>::get;
    ctx.device_index = 0;

    zhc::DispatchResult result{};

    if (fx.has_zcl) {
        const char* cluster_name = cluster_id_to_name(fx.cluster);
        if (!cluster_name) {
            r.first_failure = "unknown cluster id";
            return r;
        }
        zhc::InboundApsFrame raw{};
        raw.cluster_id   = fx.cluster;
        raw.src_endpoint = fx.endpoint;
        raw.dst_endpoint = fx.endpoint;
        raw.linkquality  = 0xC8;
        raw.data         = std::span<const std::uint8_t>(fx.hex.data(),
                                                          fx.hex.size());
        zhc::DecodedMessage msg{};
        if (!zhc::decode_frame(raw, {}, msg)) {
            r.first_failure = "decode_frame failed";
            return r;
        }
        msg.cluster = cluster_name;

        zhc::TuyaDpRecord dp_scratch[16];
        std::size_t dp_count = 0;
        std::span<const zhc::TuyaDpRecord> dp_span{};
        if (msg.family == zhc::FrameFamily::TuyaDp) {
            zhc::ZclHeader hdr{}; std::size_t hlen = 0;
            if (zhc::parse_zcl_header(raw.data, hdr, hlen) &&
                raw.data.size() >= hlen + 2) {
                const auto body = raw.data.subspan(hlen + 2);
                zhc::parse_tuya_dp_stream(body,
                    std::span<zhc::TuyaDpRecord>(dp_scratch,
                        sizeof(dp_scratch) / sizeof(dp_scratch[0])),
                    dp_count);
                dp_span = std::span<const zhc::TuyaDpRecord>(dp_scratch, dp_count);
            }
        }
        result = zhc::dispatch_from_zigbee(msg, dp_span, *def, raw, ctx);
    } else if (fx.has_dp) {
        std::vector<zhc::TuyaDpRecord> recs;
        recs.reserve(fx.dps.size());
        for (const auto& e : fx.dps) {
            recs.push_back(zhc::TuyaDpRecord{
                e.dp_id, e.dp_type,
                std::span<const std::uint8_t>(e.value.data(), e.value.size()),
            });
        }
        zhc::InboundApsFrame raw{};
        raw.cluster_id = 0xEF00;
        raw.src_endpoint = 1; raw.dst_endpoint = 1; raw.linkquality = 0xC8;

        zhc::DecodedMessage msg{};
        msg.family       = zhc::FrameFamily::TuyaDp;
        msg.type         = zhc::MessageType::Command;
        msg.cluster      = "manuSpecificTuya";
        msg.direction    = zhc::Direction::ServerToClient;
        msg.command_id   = 0x02;
        msg.src_endpoint = 1; msg.dst_endpoint = 1;

        result = zhc::dispatch_from_zigbee(
            msg,
            std::span<const zhc::TuyaDpRecord>(recs.data(), recs.size()),
            *def, raw, ctx);
    }

    if (!result.any_matched) {
        r.first_failure = "no converter matched";
        return r;
    }

    for (const auto& kv : fx.expected.obj) {
        auto msg_diff = diff_one(kv.first, kv.second, result.merged);
        if (!msg_diff.empty()) {
            r.first_failure = kv.first + ": " + msg_diff;
            return r;
        }
    }

    r.passed = true;
    return r;
}

}  // namespace

int main(int argc, char** argv) {
    try {
        const fs::path root = argc > 1
            ? fs::path{argv[1]}
            : fs::path{"embedded/zhc/tests/fixtures"};
        const auto fixtures = load_fixtures(root);
        if (fixtures.empty()) {
            std::printf("0 fixtures found, no-op\n");
            return 0;
        }

        const auto registry = build_combined_registry();

        std::size_t passed  = 0;
        std::size_t failed  = 0;
        std::size_t skipped = 0;

        for (const auto& fx : fixtures) {
            const auto r = replay(fx, registry);
            const char* tag = r.passed  ? "PASS"
                             : r.skipped ? "SKIP"
                             : "FAIL";
            std::printf("%s  %s  model=%s",
                        tag, fx.path.c_str(), fx.model.c_str());
            if (fx.has_zcl) {
                std::printf("  cluster=0x%04X", fx.cluster);
            } else if (fx.has_dp) {
                std::printf("  dp=%zu", fx.dps.size());
            }
            if (!r.first_failure.empty()) {
                std::printf("  %s", r.first_failure.c_str());
            }
            std::printf("\n");
            if      (r.passed)  ++passed;
            else if (r.skipped) ++skipped;
            else                 ++failed;
        }
        std::printf("%zu PASS / %zu FAIL / %zu SKIP (of %zu)  "
                    "registry=%zu\n",
                    passed, failed, skipped, fixtures.size(), registry.size());
        return failed == 0 ? 0 : 1;
    } catch (const std::exception& e) {
        std::fprintf(stderr, "fixture_runner: %s\n", e.what());
        return 1;
    }
}
