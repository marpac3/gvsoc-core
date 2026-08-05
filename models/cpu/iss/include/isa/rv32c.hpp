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
 * Authors: Germain Haugou, GreenWaves Technologies (germain.haugou@greenwaves-technologies.com)
 */

#ifndef __CPU_ISS_RV32C_HPP
#define __CPU_ISS_RV32C_HPP

#ifdef CONFIG_GVSOC_ISS_V2
#include "cpu/iss/include/isa_lib/int.h"
#include "cpu/iss_v2/include/isa_lib/macros.h"
#else
#include "cpu/iss/include/iss_core.hpp"
#include "cpu/iss/include/isa_lib/int.h"
#include "cpu/iss/include/isa_lib/macros.h"
#endif

static inline iss_reg_t c_unimp_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->exception.raise(pc, ISS_EXCEPT_ILLEGAL);
    return pc;
}

// The ISA table decodes the reserved RVC encodings with the regular handlers.
// With CONFIG_GVSOC_ISS_RVC_STRICT these handlers call c_reserved_exec, which
// raises an illegal instruction exception as iss_exec_insn_illegal does.
static inline iss_reg_t c_reserved_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->decode.trace.msg("Executing illegal instruction\n");
    iss->exception.raise(pc, ISS_EXCEPT_ILLEGAL);
#ifdef CONFIG_GVSOC_ISS_V2
    iss->exec.insn_stall();
#endif
    return pc;
}

static inline iss_reg_t c_addi4spn_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x1FE0) == 0)  /* nzuimm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_addi4spn_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x1FE0) == 0)  /* nzuimm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    iss->timing.event_rvc_account(1);
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_lw_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return lw_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_lw_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return lw_exec(iss, insn, pc);
}

static inline iss_reg_t c_sw_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return sw_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_sw_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return sw_exec(iss, insn, pc);
}

static inline iss_reg_t c_swsp_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return sw_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_swsp_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return sw_exec(iss, insn, pc);
}

static inline iss_reg_t c_nop_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return nop_exec(iss, insn, pc);
}

static inline iss_reg_t c_nop_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return nop_exec(iss, insn, pc);
}

static inline iss_reg_t c_addi_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_addi_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_jal_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return jal_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_jal_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return jal_exec(iss, insn, pc);
}

static inline iss_reg_t c_li_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_li_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_addi16sp_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x107C) == 0)  /* nzimm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_addi16sp_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x107C) == 0)  /* nzimm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    iss->timing.event_rvc_account(1);
    return addi_exec(iss, insn, pc);
}

static inline iss_reg_t c_jalr_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return jalr_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_jalr_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return jalr_exec(iss, insn, pc);
}

static inline iss_reg_t c_lui_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x107C) == 0)  /* imm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    return lui_exec(iss, insn, pc);
}

static inline iss_reg_t c_lui_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x107C) == 0)  /* imm == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    iss->timing.event_rvc_account(1);
    return lui_exec(iss, insn, pc);
}

static inline iss_reg_t c_srli_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return srli_exec(iss, insn, pc);
}

static inline iss_reg_t c_srli_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return srli_exec(iss, insn, pc);
}

static inline iss_reg_t c_srai_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return srai_exec(iss, insn, pc);
}

static inline iss_reg_t c_srai_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return srai_exec(iss, insn, pc);
}

static inline iss_reg_t c_andi_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return andi_exec(iss, insn, pc);
}

static inline iss_reg_t c_andi_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return andi_exec(iss, insn, pc);
}

static inline iss_reg_t c_sub_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return sub_exec(iss, insn, pc);
}

static inline iss_reg_t c_sub_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return sub_exec(iss, insn, pc);
}

static inline iss_reg_t c_xor_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return xor_exec(iss, insn, pc);
}

static inline iss_reg_t c_xor_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return xor_exec(iss, insn, pc);
}

static inline iss_reg_t c_or_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return or_exec(iss, insn, pc);
}

static inline iss_reg_t c_or_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return or_exec(iss, insn, pc);
}

static inline iss_reg_t c_and_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return and_exec(iss, insn, pc);
}

static inline iss_reg_t c_and_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return and_exec(iss, insn, pc);
}

static inline iss_reg_t c_j_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return jal_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_j_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return jal_exec(iss, insn, pc);
}

static inline iss_reg_t c_beqz_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return beq_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_beqz_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return beq_exec(iss, insn, pc);
}

static inline iss_reg_t c_bnez_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return bne_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_bnez_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return bne_exec(iss, insn, pc);
}

static inline iss_reg_t c_slli_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return slli_exec(iss, insn, pc);
}

static inline iss_reg_t c_slli_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return slli_exec(iss, insn, pc);
}

static inline iss_reg_t c_lwsp_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x0F80) == 0)  /* rd == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    return lw_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_lwsp_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x0F80) == 0)  /* rd == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    iss->timing.event_rvc_account(1);
    return lw_exec(iss, insn, pc);
}

static inline iss_reg_t c_jr_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x0F80) == 0)  /* rs1 == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    return jalr_exec_fast(iss, insn, pc);
}

static inline iss_reg_t c_jr_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    if ((insn->opcode & 0x0F80) == 0)  /* rs1 == 0 is reserved */
        return c_reserved_exec(iss, insn, pc);
#endif
    iss->timing.event_rvc_account(1);
    return jalr_exec(iss, insn, pc);
}

static inline iss_reg_t c_mv_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return add_exec(iss, insn, pc);
}

static inline iss_reg_t c_mv_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return add_exec(iss, insn, pc);
}

static inline iss_reg_t c_add_exec_fast(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    return add_exec(iss, insn, pc);
}

static inline iss_reg_t c_add_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);
    return add_exec(iss, insn, pc);
}

static inline iss_reg_t c_ebreak_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
    iss->timing.event_rvc_account(1);

    // Same as ebreak_exec in rv32i.hpp, except for the semihosting check, which
    // is only defined on the 32-bit encoding.
    if (iss->exec.debug_mode)
    {
        // An ebreak in debug mode goes back to the debug handler.
        return iss->irq.debug_handler;
    }
    iss->exception.raise(pc, ISS_EXCEPT_BREAKPOINT);
    return pc;
}

static inline iss_reg_t c_sbreak_exec(Iss *iss, iss_insn_t *insn, iss_reg_t pc)
{
#ifdef CONFIG_GVSOC_ISS_RVC_STRICT
    // 0x8002 is c.jr with rs1 = 0, a reserved encoding. The ISA table decodes
    // it as the PULP c.sbreak, so c_jr_exec never checks it and the trap must
    // be raised here.
    return c_reserved_exec(iss, insn, pc);
#else
    iss->timing.event_rvc_account(1);
    // iss->dbgunit.set_halt_mode(true, 3);
    return iss_insn_next(iss, insn, pc);
#endif
}

#endif
