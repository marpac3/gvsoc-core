/*
 * Copyright (C) 2020 GreenWaves Technologies, SAS, ETH Zurich and
 *                    University of Bologna
 * Copyright (C) 2026 Fondazione Chips-it
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Authors: Marco Paci, Fondazione Chips-it (marco.paci@chips.it)
 */

/*
 * Stub header mirroring memory_config.py fields as a POD struct.
 * Used with vp::Component tree_config memcpy mechanism.
 * Field names and types must match the Python cfg_field declarations.
 */

#pragma once

#include <stdint.h>

/**
 * POD struct matching Python MemoryConfig (memory_config.py):
 *   size:     int = cfg_field(default=0,     fmt="hex", dump=True)
 *   atomics:  bool = cfg_field(default=False, dump=True)
 *   latency:  int = cfg_field(default=0,     dump=True)
 *   truncate: int = cfg_field(default=True,  dump=True)
 *
 * Must remain POD — vp::Component copies it via memcpy.
 */
struct MemoryConfig {
    uint64_t size     = 0;
    bool     atomics  = false;
    int      latency  = 0;
    int      truncate = 1;
};
