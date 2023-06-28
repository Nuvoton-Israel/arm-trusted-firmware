/*
 * Copyright (c) 2014-2023, ARM Limited and Contributors. All rights reserved.
 *
 * Copyright (c) 2017-2023 Nuvoton Technology Corp.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/console.h>
#include <npcm845x_lpuart.h>
#include <nuvoton_uart_16550.h>
#include <plat/arm/common/plat_arm.h>
#include <platform_def.h>
#include <platform_tsp.h>

#ifdef NPCM845X_DEBUG
#include <plat_private.h>
#endif /* NPCM845X_DEBUG */

/*******************************************************************************
 * Initialize the UART
 ******************************************************************************/
void tsp_early_platform_setup(void)
{
/*
 * Register a different console than already in use to display
 * messages from TSP
 */
	unsigned long UART_BASE_ADDR;
	static console_t console;

/* Use the same console to print data from TSP */

/*
 * Register UART w/o initialization -
 * A clock rate of zero means to skip the initialisation.
 */
#ifdef CONFIG_TARGET_ARBEL_PALLADIUM
	UART_BASE_ADDR = npcm845x_get_base_uart(UART0_DEV);
#else
	UART_BASE_ADDR = npcm845x_get_base_uart(UART3_DEV);
#endif
	nuvoton_console_16550_register(UART_BASE_ADDR, 0, 0, &console);
}

/******************************************************************************
 * Perform platform specific setup placeholder
 *****************************************************************************/
void tsp_platform_setup(void)
{
	plat_arm_gic_driver_init();
	plat_arm_gic_init();
}

 #define MAP_BL32_TOTAL		MAP_REGION_FLAT( \
					BL32_BASE, BL32_END - BL32_BASE, \
					MT_CODE | MT_RW | MT_SECURE)

/******************************************************************************
 * Perform the very early platform specific architectural setup here.
 * At the moment this is only intializes the MMU
 *****************************************************************************/
void tsp_plat_arch_setup(void)
{
	const mmap_region_t bl_regions[] = {
		MAP_BL32_TOTAL,
#if RECLAIM_INIT_CODE
/* Not compiled */
		MAP_BL_INIT_CODE,
#endif
#if SEPARATE_NOBITS_REGION
		MAP_BL31_NOBITS,
#endif
		ARM_MAP_BL_RO,
#if USE_ROMLIB
		ARM_MAP_ROMLIB_CODE,
		ARM_MAP_ROMLIB_DATA,
#endif
#if USE_COHERENT_MEM
		ARM_MAP_BL_COHERENT_RAM,
#endif
		{0}
	};

	setup_page_tables(bl_regions, plat_arm_get_mmap());
	#ifndef DEBUG_MMU_OFF
	enable_mmu_el1(0);
	#endif
}
