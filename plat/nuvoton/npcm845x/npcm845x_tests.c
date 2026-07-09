/*
 * Copyright (C) 2024 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * BL31 platform self-tests for NPCM845x (Arbel EVB).
 * Runs at EL3 after bl31_platform_setup(), before BL32/BL33 handoff.
 * Enabled with BL31_SELFTEST=1 build flag.
 */

#include <stdint.h>

#include <arch_helpers.h>
#include <common/debug.h>
#include <drivers/console.h>
#include <lib/mmio.h>
#include <plat/common/platform.h>
#include <platform_def.h>

/* GIC-400 register offsets */
#define GICD_IIDR_OFFSET	U(0x008)  /* GICv2: Distributor IIDR at 0x008 */
#define GICC_IIDR_OFFSET	U(0x0FC)  /* GICv2: CPU interface IIDR at 0x0FC */
#define GIC_ARM_IMPLEMENTER	U(0x43B)  /* bits [11:0] of IIDR */

/*
 * NPCM845x scratchpad MMIO register for inter-CPU signaling.
 * Device memory — writes are immediately visible to all CPUs,
 * bypassing any cache coherency concerns.
 */
#define NPCM_SCRATCH_REG	U(0xF0800E7C)

#define SELFTEST_PASS	0
#define SELFTEST_FAIL	1

static unsigned int tests_passed;
static unsigned int tests_failed;

#define RUN_TEST(desc, fn) do {					\
	if ((fn)() == SELFTEST_PASS) {				\
		NOTICE("  PASS  " desc "\n");			\
		tests_passed++;					\
	} else {						\
		ERROR("  FAIL  " desc "\n");			\
		tests_failed++;					\
	}							\
} while (0)

/* ------------------------------------------------------------------ */
/* Test: GIC-400 distributor accessible                               */
/* ------------------------------------------------------------------ */
static int test_gic_distributor(void)
{
	uint32_t iidr = mmio_read_32(PLAT_ARM_GICD_BASE + GICD_IIDR_OFFSET);

	return ((iidr & 0xFFFU) == GIC_ARM_IMPLEMENTER) ?
		SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: GIC-400 CPU interface accessible                             */
/* ------------------------------------------------------------------ */
static int test_gic_cpu_interface(void)
{
	uint32_t iidr = mmio_read_32(PLAT_ARM_GICC_BASE + GICC_IIDR_OFFSET);

	return ((iidr & 0xFFFU) == GIC_ARM_IMPLEMENTER) ?
		SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: Generic timer advances                                        */
/* ------------------------------------------------------------------ */
static int test_timer_counting(void)
{
	uint64_t t1, t2;
	volatile int i;

	t1 = read_cntpct_el0();
	for (i = 0; i < 10000; i++)
		;
	t2 = read_cntpct_el0();

	return (t2 > t1) ? SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: CNTFRQ_EL0 is non-zero and plausible                         */
/* ------------------------------------------------------------------ */
static int test_timer_frequency(void)
{
	uint64_t freq = read_cntfrq_el0();

	/* Accept any frequency between 1 MHz and 1 GHz */
	return (freq >= 1000000ULL && freq <= 1000000000ULL) ?
		SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: DRAM alternating and walking-ones patterns                   */
/* ------------------------------------------------------------------ */
static uint64_t dram_buf[128] __aligned(64);

static int test_dram_pattern(void)
{
	const uint64_t PAT_AA = 0xAAAAAAAAAAAAAAAAULL;
	const uint64_t PAT_55 = 0x5555555555555555ULL;
	unsigned int i;

	for (i = 0U; i < 128U; i += 2U) {
		dram_buf[i]      = PAT_AA;
		dram_buf[i + 1U] = PAT_55;
	}
	dsb();

	for (i = 0U; i < 128U; i += 2U) {
		if (dram_buf[i]      != PAT_AA) return SELFTEST_FAIL;
		if (dram_buf[i + 1U] != PAT_55) return SELFTEST_FAIL;
	}

	/* Walking-ones */
	for (i = 0U; i < 64U; i++) {
		dram_buf[i] = 1ULL << (i & 63U);
	}
	dsb();
	for (i = 0U; i < 64U; i++) {
		if (dram_buf[i] != (1ULL << (i & 63U)))
			return SELFTEST_FAIL;
	}

	return SELFTEST_PASS;
}

/* ------------------------------------------------------------------ */
/* Test: CPU0 MPIDR is 0, core_pos is 0                               */
/* ------------------------------------------------------------------ */
static int test_cpu_mpidr(void)
{
	u_register_t mpidr = read_mpidr_el1();
	u_register_t aff   = mpidr & (MPIDR_CPU_MASK | MPIDR_CLUSTER_MASK);
	unsigned int cpos  = plat_my_core_pos();

	return ((aff == 0UL) && (cpos == 0U)) ? SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: GIC-400 topology (GICD_TYPER)                                */
/* ------------------------------------------------------------------ */
static int test_gic_topology(void)
{
	uint32_t typer    = mmio_read_32(PLAT_ARM_GICD_BASE + U(0x004));
	unsigned int cpus = ((typer >> 5U) & 0x7U) + 1U;  /* bits [7:5] */
	unsigned int itln = ((typer & 0x1FU) + 1U) * 32U; /* bits [4:0] */

	NOTICE("  INFO  GIC: %u CPUs, %u interrupt lines\n", cpus, itln);

	/* NPCM845x: 4-core cluster, at least 128 interrupt IDs */
	return (cpus == 4U && itln >= 128U) ? SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Test: System counter CNTCR is enabled                              */
/* ------------------------------------------------------------------ */
static int test_system_counter_enabled(void)
{
	uint32_t cntcr = mmio_read_32(ARM_SYS_CNTCTL_BASE + CNTCR_OFF);

	/* bit 0 = EN: counter is running */
	return ((cntcr & 1U) != 0U) ? SELFTEST_PASS : SELFTEST_FAIL;
}

/* ------------------------------------------------------------------ */
/* Tests: Bring up CPU1, CPU2, CPU3 via Nuvoton mailbox               */
/* ------------------------------------------------------------------ */

/*
 * Scratchpad bits used for secondary CPU signaling (bits 8–10).
 * Device memory — immediately visible to all CPUs, no cache concerns.
 */
#define NPCM_CPU_TEST_BIT(n)	BIT_32(8U + (n) - 1U)  /* n=1,2,3 */

/*
 * Common entry point for all secondary CPUs.
 * Each CPU sets its own bit in the scratchpad based on core_pos.
 * Must NOT return — warm_boot assembly uses "br x1", not "blr x1".
 */
static void __dead2 secondary_selftest_entry(void)
{
	unsigned int cpu = plat_my_core_pos();  /* 1, 2, or 3 */

	if (cpu >= 1U && cpu <= 3U) {
		mmio_write_32(NPCM_SCRATCH_REG,
			      mmio_read_32(NPCM_SCRATCH_REG) |
			      NPCM_CPU_TEST_BIT(cpu));
		dsb();
	}
	for (;;) wfi();
}

/*
 * Wake secondary CPU `cpu` (1..3), wait up to 1 s for it to signal,
 * return SELFTEST_PASS / SELFTEST_FAIL.
 */
static int wake_and_verify_cpu(unsigned int cpu, uint64_t freq,
				uintptr_t ep_addr)
{
	uintptr_t hold_addr = PLAT_NPCM_TM_HOLD_BASE +
			      (cpu * PLAT_NPCM_TM_HOLD_ENTRY_SIZE);
	uint32_t  bit       = NPCM_CPU_TEST_BIT(cpu);
	uint64_t  deadline;

	/* Clear this CPU's signal bit */
	mmio_write_32(NPCM_SCRATCH_REG,
		      mmio_read_32(NPCM_SCRATCH_REG) & ~bit);
	dsb();

	/* Set hold slot to GO and send wake event */
	mmio_write_64(hold_addr, PLAT_NPCM_TM_HOLD_STATE_GO);
	dsb();
	isb();
	sev();

	deadline = read_cntpct_el0() + freq;
	while (!(mmio_read_32(NPCM_SCRATCH_REG) & bit)) {
		if (read_cntpct_el0() > deadline)
			return SELFTEST_FAIL;
	}
	return SELFTEST_PASS;
}

static int test_cpu1_bringup(void)
{
	uintptr_t ep_addr  = PLAT_NPCM_TM_ENTRYPOINT;
	uint64_t  saved_ep = mmio_read_64(ep_addr);
	uint64_t  freq     = read_cntfrq_el0();
	int       rc;

	if (freq == 0ULL) freq = 25000000ULL;

	mmio_write_64(ep_addr, (uint64_t)(uintptr_t)secondary_selftest_entry);
	dsb();
	rc = wake_and_verify_cpu(1U, freq, ep_addr);
	mmio_write_64(ep_addr, saved_ep);
	dsb();
	return rc;
}

static int test_cpu2_bringup(void)
{
	uintptr_t ep_addr  = PLAT_NPCM_TM_ENTRYPOINT;
	uint64_t  saved_ep = mmio_read_64(ep_addr);
	uint64_t  freq     = read_cntfrq_el0();
	int       rc;

	if (freq == 0ULL) freq = 25000000ULL;

	mmio_write_64(ep_addr, (uint64_t)(uintptr_t)secondary_selftest_entry);
	dsb();
	rc = wake_and_verify_cpu(2U, freq, ep_addr);
	mmio_write_64(ep_addr, saved_ep);
	dsb();
	return rc;
}

static int test_cpu3_bringup(void)
{
	uintptr_t ep_addr  = PLAT_NPCM_TM_ENTRYPOINT;
	uint64_t  saved_ep = mmio_read_64(ep_addr);
	uint64_t  freq     = read_cntfrq_el0();
	int       rc;

	if (freq == 0ULL) freq = 25000000ULL;

	mmio_write_64(ep_addr, (uint64_t)(uintptr_t)secondary_selftest_entry);
	dsb();
	rc = wake_and_verify_cpu(3U, freq, ep_addr);
	mmio_write_64(ep_addr, saved_ep);
	dsb();
	return rc;
}

/* ------------------------------------------------------------------ */
/* Public entry point                                                  */
/* ------------------------------------------------------------------ */
void npcm845x_run_selftests(void)
{
	tests_passed = 0U;
	tests_failed = 0U;

	NOTICE("========================================\n");
	NOTICE("BL31 platform self-test  (NPCM845x)\n");
	NOTICE("========================================\n");

	RUN_TEST("GIC-400 distributor",      test_gic_distributor);
	RUN_TEST("GIC-400 CPU interface",    test_gic_cpu_interface);
	RUN_TEST("GIC-400 topology",         test_gic_topology);
	RUN_TEST("Generic timer counting",   test_timer_counting);
	RUN_TEST("Generic timer frequency",  test_timer_frequency);
	RUN_TEST("System counter enabled",   test_system_counter_enabled);
	RUN_TEST("DRAM read/write pattern",  test_dram_pattern);
	RUN_TEST("CPU0 MPIDR / core_pos",    test_cpu_mpidr);
	RUN_TEST("CPU1 mailbox bringup",     test_cpu1_bringup);
	RUN_TEST("CPU2 mailbox bringup",     test_cpu2_bringup);
	RUN_TEST("CPU3 mailbox bringup",     test_cpu3_bringup);

	NOTICE("========================================\n");
	NOTICE("Result: %u passed, %u failed\n", tests_passed, tests_failed);
	NOTICE("========================================\n");
	console_flush();
}
