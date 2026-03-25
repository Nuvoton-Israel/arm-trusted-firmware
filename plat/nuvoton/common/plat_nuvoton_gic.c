/*
 * Copyright (c) 2016-2023, ARM Limited and Contributors. All rights reserved.
 *
 * Copyright (C) 2022-2023 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <common/interrupt_props.h>
#include <drivers/arm/gicv2.h>
#include <plat/common/platform.h>
#include <platform_def.h>
#include <common/debug.h>

static const interrupt_prop_t g0_interrupt_props[] = {
	INTR_PROP_DESC(FIQ_SMP_CALL_SGI, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_EDGE),
	/* WD2 pre-timeout: fires 1024 prescale clocks before watchdog reset */
	INTR_PROP_DESC(NPCM845X_WDG_INT2, GIC_HIGHEST_SEC_PRIORITY,
			GICV2_INTR_GROUP0, GIC_INTR_CFG_LEVEL),
};

gicv2_driver_data_t arm_gic_data = {
	.gicd_base = BASE_GICD_BASE,
	.gicc_base = BASE_GICC_BASE,
	.interrupt_props = g0_interrupt_props,
	.interrupt_props_num = ARRAY_SIZE(g0_interrupt_props),
};

void plat_gic_driver_init(void)
{
	INFO("Function: %s:%d\n", __func__, __LINE__);
	gicv2_driver_init(&arm_gic_data);
	INFO("Function: %s:%d\n", __func__, __LINE__);
}

void plat_gic_init(void)
{
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
}

void plat_gic_cpuif_enable(void)
{
	gicv2_cpuif_enable();
}

void plat_gic_cpuif_disable(void)
{
	gicv2_cpuif_disable();
}

void plat_gic_pcpu_init(void)
{
	gicv2_pcpu_distif_init();
}
