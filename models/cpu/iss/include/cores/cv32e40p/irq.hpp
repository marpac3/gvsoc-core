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

#pragma once

#include <cpu/iss/include/irq/irq_riscv.hpp>

class Cv32e40pIrq : public Irq
{
public:
    Cv32e40pIrq(Iss &iss) : Irq(iss) {}

    bool mip_access(bool is_write, iss_reg_t &value) override;
    bool mie_access(bool is_write, iss_reg_t &value) override;
    bool mtvec_access(bool is_write, iss_reg_t &value) override;
    void elw_irq_unstall() override;

    /* pre-fetch IRQ check in fast handler.  Without this hook the
     * fast handler would only check IRQs post-execute, causing mepc off-by-one. */
    bool check_pre_fetch_fast() override;

    /* post-instruction IRQ check in slow handler. Returns IRQ-taken 
     * status so caller can early-return and
     * defer next-instruction fetch to next cycle. */
    bool check_post_instr_slow() override;

protected:
    void register_csr_callbacks() override;
};
