// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Compile-time default capacities. Every value is overridable from the
// outside build (e.g. `-DZHC_FIXED_PAYLOAD_CAP=32`). These are defaults,
// not ABI promises — bumping a cap doesn't change the public API.

#ifndef ZHC_FIXED_PAYLOAD_CAP
#define ZHC_FIXED_PAYLOAD_CAP 24
#endif

#ifndef ZHC_MAX_DEVICES
#define ZHC_MAX_DEVICES 200
#endif

#ifndef ZHC_MI_STRUCT_CAP
#define ZHC_MI_STRUCT_CAP 24
#endif

#ifndef ZHC_MAX_FZ_PER_DEF
#define ZHC_MAX_FZ_PER_DEF 8
#endif

#ifndef ZHC_MAX_TZ_PER_DEF
#define ZHC_MAX_TZ_PER_DEF 8
#endif

#ifndef ZHC_MAX_ENDPOINTS_PER_DEVICE
#define ZHC_MAX_ENDPOINTS_PER_DEVICE 8
#endif
