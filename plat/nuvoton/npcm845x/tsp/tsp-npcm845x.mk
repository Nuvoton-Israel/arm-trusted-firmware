#
# Copyright (c) 2014-2023, ARM Limited and Contributors. All rights reserved.
#
# Copyright (c) 2017-2023 Nuvoton Technology Corp.
#
# SPDX-License-Identifier: BSD-3-Clause

# TSP source files specific to Nuvoton platform
BL32_SOURCES		+=	plat/common/aarch64/platform_mp_stack.S		\
				plat/nuvoton/npcm845x/npcm845x_serial_port.c		\
				drivers/nuvoton/common/uart/nuvoton_16550_console.S	\
				plat/nuvoton/npcm845x/tsp/tsp_plat_setup.c
