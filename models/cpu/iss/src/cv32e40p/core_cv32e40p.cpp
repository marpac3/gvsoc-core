/*
 * Copyright (C) 2020 GreenWaves Technologies, SAS, ETH Zurich and
 *                    University of Bologna
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
 * Authors: Marco Paci, Chips-it (marco.paci@chips.it)
 */

#ifdef CONFIG_GVSOC_ISS_CV32E40P

#include <vp/vp.hpp>
#include <cpu/iss/include/iss.hpp>

// Plan A: CV32E40P is M-mode only (PULP_SECURE=0 in RTL).
// RTL always sets priv_lvl_n = PRIV_LVL_M on MRET regardless of MPP
// (cv32e40p_cs_registers.sv:1069).
void Cv32e40pCore::mret_mode_restore()
{
    this->mode_set(PRIV_M);
    this->iss.csr.mstatus.mpp = PRIV_M;
}

// Plan A: CV32E40P mstatus_write_mask FPU-aware.
// D62: effective mask matches RTL always_ff (PULP_SECURE=0) —
// only MIE(3) + MPIE(7) writable; MPP forced to M by hardware.
// With FPU: add FS(14:13).
void Cv32e40pCore::mstatus_write_mask_fixup()
{
    bool fpu_in_isa = this->iss.top.get_js_config()->get_child_bool("fpu_in_isa");
    this->mstatus_write_mask = fpu_in_isa ? (iss_reg_t)0x6088 : (iss_reg_t)0x0088;
}

#endif
