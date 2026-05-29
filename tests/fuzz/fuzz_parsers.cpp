// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
//
// fuzz_parsers.cpp — libFuzzer harness for the hand-rolled ZCL/Tuya wire
// walkers in src/zcl/foundation.cpp (FINDINGS.md F31). These decode
// attacker-influenced bytes off the radio, so they are the highest-value
// fuzz target: a single byte selects a parser, the remainder is the raw
// payload. Build + run (host, clang):
//
//   cmake -B build-fuzz -DZHC_FUZZ=ON -DCMAKE_C_COMPILER=clang \
//         -DCMAKE_CXX_COMPILER=clang++ .
//   cmake --build build-fuzz --target zhc_fuzz_parsers
//   ./build-fuzz/zhc_fuzz_parsers -max_total_time=120 corpus/
//
// See tests/fuzz/README.md.
#include "zhc/zcl/foundation.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    using namespace zhc;
    if (size < 1) return 0;

    const std::uint8_t selector = data[0];
    std::span<const std::uint8_t> body(data + 1, size - 1);

    // Bounded scratch mirrors the on-device callers; ASan/UBSan catch any
    // OOB write or signed/overflow UB regardless of the (small) sizes here.
    char scratch[256];

    switch (selector & 0x03) {
        case 0: {
            FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
            (void)parse_report_attributes(body, {}, scratch, sizeof(scratch), out);
            break;
        }
        case 1: {
            FixedPayload<ZHC_FIXED_PAYLOAD_CAP> out{};
            (void)parse_read_attr_response(body, {}, scratch, sizeof(scratch), out);
            break;
        }
        case 2: {
            FixedPayload<ZHC_MI_STRUCT_CAP> arena{};
            (void)parse_mi_struct(body, scratch, sizeof(scratch), arena);
            break;
        }
        case 3: {
            TuyaDpRecord recs[16];
            std::size_t count = 0;
            (void)parse_tuya_dp_stream(body, std::span<TuyaDpRecord>(recs, 16), count);
            break;
        }
    }
    return 0;
}
