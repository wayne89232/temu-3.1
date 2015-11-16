#include <stdbool.h>

#define PsActiveProcessHead 0x50
#define PCB 0x188
#define PID  0x180
#define ImageFileName 0x2e0
#define CR3 0x28
#define PPID 0x290
#define PEB 0x338
#define LDR 0x18
#define CreateTime 0x168
#define KDBG_offset 0x10

#define R_EAX 0
#define R_ECX 1
#define R_EDX 2
#define R_EBX 3
#define R_ESP 4
#define R_EBP 5
#define R_ESI 6
#define R_EDI 7

#define R_AL 0
#define R_CL 1
#define R_DL 2
#define R_BL 3
#define R_AH 4
#define R_CH 5
#define R_DH 6
#define R_BH 7

#define R_ES 0
#define R_CS 1
#define R_SS 2
#define R_DS 3
#define R_FS 4
#define R_GS 5

/* segment descriptor fields */
#define DESC_G_MASK     (1 << 23)
#define DESC_B_SHIFT    22
#define DESC_B_MASK     (1 << DESC_B_SHIFT)
#define DESC_L_SHIFT    21 /* x86_64 only : 64 bit code segment */
#define DESC_L_MASK     (1 << DESC_L_SHIFT)
#define DESC_AVL_MASK   (1 << 20)
#define DESC_P_MASK     (1 << 15)
#define DESC_DPL_SHIFT  13
#define DESC_DPL_MASK   (3 << DESC_DPL_SHIFT)
#define DESC_S_MASK     (1 << 12)
#define DESC_TYPE_SHIFT 8
#define DESC_TYPE_MASK  (15 << DESC_TYPE_SHIFT)
#define DESC_A_MASK     (1 << 8)

#define DESC_CS_MASK    (1 << 11) /* 1=code segment 0=data segment */
#define DESC_C_MASK     (1 << 10) /* code: conforming */
#define DESC_R_MASK     (1 << 9)  /* code: readable */

#define DESC_E_MASK     (1 << 10) /* data: expansion direction */
#define DESC_W_MASK     (1 << 9)  /* data: writable */

#define DESC_TSS_BUSY_MASK (1 << 9)

/* eflags masks */
#define CC_C    0x0001
#define CC_P    0x0004
#define CC_A    0x0010
#define CC_Z    0x0040
#define CC_S    0x0080
#define CC_O    0x0800

#define TF_SHIFT   8
#define IOPL_SHIFT 12
#define VM_SHIFT   17

#define TF_MASK                 0x00000100
#define IF_MASK                 0x00000200
#define DF_MASK                 0x00000400
#define IOPL_MASK               0x00003000
#define NT_MASK                 0x00004000
#define RF_MASK                 0x00010000
#define VM_MASK                 0x00020000
#define AC_MASK                 0x00040000
#define VIF_MASK                0x00080000
#define VIP_MASK                0x00100000
#define ID_MASK                 0x00200000

/* hidden flags - used internally by qemu to represent additional cpu
   states. Only the INHIBIT_IRQ, SMM and SVMI are not redundant. We
   avoid using the IOPL_MASK, TF_MASK, VM_MASK and AC_MASK bit
   positions to ease oring with eflags. */
/* current cpl */
#define HF_CPL_SHIFT         0
/* true if soft mmu is being used */
#define HF_SOFTMMU_SHIFT     2
/* true if hardware interrupts must be disabled for next instruction */
#define HF_INHIBIT_IRQ_SHIFT 3
/* 16 or 32 segments */
#define HF_CS32_SHIFT        4
#define HF_SS32_SHIFT        5
/* zero base for DS, ES and SS : can be '0' only in 32 bit CS segment */
#define HF_ADDSEG_SHIFT      6
/* copy of CR0.PE (protected mode) */
#define HF_PE_SHIFT          7
#define HF_TF_SHIFT          8 /* must be same as eflags */
#define HF_MP_SHIFT          9 /* the order must be MP, EM, TS */
#define HF_EM_SHIFT         10
#define HF_TS_SHIFT         11
#define HF_IOPL_SHIFT       12 /* must be same as eflags */
#define HF_LMA_SHIFT        14 /* only used on x86_64: long mode active */
#define HF_CS64_SHIFT       15 /* only used on x86_64: 64 bit code segment  */
#define HF_RF_SHIFT         16 /* must be same as eflags */
#define HF_VM_SHIFT         17 /* must be same as eflags */
#define HF_AC_SHIFT         18 /* must be same as eflags */
#define HF_SMM_SHIFT        19 /* CPU in SMM mode */
#define HF_SVME_SHIFT       20 /* SVME enabled (copy of EFER.SVME) */
#define HF_SVMI_SHIFT       21 /* SVM intercepts are active */
#define HF_OSFXSR_SHIFT     22 /* CR4.OSFXSR */
#define HF_SMAP_SHIFT       23 /* CR4.SMAP */

#define HF_CPL_MASK          (3 << HF_CPL_SHIFT)
#define HF_SOFTMMU_MASK      (1 << HF_SOFTMMU_SHIFT)
#define HF_INHIBIT_IRQ_MASK  (1 << HF_INHIBIT_IRQ_SHIFT)
#define HF_CS32_MASK         (1 << HF_CS32_SHIFT)
#define HF_SS32_MASK         (1 << HF_SS32_SHIFT)
#define HF_ADDSEG_MASK       (1 << HF_ADDSEG_SHIFT)
#define HF_PE_MASK           (1 << HF_PE_SHIFT)
#define HF_TF_MASK           (1 << HF_TF_SHIFT)
#define HF_MP_MASK           (1 << HF_MP_SHIFT)
#define HF_EM_MASK           (1 << HF_EM_SHIFT)
#define HF_TS_MASK           (1 << HF_TS_SHIFT)
#define HF_IOPL_MASK         (3 << HF_IOPL_SHIFT)
#define HF_LMA_MASK          (1 << HF_LMA_SHIFT)
#define HF_CS64_MASK         (1 << HF_CS64_SHIFT)
#define HF_RF_MASK           (1 << HF_RF_SHIFT)
#define HF_VM_MASK           (1 << HF_VM_SHIFT)
#define HF_AC_MASK           (1 << HF_AC_SHIFT)
#define HF_SMM_MASK          (1 << HF_SMM_SHIFT)
#define HF_SVME_MASK         (1 << HF_SVME_SHIFT)
#define HF_SVMI_MASK         (1 << HF_SVMI_SHIFT)
#define HF_OSFXSR_MASK       (1 << HF_OSFXSR_SHIFT)
#define HF_SMAP_MASK         (1 << HF_SMAP_SHIFT)

/* hflags2 */

#define HF2_GIF_SHIFT        0 /* if set CPU takes interrupts */
#define HF2_HIF_SHIFT        1 /* value of IF_MASK when entering SVM */
#define HF2_NMI_SHIFT        2 /* CPU serving NMI */
#define HF2_VINTR_SHIFT      3 /* value of V_INTR_MASKING bit */

#define HF2_GIF_MASK          (1 << HF2_GIF_SHIFT)
#define HF2_HIF_MASK          (1 << HF2_HIF_SHIFT)
#define HF2_NMI_MASK          (1 << HF2_NMI_SHIFT)
#define HF2_VINTR_MASK        (1 << HF2_VINTR_SHIFT)

#define CR0_PE_SHIFT 0
#define CR0_MP_SHIFT 1

#define CR0_PE_MASK  (1U << 0)
#define CR0_MP_MASK  (1U << 1)
#define CR0_EM_MASK  (1U << 2)
#define CR0_TS_MASK  (1U << 3)
#define CR0_ET_MASK  (1U << 4)
#define CR0_NE_MASK  (1U << 5)
#define CR0_WP_MASK  (1U << 16)
#define CR0_AM_MASK  (1U << 18)
#define CR0_PG_MASK  (1U << 31)

#define CR4_VME_MASK  (1U << 0)
#define CR4_PVI_MASK  (1U << 1)
#define CR4_TSD_MASK  (1U << 2)
#define CR4_DE_MASK   (1U << 3)
#define CR4_PSE_MASK  (1U << 4)
#define CR4_PAE_MASK  (1U << 5)
#define CR4_MCE_MASK  (1U << 6)
#define CR4_PGE_MASK  (1U << 7)
#define CR4_PCE_MASK  (1U << 8)
#define CR4_OSFXSR_SHIFT 9
#define CR4_OSFXSR_MASK (1U << CR4_OSFXSR_SHIFT)
#define CR4_OSXMMEXCPT_MASK  (1U << 10)
#define CR4_VMXE_MASK   (1U << 13)
#define CR4_SMXE_MASK   (1U << 14)
#define CR4_FSGSBASE_MASK (1U << 16)
#define CR4_PCIDE_MASK  (1U << 17)
#define CR4_OSXSAVE_MASK (1U << 18)
#define CR4_SMEP_MASK   (1U << 20)
#define CR4_SMAP_MASK   (1U << 21)

#define DR6_BD          (1 << 13)
#define DR6_BS          (1 << 14)
#define DR6_BT          (1 << 15)
#define DR6_FIXED_1     0xffff0ff0

#define DR7_GD          (1 << 13)
#define DR7_TYPE_SHIFT  16
#define DR7_LEN_SHIFT   18
#define DR7_FIXED_1     0x00000400
#define DR7_LOCAL_BP_MASK    0x55
#define DR7_MAX_BP           4
#define DR7_TYPE_BP_INST     0x0
#define DR7_TYPE_DATA_WR     0x1
#define DR7_TYPE_IO_RW       0x2
#define DR7_TYPE_DATA_RW     0x3

#define PG_PRESENT_BIT  0
#define PG_RW_BIT       1
#define PG_USER_BIT     2
#define PG_PWT_BIT      3
#define PG_PCD_BIT      4
#define PG_ACCESSED_BIT 5
#define PG_DIRTY_BIT    6
#define PG_PSE_BIT      7
#define PG_GLOBAL_BIT   8
#define PG_PSE_PAT_BIT  12
#define PG_NX_BIT       63

#define PG_PRESENT_MASK  (1 << PG_PRESENT_BIT)
#define PG_RW_MASK       (1 << PG_RW_BIT)
#define PG_USER_MASK     (1 << PG_USER_BIT)
#define PG_PWT_MASK      (1 << PG_PWT_BIT)
#define PG_PCD_MASK      (1 << PG_PCD_BIT)
#define PG_ACCESSED_MASK (1 << PG_ACCESSED_BIT)
#define PG_DIRTY_MASK    (1 << PG_DIRTY_BIT)
#define PG_PSE_MASK      (1 << PG_PSE_BIT)
#define PG_GLOBAL_MASK   (1 << PG_GLOBAL_BIT)
#define PG_PSE_PAT_MASK  (1 << PG_PSE_PAT_BIT)
#define PG_ADDRESS_MASK  0x000ffffffffff000LL
#define PG_HI_RSVD_MASK  (PG_ADDRESS_MASK & ~PHYS_ADDR_MASK)
#define PG_HI_USER_MASK  0x7ff0000000000000LL
#define PG_NX_MASK       (1LL << PG_NX_BIT)

#define PG_ERROR_W_BIT     1

#define PG_ERROR_P_MASK    0x01
#define PG_ERROR_W_MASK    (1 << PG_ERROR_W_BIT)
#define PG_ERROR_U_MASK    0x04
#define PG_ERROR_RSVD_MASK 0x08
#define PG_ERROR_I_D_MASK  0x10

#define MCG_CTL_P       (1ULL<<8)   /* MCG_CAP register available */
#define MCG_SER_P       (1ULL<<24) /* MCA recovery/new status bits */

#define MCE_CAP_DEF     (MCG_CTL_P|MCG_SER_P)
#define MCE_BANKS_DEF   10

#define MCG_STATUS_RIPV (1ULL<<0)   /* restart ip valid */
#define MCG_STATUS_EIPV (1ULL<<1)   /* ip points to correct instruction */
#define MCG_STATUS_MCIP (1ULL<<2)   /* machine check in progress */

#define MCI_STATUS_VAL   (1ULL<<63)  /* valid error */
#define MCI_STATUS_OVER  (1ULL<<62)  /* previous errors lost */
#define MCI_STATUS_UC    (1ULL<<61)  /* uncorrected error */
#define MCI_STATUS_EN    (1ULL<<60)  /* error enabled */
#define MCI_STATUS_MISCV (1ULL<<59)  /* misc error reg. valid */
#define MCI_STATUS_ADDRV (1ULL<<58)  /* addr reg. valid */
#define MCI_STATUS_PCC   (1ULL<<57)  /* processor context corrupt */
#define MCI_STATUS_S     (1ULL<<56)  /* Signaled machine check */
#define MCI_STATUS_AR    (1ULL<<55)  /* Action required */

/* MISC register defines */
#define MCM_ADDR_SEGOFF  0      /* segment offset */
#define MCM_ADDR_LINEAR  1      /* linear address */
#define MCM_ADDR_PHYS    2      /* physical address */
#define MCM_ADDR_MEM     3      /* memory address */
#define MCM_ADDR_GENERIC 7      /* generic */

#define MSR_IA32_TSC                    0x10
#define MSR_IA32_APICBASE               0x1b
#define MSR_IA32_APICBASE_BSP           (1<<8)
#define MSR_IA32_APICBASE_ENABLE        (1<<11)
#define MSR_IA32_APICBASE_BASE          (0xfffff<<12)
#define MSR_IA32_FEATURE_CONTROL        0x0000003a
#define MSR_TSC_ADJUST                  0x0000003b
#define MSR_IA32_TSCDEADLINE            0x6e0

#define MSR_P6_PERFCTR0                 0xc1

#define MSR_MTRRcap                     0xfe
#define MSR_MTRRcap_VCNT                8
#define MSR_MTRRcap_FIXRANGE_SUPPORT    (1 << 8)
#define MSR_MTRRcap_WC_SUPPORTED        (1 << 10)

#define MSR_IA32_SYSENTER_CS            0x174
#define MSR_IA32_SYSENTER_ESP           0x175
#define MSR_IA32_SYSENTER_EIP           0x176

#define MSR_MCG_CAP                     0x179
#define MSR_MCG_STATUS                  0x17a
#define MSR_MCG_CTL                     0x17b

#define MSR_P6_EVNTSEL0                 0x186

#define MSR_IA32_PERF_STATUS            0x198

#define MSR_IA32_MISC_ENABLE            0x1a0
/* Indicates good rep/movs microcode on some processors: */
#define MSR_IA32_MISC_ENABLE_DEFAULT    1

#define MSR_MTRRphysBase(reg)           (0x200 + 2 * (reg))
#define MSR_MTRRphysMask(reg)           (0x200 + 2 * (reg) + 1)

#define MSR_MTRRphysIndex(addr)         ((((addr) & ~1u) - 0x200) / 2)

#define MSR_MTRRfix64K_00000            0x250
#define MSR_MTRRfix16K_80000            0x258
#define MSR_MTRRfix16K_A0000            0x259
#define MSR_MTRRfix4K_C0000             0x268
#define MSR_MTRRfix4K_C8000             0x269
#define MSR_MTRRfix4K_D0000             0x26a
#define MSR_MTRRfix4K_D8000             0x26b
#define MSR_MTRRfix4K_E0000             0x26c
#define MSR_MTRRfix4K_E8000             0x26d
#define MSR_MTRRfix4K_F0000             0x26e
#define MSR_MTRRfix4K_F8000             0x26f

#define MSR_PAT                         0x277

#define MSR_MTRRdefType                 0x2ff

#define MSR_CORE_PERF_FIXED_CTR0        0x309
#define MSR_CORE_PERF_FIXED_CTR1        0x30a
#define MSR_CORE_PERF_FIXED_CTR2        0x30b
#define MSR_CORE_PERF_FIXED_CTR_CTRL    0x38d
#define MSR_CORE_PERF_GLOBAL_STATUS     0x38e
#define MSR_CORE_PERF_GLOBAL_CTRL       0x38f
#define MSR_CORE_PERF_GLOBAL_OVF_CTRL   0x390

#define MSR_MC0_CTL                     0x400
#define MSR_MC0_STATUS                  0x401
#define MSR_MC0_ADDR                    0x402
#define MSR_MC0_MISC                    0x403

#define MSR_EFER                        0xc0000080

#define MSR_EFER_SCE   (1 << 0)
#define MSR_EFER_LME   (1 << 8)
#define MSR_EFER_LMA   (1 << 10)
#define MSR_EFER_NXE   (1 << 11)
#define MSR_EFER_SVME  (1 << 12)
#define MSR_EFER_FFXSR (1 << 14)

#define MSR_STAR                        0xc0000081
#define MSR_LSTAR                       0xc0000082
#define MSR_CSTAR                       0xc0000083
#define MSR_FMASK                       0xc0000084
#define MSR_FSBASE                      0xc0000100
#define MSR_GSBASE                      0xc0000101
#define MSR_KERNELGSBASE                0xc0000102
#define MSR_TSC_AUX                     0xc0000103

#define MSR_VM_HSAVE_PA                 0xc0010117

#define MSR_IA32_BNDCFGS                0x00000d90
#define MSR_IA32_XSS                    0x00000da0

#define XSTATE_FP                       (1ULL << 0)
#define XSTATE_SSE                      (1ULL << 1)
#define XSTATE_YMM                      (1ULL << 2)
#define XSTATE_BNDREGS                  (1ULL << 3)
#define XSTATE_BNDCSR                   (1ULL << 4)
#define XSTATE_OPMASK                   (1ULL << 5)
#define XSTATE_ZMM_Hi256                (1ULL << 6)
#define XSTATE_Hi16_ZMM                 (1ULL << 7)


typedef uint64_t target_ulong;
typedef uint8_t flag;

#define CPUArchState struct CPUX86State

#define CPU_NB_REGS64 16
#define CPU_NB_REGS32 8

#ifdef TARGET_X86_64
#define CPU_NB_REGS CPU_NB_REGS64
#else
#define CPU_NB_REGS CPU_NB_REGS32
#endif
#define MAX_FIXED_COUNTERS 3
#define MAX_GP_COUNTERS    (MSR_IA32_PERF_STATUS - MSR_P6_EVNTSEL0)

#define NB_MMU_MODES 3

#define NB_OPMASK_REGS 8

typedef struct {
    uint32_t v;
} float32;

typedef struct {
    uint64_t v;
} float64;

typedef enum FeatureWord {
    FEAT_1_EDX,         /* CPUID[1].EDX */
    FEAT_1_ECX,         /* CPUID[1].ECX */
    FEAT_7_0_EBX,       /* CPUID[EAX=7,ECX=0].EBX */
    FEAT_8000_0001_EDX, /* CPUID[8000_0001].EDX */
    FEAT_8000_0001_ECX, /* CPUID[8000_0001].ECX */
    FEAT_8000_0007_EDX, /* CPUID[8000_0007].EDX */
    FEAT_C000_0001_EDX, /* CPUID[C000_0001].EDX */
    FEAT_KVM,           /* CPUID[4000_0001].EAX (KVM_CPUID_FEATURES) */
    FEAT_SVM,           /* CPUID[8000_000A].EDX */
    FEAT_XSAVE,         /* CPUID[EAX=0xd,ECX=1].EAX */
    FEATURE_WORDS,
} FeatureWord;

typedef enum TPRAccess {
    TPR_ACCESS_READ,
    TPR_ACCESS_WRITE,
} TPRAccess;

typedef uint32_t FeatureWordArray[FEATURE_WORDS];

typedef struct {
    uint64_t low;
    uint16_t high;
} floatx80;

typedef struct {
    uint64_t base;
    uint64_t mask;
} MTRRVar;

typedef struct float_status {
    signed char float_detect_tininess;
    signed char float_rounding_mode;
    signed char float_exception_flags;
    signed char floatx80_rounding_precision;
    /* should denormalised results go to zero and set the inexact flag? */
    flag flush_to_zero;
    /* should denormalised inputs go to zero and set the input_denormal flag? */
    flag flush_inputs_to_zero;
    flag default_nan_mode;
} float_status;

typedef union {
    uint8_t _b[8];
    uint16_t _w[4];
    uint32_t _l[2];
    float32 _s[2];
    uint64_t q;
} MMXReg;

typedef union {
    floatx80 d __attribute__((aligned(16)));
    MMXReg mmx;
} FPReg;

typedef struct SegmentCache {
    uint32_t selector;
    uint32_t base;
    uint32_t limit;
    uint32_t flags;
} SegmentCache;

typedef union {
    uint8_t _b[64];
    uint16_t _w[32];
    uint32_t _l[16];
    uint64_t _q[8];
    float32 _s[16];
    float64 _d[8];
} XMMReg; /* really zmm */



typedef struct BNDReg {
    uint64_t lb;
    uint64_t ub;
} BNDReg;

typedef struct BNDCSReg {
    uint64_t cfgu;
    uint64_t sts;
} BNDCSReg;

typedef struct CPUX86State {
    /* standard registers */
    target_ulong regs[CPU_NB_REGS];
    target_ulong eip;
    target_ulong eflags; /* eflags register. During CPU emulation, CC
                        flags and DF are set to zero because they are
                        stored elsewhere */

    /* emulator internal eflags handling */
    target_ulong cc_dst;
    target_ulong cc_src;
    target_ulong cc_src2;
    uint32_t cc_op;
    int32_t df; /* D flag : 1 if D = 0, -1 if D = 1 */
    uint32_t hflags; /* TB flags, see HF_xxx constants. These flags
                        are known at translation time. */
    uint32_t hflags2; /* various other flags, see HF2_xxx constants. */

    /* segments */
    SegmentCache segs[6]; /* selector values */
    SegmentCache ldt;
    SegmentCache tr;
    SegmentCache gdt; /* only base and limit are used */
    SegmentCache idt; /* only base and limit are used */

    target_ulong cr[5]; /* NOTE: cr1 is unused */
    int32_t a20_mask;

    BNDReg bnd_regs[4];
    BNDCSReg bndcs_regs;
    uint64_t msr_bndcfgs;

    /* Beginning of state preserved by INIT (dummy marker).  */
    struct {} start_init_save;

    /* FPU state */
    unsigned int fpstt; /* top of stack index */
    uint16_t fpus;
    uint16_t fpuc;
    uint8_t fptags[8];   /* 0 = valid, 1 = empty */
    FPReg fpregs[8];
    /* KVM-only so far */
    uint16_t fpop;
    uint64_t fpip;
    uint64_t fpdp;

    /* emulator internal variables */
    float_status fp_status;
    floatx80 ft0;

    float_status mmx_status; /* for 3DNow! float ops */
    float_status sse_status;
    uint32_t mxcsr;
    XMMReg xmm_regs[CPU_NB_REGS == 8 ? 8 : 32];
    XMMReg xmm_t0;
    MMXReg mmx_t0;

    uint64_t opmask_regs[NB_OPMASK_REGS];

    /* sysenter registers */
    uint32_t sysenter_cs;
    target_ulong sysenter_esp;
    target_ulong sysenter_eip;
    uint64_t efer;
    uint64_t star;

    uint64_t vm_hsave;

#ifdef TARGET_X86_64
    target_ulong lstar;
    target_ulong cstar;
    target_ulong fmask;
    target_ulong kernelgsbase;
#endif

    uint64_t tsc;
    uint64_t tsc_adjust;
    uint64_t tsc_deadline;

    uint64_t mcg_status;
    uint64_t msr_ia32_misc_enable;
    uint64_t msr_ia32_feature_control;

    uint64_t msr_fixed_ctr_ctrl;
    uint64_t msr_global_ctrl;
    uint64_t msr_global_status;
    uint64_t msr_global_ovf_ctrl;
    uint64_t msr_fixed_counters[MAX_FIXED_COUNTERS];
    uint64_t msr_gp_counters[MAX_GP_COUNTERS];
    uint64_t msr_gp_evtsel[MAX_GP_COUNTERS];

    uint64_t pat;
    uint32_t smbase;

    /* End of state preserved by INIT (dummy marker).  */
    struct {} end_init_save;

    uint64_t system_time_msr;
    uint64_t wall_clock_msr;
    uint64_t steal_time_msr;
    uint64_t async_pf_en_msr;
    uint64_t pv_eoi_en_msr;

    uint64_t msr_hv_hypercall;
    uint64_t msr_hv_guest_os_id;
    uint64_t msr_hv_vapic;
    uint64_t msr_hv_tsc;

    /* exception/interrupt handling */
    int error_code;
    int exception_is_int;
    target_ulong exception_next_eip;
    target_ulong dr[8]; /* debug registers */
    union {
        struct CPUBreakpoint *cpu_breakpoint[4];
        struct CPUWatchpoint *cpu_watchpoint[4];
    }; /* break/watchpoints for dr[0..3] */
    int old_exception;  /* exception in flight */

    uint64_t vm_vmcb;
    uint64_t tsc_offset;
    uint64_t intercept;
    uint16_t intercept_cr_read;
    uint16_t intercept_cr_write;
    uint16_t intercept_dr_read;
    uint16_t intercept_dr_write;
    uint32_t intercept_exceptions;
    uint8_t v_tpr;

    /* KVM states, automatically cleared on reset */
    uint8_t nmi_injected;
    uint8_t nmi_pending;

    /* Fields from here on are preserved across CPU reset. */

    /* processor features (e.g. for CPUID insn) */
    uint32_t cpuid_level;
    uint32_t cpuid_xlevel;
    uint32_t cpuid_xlevel2;
    uint32_t cpuid_vendor1;
    uint32_t cpuid_vendor2;
    uint32_t cpuid_vendor3;
    uint32_t cpuid_version;
    FeatureWordArray features;
    uint32_t cpuid_model[12];

    /* MTRRs */
    uint64_t mtrr_fixed[11];
    uint64_t mtrr_deftype;
    MTRRVar mtrr_var[MSR_MTRRcap_VCNT];

    /* For KVM */
    uint32_t mp_state;
    int32_t exception_injected;
    int32_t interrupt_injected;
    uint8_t soft_interrupt;
    uint8_t has_error_code;
    uint32_t sipi_vector;
    bool tsc_valid;
    int tsc_khz;
    void *kvm_xsave_buf;

    uint64_t mcg_cap;
    uint64_t mcg_ctl;
    uint64_t mce_banks[MCE_BANKS_DEF*4];

    uint64_t tsc_aux;

    /* vmstate */
    uint16_t fpus_vmstate;
    uint16_t fptag_vmstate;
    uint16_t fpregs_format_vmstate;
    uint64_t xstate_bv;

    uint64_t xcr0;
    uint64_t xss;

    TPRAccess tpr_access_type;
} CPUX86State;


typedef struct p_cmdline{
    CPUArchState* env;
    char params[256];
}p_cmdline;