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


#pragma once

#include <cpu/iss/include/types.hpp>

class Csr;

typedef struct
{
    union
    {
        struct
        {
            unsigned int fflags : 5;
            unsigned int frm : 3;
        };
        iss_reg_t raw;
    };
} __attribute__((packed)) iss_fcsr_t;

class CsrAbtractReg
{
    friend class Csr;

public:
    CsrAbtractReg(iss_reg_t *value=NULL);
    void register_callback(std::function<bool(bool, iss_reg_t &)> callback);
    bool access(bool is_write, iss_reg_t &value);
    virtual bool check_access(Iss *iss, bool write, bool read);
    virtual iss_reg_t handle(Iss *iss, iss_insn_t *insn, iss_reg_t pc, iss_reg_t reg_value);

    const char *name;
    iss_reg_t reset_val;
    iss_reg_t write_mask;
    bool write_illegal = false;

    // Public setter for write_mask — allows core-specific subclasses to
    // configure CSR masks without requiring friend access.
    void set_write_mask(iss_reg_t mask) { write_mask = mask; }

protected:
    void reset(bool active);

private:
    std::vector<std::function<bool(bool, iss_reg_t &)>> callbacks;
    iss_reg_t default_value;
    iss_reg_t *value_p;
};

class CsrReg : public CsrAbtractReg
{
    friend class Csr;

public:
    CsrReg() : CsrAbtractReg(&this->value) {}

    iss_reg_t value;
};

class Cycle : public CsrAbtractReg
{
public:
    bool check_access(Iss *iss, bool write, bool read);
};

class Mstatus : public CsrAbtractReg
{
    public:
        Mstatus() : CsrAbtractReg(&this->value) {}
        bool check_access(Iss *iss, bool write, bool read);
        union
        {
            iss_reg_t value;
            struct
            {
#if ISS_REG_WIDTH == 32
                unsigned int uie:1;
                unsigned int sie:1;
                unsigned int reserved0:1;
                unsigned int mie:1;
                unsigned int upie:1;
                unsigned int spie:1;
                unsigned int reserved1:1;
                unsigned int mpie:1;
                unsigned int spp:1;
                unsigned int reserved2:2;
                unsigned int mpp:2;
                unsigned int fs:2;
                unsigned int xs:2;
                unsigned int mprv:1;
                unsigned int sum:1;
                unsigned int mxr:1;
                unsigned int tvm:1;
                unsigned int tw:1;
                unsigned int tsr:1;
                unsigned int reserved3:8;
                unsigned int sd:1;
#else
                unsigned int uie:1;
                unsigned int sie:1;
                unsigned int reserved0:1;
                unsigned int mie:1;
                unsigned int upie:1;
                unsigned int spie:1;
                unsigned int reserved1:1;
                unsigned int mpie:1;
                unsigned int spp:1;
                unsigned int reserved2:2;
                unsigned int mpp:2;
                unsigned int fs:2;
                unsigned int xs:2;
                unsigned int mprv:1;
                unsigned int sum:1;
                unsigned int mxr:1;
                unsigned int tvm:1;
                unsigned int tw:1;
                unsigned int tsr:1;
                unsigned int reserved3:9;
                unsigned int uxl:2;
                unsigned int sxl:2;
                unsigned int reserved4:27;
                unsigned int sd:1;
#endif
            };
        };
};



class Csr
{
public:
    Csr(Iss &iss);

    void build();
    void reset(bool active);
    virtual ~Csr() = default;

    void declare_pcer(int index, std::string name, std::string help);
    void declare_csr(CsrAbtractReg *reg, std::string name, iss_reg_t address, iss_reg_t reset_val=0, iss_reg_t mask=-1);
    CsrAbtractReg *get_csr(iss_reg_t address);

    bool access(bool is_write, iss_reg_t address, iss_reg_t &value);

    Iss &iss;

    vp::Trace trace;

    Cycle cycle;
    CsrReg time;
    CsrReg instret;

    CsrReg sstatus;
    CsrReg sie;
    CsrReg stvec;
    CsrReg scounteren;

    CsrReg  sscratch;
    CsrReg  sepc;
    CsrReg  scause;
    CsrReg  stval;
    CsrReg  sip;

    CsrReg  satp;

    Mstatus mstatus;
    CsrReg misa;
    CsrReg  medeleg;
    CsrReg  mideleg;
    CsrReg  mie;
    CsrReg  mtvec;
    CsrReg  mcounteren;

    CsrReg  mscratch;
    CsrReg  mepc;
    CsrReg  mcause;
    CsrReg  mtval;
    CsrReg  mip;

    CsrReg tselect;
    CsrReg tdata1;
    CsrReg tdata2;
    CsrReg tdata3;

    CsrReg mvendorid;
    CsrReg marchid;

    CsrReg mnscratch;
    CsrReg mnepc;
    CsrReg mncause;
    CsrReg mnstatus;

    CsrReg mcycle;
    CsrReg mhpmcounter[29];
#if ISS_REG_WIDTH == 32
    CsrReg mhpmcounterh[29];
#endif
    CsrReg mcountinhibit;

    // Plan A: declared unconditionally so Csr::build() can declare them
    // without #ifdef.  Core-specific callbacks/init in subclass build_*().
#if ISS_REG_WIDTH == 32
    CsrReg mcycleh;
    CsrReg minstreth;
#endif

    CsrReg minstret;
    CsrReg mhpmevent[29];

    CsrReg tinfo;
    CsrReg mcontext;
    CsrReg scontext;

#if defined(CONFIG_GVSOC_ISS_PMP)
    CsrReg pmpcfg[16];
    CsrReg pmpaddr[64];
#endif

    iss_reg_t depc;
    iss_reg_t dcsr;
#if defined(ISS_HAS_PERF_COUNTERS)
    iss_reg_t pccr[32];
    iss_reg_t pcer;
    iss_reg_t pcmr;
#endif

#if defined(CONFIG_GVSOC_ISS_STACK_CHECKER)
    iss_reg_t stack_conf;
    iss_reg_t stack_start;
    iss_reg_t stack_end;
#endif

    iss_reg_t scratch0;
    iss_reg_t scratch1;
    iss_fcsr_t fcsr;
    
    CsrReg mhartid;
    CsrReg mimpid;
    CsrReg vstart;
    CsrReg vxstat;
    CsrReg vxrm;
    CsrReg vcsr;
    CsrReg vl;
    CsrReg vtype;
    CsrReg vlenb;

#if defined(CONFIG_GVSOC_ISS_RI5KY) || defined(CONFIG_GVSOC_ISS_HWLOOP)
    iss_reg_t hwloop_regs[HWLOOP_NB_REGS];
#endif

    /* Hook for core-specific mstatus read fixup (e.g. SD bit).
     * Called by Core::mstatus_update on read path. Default: no-op. */
    virtual void mstatus_read_fixup(iss_reg_t &value) {}

    /* Plan A hook: FP/Vector CSR access pre-check.
     * Default: allow (returns false → not illegal).
     * Cv32e40pCsr overrides: returns true when mstatus[FS]==00 (Off).
     * Called from static fflags/frm/fcsr read/write to detect illegal access. */
    virtual bool fp_access_illegal() { return false; }

    /* Plan A config field: default tselect read value (when no trigger
     * selected). Default: -1 (all-1s, conventional "no trigger" sentinel).
     * Cv32e40pCsr::build_cv32e40p() sets it to tselect.reset_val (0 per RTL). */
    iss_reg_t tselect_default_read = (iss_reg_t)-1;

    /* Plan A config field: behavior on access to an undeclared/unsupported
     * CSR. Default: false → log warning, no exception (legacy GVSOC behavior).
     * Cv32e40pCsr::build_cv32e40p() sets it true (RISC-V spec strict). */
    bool raise_on_unsupported_csr_flag = false;

    /* Plan A hook: map a CSR address to its core-specific HWLOOP register index.
     * Default: -1 (not a HWLOOP CSR).
     * Cv32e40pCsr overrides: 0xCC0..0xCC2 → 0..2, 0xCC4..0xCC6 → 4..6 (CoreV2 mapping).
     * Used by iss_csr_read (dispatch to hwloop_read) and iss_csr_write
     * (CV32E40P raises illegal — HWLOOP CSRs are read-only via CSR insn). */
    virtual int hwloop_csr_index(iss_reg_t reg) { return -1; }

    /* Plan A hook: core-specific CSR name lookup for trace messages.
     * Default: nullptr (fall through to generic table).
     * Cv32e40pCsr overrides: 0xCC0..0xCC6 → "lpstart0"/"lpend0"/.../"lpcount1". */
    virtual const char *custom_csr_name(iss_reg_t reg) { return nullptr; }

    /* Plan A config field: whether Exec::bootaddr_apply derives mtvec from
     * boot address. Default: true (generic RISC-V: mtvec = bootaddr & ~0xFF).
     * Cv32e40pCsr::build_cv32e40p() sets it false — mtvec is set by
     * Csr::build() (reset_val=0x1, vectored mode) and rvviRefCsrSet; bootaddr
     * derivation would corrupt the RTL-mandated value. */
    bool bootaddr_writes_mtvec_flag = true;

    /* Plan A hook: EBREAK in M-mode behavior.
     * Default: false → raise ISS_EXCEPT_BREAKPOINT (generic RISC-V w/o debug).
     * Cv32e40pCsr overrides: true when dcsr.ebreakm=1, enter debug mode
     * (RISC-V Debug Spec §3.1.2). */
    virtual bool ebreak_m_mode_enters_debug() { return false; }

protected:
    virtual bool mstatus_access(bool is_write, iss_reg_t &value);
    virtual bool mcycle_access(bool is_write, iss_reg_t &value);
    void undeclare_csr(iss_reg_t address) { regs.erase(address); }

    std::map<iss_reg_t, CsrAbtractReg *> regs;

private:

    bool tselect_access(bool is_write, iss_reg_t &value);
    bool time_access(bool is_write, iss_reg_t &value);
    vp::WireMaster<uint64_t> time_itf;

};
