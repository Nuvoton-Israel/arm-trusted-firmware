/*
 * Copyright (c) 2015-2020, ARM Limited and Contributors. All rights reserved.
 *	Copyright (c) 2017 Nuvoton Technology Corp.
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>

#include <arch.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <common/debug.h>
#include <drivers/console.h>
#include <npcm845x_trustzone.h>
#include <lib/debugfs.h>
#include <lib/extensions/ras.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_tables_compat.h>
#include <plat/arm/common/plat_arm.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <drivers/generic_delay_timer.h>
#include <common/bl_common.h>

#include <nuvoton_uart_16550.h>
//#include <npcm845x_pads.h>
//#include <npcm845x_iomux.h>
#include <npcm845x_lpuart.h>
#include <npcm845x_clock.h>
#include <npcm845x_gcr.h>
#include <plat_npcm845x.h>


/*
 * Placeholder variables for copying the arguments that have been passed to
 * BL31 from BL2.
 */
static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;

#if !RESET_TO_BL31
/*
 * Check that BL31_BASE is above ARM_FW_CONFIG_LIMIT. The reserved page
 * is required for SOC_FW_CONFIG/TOS_FW_CONFIG passed from BL2.
 */
//CASSERT(BL31_BASE >= ARM_FW_CONFIG_LIMIT, assert_bl31_base_overflows);
#endif

#define MAP_BL31_TOTAL		MAP_REGION_FLAT(			\
					BL31_START,			\
					BL31_END - BL31_START,		\
					MT_MEMORY | MT_RW | EL3_PAS)
#if RECLAIM_INIT_CODE
Not Compiled
IMPORT_SYM(unsigned long, __INIT_CODE_START__, BL_INIT_CODE_BASE);
IMPORT_SYM(unsigned long, __INIT_CODE_END__, BL_CODE_END_UNALIGNED);

#define	BL_INIT_CODE_END	((BL_CODE_END_UNALIGNED + PAGE_SIZE - 1) & \
					~(PAGE_SIZE - 1))

#define MAP_BL_INIT_CODE	MAP_REGION_FLAT(			\
					BL_INIT_CODE_BASE,		\
					BL_INIT_CODE_END		\
						- BL_INIT_CODE_BASE,	\
					MT_CODE | MT_SECURE)
#endif

#if SEPARATE_NOBITS_REGION
#define MAP_BL31_NOBITS		MAP_REGION_FLAT(			\
					BL31_NOBITS_BASE,		\
					BL31_NOBITS_LIMIT 		\
						- BL31_NOBITS_BASE,	\
					MT_MEMORY | MT_RW | EL3_PAS)

#endif
/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for the
 * security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type. A NULL pointer is returned
 * if the image does not exist.
 ******************************************************************************/
struct entry_point_info *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	assert(sec_state_is_valid(type));
	next_image_info = (type == NON_SECURE)
			? &bl33_image_ep_info : &bl32_image_ep_info;
	/*
	 * None of the images on the ARM development platforms can have 0x0
	 * as the entrypoint
	 */
	if (next_image_info->pc)
		return next_image_info;
	else
		return NULL;
}

int board_clocks_init()
{
	
	if (arbel_clk_if_init() != 0)
		return -1;
	
	return 0;
	
}
int board_uart_init(void)
{
	unsigned long UART_BASE_ADDR;
	static console_t console;
	
#ifdef CONFIG_TARGET_ARBEL_PALLADIUM
	UART_Init(UART0_DEV, UART_MUX_MODE1_HSP1_SI2____HSP2_UART2__UART1_s_HSP1__UART3_s_SI2, UART_BAUDRATE_115200);
	UART_BASE_ADDR = npcm845x_get_base_uart(UART0_DEV);
#else
   UART_BASE_ADDR = npcm845x_get_base_uart(UART0_DEV);

// Hila: To Do check flag options, or modify UART init 
#define INITIALIZE_UART_BY_BOOTBLOCK
#ifndef INITIALIZE_UART_BY_BOOTBLOCK

	UART_Init(UART0_DEV,  UART_BAUDRATE_115200);

#endif

#endif
	
	
	/* Register UART w/o initialization - A clock rate of zero means to skip the initialisation.  */
	nuvoton_console_16550_register(UART_BASE_ADDR, 0, 0, &console);
	
	
	return 0;
}




unsigned int plat_get_syscnt_freq2(void)
{
	return COUNTER_FREQUENCY;
}

/*******************************************************************************
 * Perform any BL31 early platform setup common to ARM standard platforms.
 * Here is an opportunity to copy parameters passed by the calling EL (S-EL1
 * in BL2 & EL3 in BL1) before they are lost (potentially). This needs to be
 * done before the MMU is initialized so that the memory layout can be used
 * while creating page tables. BL2 has flushed this information to memory, so
 * we are guaranteed to pick up good data.
 ******************************************************************************/
 void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
		u_register_t arg2, u_register_t arg3)
{
	//To Do: test if can be commented out 
	#if RESET_TO_BL31
	 void *from_bl2 = (void *)arg0;
	 void *plat_params_from_bl2 = (void *)arg3;
	 if (from_bl2 != NULL)
		 assert(from_bl2 == NULL);
	  if (plat_params_from_bl2 != NULL)
		 assert(plat_params_from_bl2 == NULL);
	#endif
	
	
	// Initialize Delay timer 
	 generic_delay_timer_init();
	/* Initialize the console to provide early debug support */
	//Skipping UART is already initialized - use Boot Block initialization
	//arm_console_boot_init();
	// Do Specific Board/Chip initializations
	
	board_uart_init();
	
	board_clocks_init();
	
	
	/*
	 * Initialize Interconnect for this cluster during cold boot.
	 * No need for locks as no other CPU is active.
	 */
	 
	 // Hila: Skip, Not used in many projects.
	//plat_arm_interconnect_init();

	/*
	 * Enable Interconnect coherency for the primary CPU's cluster.
	 * Earlier bootloader stages might already do this (e.g. Trusted
	 * Firmware's BL1 does it) but we can't assume so. There is no harm in
	 * executing this code twice anyway.
	 * Platform specific PSCI code will enable coherency for other
	 * clusters.
	 */
	 //Hila Skip, for now only one cluster 
	 //To Do - handle coherency 
	 
	//plat_arm_interconnect_enter_coherency();
#if RESET_TO_BL31
		/* There are no parameters from BL2 if BL31 is a reset vector */
		assert(from_bl2 == NULL);
		assert(plat_params_from_bl2 == NULL);
	
# ifdef BL32_BASE
		/* Populate entry point information for BL32 */
		SET_PARAM_HEAD(&bl32_image_ep_info,
					PARAM_EP,
					VERSION_1,
					0);
		SET_SECURITY_STATE(bl32_image_ep_info.h.attr, SECURE);
		bl32_image_ep_info.pc = BL32_BASE;
		bl32_image_ep_info.spsr = arm_get_spsr_for_bl32_entry();
	
#if defined(SPD_spmd)
		/* SPM (hafnium in secure world) expects SPM Core manifest base address
		 * in x0, which in !RESET_TO_BL31 case loaded after base of non shared
		 * SRAM(after 4KB offset of SRAM). But in RESET_TO_BL31 case all non
		 * shared SRAM is allocated to BL31, so to avoid overwriting of manifest
		 * keep it in the last page.
		 */
		bl32_image_ep_info.args.arg0 = ARM_TRUSTED_SRAM_BASE +
					PLAT_ARM_TRUSTED_SRAM_SIZE - PAGE_SIZE;
#endif
	
# endif /* BL32_BASE */
	
		/* Populate entry point information for BL33 */
		SET_PARAM_HEAD(&bl33_image_ep_info,
					PARAM_EP,
					VERSION_1,
					0);
		/*
		 * Tell BL31 where the non-trusted software image
		 * is located and the entry state information
		 */
		bl33_image_ep_info.pc = plat_get_ns_image_entrypoint();
		// Generic ARM code will switch to EL2, revert to EL1 
		bl33_image_ep_info.spsr = arm_get_spsr_for_bl33_entry();
		bl33_image_ep_info.spsr &= ~0x8;
		bl33_image_ep_info.spsr |= 0x4;
		
		SET_SECURITY_STATE(bl33_image_ep_info.h.attr, NON_SECURE);
	
#if defined(SPD_spmd) && !(ARM_LINUX_KERNEL_AS_BL33)
		/*
		 * Hafnium in normal world expects its manifest address in x0, which
		 * is loaded at base of DRAM.
		 */
		bl33_image_ep_info.args.arg0 = (u_register_t)ARM_DRAM1_BASE;
#endif
	
# if ARM_LINUX_KERNEL_AS_BL33
		/*
		 * According to the file ``Documentation/arm64/booting.txt`` of the
		 * Linux kernel tree, Linux expects the physical address of the device
		 * tree blob (DTB) in x0, while x1-x3 are reserved for future use and
		 * must be 0.
		 */
		bl33_image_ep_info.args.arg0 = (u_register_t)ARM_PRELOADED_DTB_BASE;
		bl33_image_ep_info.args.arg1 = 0U;
		bl33_image_ep_info.args.arg2 = 0U;
		bl33_image_ep_info.args.arg3 = 0U;
# endif
	
#else /* RESET_TO_BL31 */
	
		/*
		 * In debug builds, we pass a special value in 'plat_params_from_bl2'
		 * to verify platform parameters from BL2 to BL31.
		 * In release builds, it's not used.
		 */
		assert(((unsigned long long)plat_params_from_bl2) ==
			ARM_BL31_PLAT_PARAM_VAL);
	
		/*
		 * Check params passed from BL2 should not be NULL,
		 */
		bl_params_t *params_from_bl2 = (bl_params_t *)from_bl2;
		assert(params_from_bl2 != NULL);
		assert(params_from_bl2->h.type == PARAM_BL_PARAMS);
		assert(params_from_bl2->h.version >= VERSION_2);
	
		bl_params_node_t *bl_params = params_from_bl2->head;
	
		/*
		 * Copy BL33 and BL32 (if present), entry point information.
		 * They are stored in Secure RAM, in BL2's address space.
		 */
		while (bl_params != NULL) {
			if (bl_params->image_id == BL32_IMAGE_ID)
				bl32_image_ep_info = *bl_params->ep_info;
	
			if (bl_params->image_id == BL33_IMAGE_ID)
				bl33_image_ep_info = *bl_params->ep_info;
	
			bl_params = bl_params->next_params_info;
		}
	
		if (bl33_image_ep_info.pc == 0U)
			panic();
#endif /* RESET_TO_BL31 */

}
static void npcm845x_security_setup(void)
{

	//TZ windows is from 0xFFFD0000 - FFFCFFFF
	tz_enable_win(MCR_RAM2_AREA4_INDEX, true);
	tz_enable_win(MCR_RAM2_AREA5_INDEX, true);
	tz_enable_win(MCR_RAM2_AREA6_INDEX, true);
	tz_enable_win(MCR_RAM2_AREA7_INDEX, true);
}
/*******************************************************************************
 * Perform any BL31 platform setup common to ARM standard platforms
 ******************************************************************************/
void bl31_platform_setup(void)
{
	/* Initialize the GIC driver, cpu and distributor interfaces */
	plat_gic_driver_init();
	plat_gic_init();

#if RESET_TO_BL31
	/*
	 * Do initial security configuration to allow DRAM/device access
	 * (if earlier BL has not already done so).
	 */
	//plat_arm_security_setup();
	npcm845x_security_setup();

#if defined(PLAT_ARM_MEM_PROT_ADDR)
	arm_nor_psci_do_dyn_mem_protect();
#endif /* PLAT_ARM_MEM_PROT_ADDR */
#else
	
	// In this soluction, we also do teh security initialzation even when BL31 is not in reset vector
	// TBD for Arbel Z1
	npcm845x_security_setup();
#endif /* RESET_TO_BL31 */
	
	/* Enable and initialize the System level generic timer */
	mmio_write_32(ARM_SYS_CNTCTL_BASE + CNTCR_OFF,
			CNTCR_FCREQ(0U) | CNTCR_EN);

	/* Hila: Check how to use his timer instead ! */
	/*
	Allow access to the System counter timer module */
	//arm_configure_sys_timer();

	/* Initialize power controller before setting up topology */
	plat_arm_pwrc_setup();

#if RAS_EXTENSION
	ras_init();
#endif

#if USE_DEBUGFS
	debugfs_init();
#endif /* USE_DEBUGFS */
}

void arm_console_runtime_init(void)
{
	/* Added in order to ignore the orignal weak function */
}
void plat_arm_program_trusted_mailbox(uintptr_t address){
	/*TBD : for now we don't use ARM mailboax ,so that function added for ignoring the weak one*/
}


void __init bl31_plat_arch_setup(void)
{
	npcm845x_bl31_plat_arch_setup();
}


void __init plat_arm_pwrc_setup(void)
{

	//NPCM850 is always powered so no need for power control
}
void __init npcm845x_bl31_plat_arch_setup(void)
{
	const mmap_region_t bl_regions[] = {
		MAP_BL31_TOTAL,
#if RECLAIM_INIT_CODE
Hila: Not compiled 
		MAP_BL_INIT_CODE,
#endif
#if SEPARATE_NOBITS_REGION
Hila: Not compiled 
		MAP_BL31_NOBITS,
#endif
		ARM_MAP_BL_RO,
#if USE_ROMLIB
Hila: Not compiled 
		ARM_MAP_ROMLIB_CODE,
		ARM_MAP_ROMLIB_DATA,
#endif
#if USE_COHERENT_MEM
		ARM_MAP_BL_COHERENT_RAM,
#endif
		ARM_MAP_SHARED_RAM,
#ifdef SECONDARY_BRINGUP
		ARM_MAP_NS_DRAM1,
	#ifdef BL32_BASE
		ARM_MAP_BL32_CORE_MEM
#endif
		{0}
	};
	setup_page_tables(bl_regions, plat_arm_get_mmap());
	enable_mmu_el3(0);

}
