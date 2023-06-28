/*
 * Copyright (c) 2015-2023, ARM Limited and Contributors. All rights reserved.
 *
 * Copyright (C) 2017-2023 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __NPCM845X_SIP_SVC_H__
#define __NPCM845X_SIP_SVC_H__

/* SMC function IDs for SiP Service queries */
#define NPCM845X_SIP_CPUFREQ 0xC2000001
#define NPCM845X_SIP_SET_CPUFREQ 0x00

#define NPCM845X_SIP_SRTC 0xC2000002
#define NPCM845X_SIP_SRTC_SET_TIME 0x00

#define NPCM845X_SIP_BUILDINFO 0xC2000003
#define NPCM845X_SIP_BUILDINFO_GET_COMMITHASH 0x00

#define NPCM845X_SIP_GET_SOC_INFO 0xC2000006

#define NPCM845X_SIP_WAKEUP_SRC 0xC2000009
#define NPCM845X_SIP_WAKEUP_SRC_SCU 0x1
#define NPCM845X_SIP_WAKEUP_SRC_IRQSTEER 0x2

#define NPCM845X_SIP_OTP_READ 0xC200000A
#define NPCM845X_SIP_OTP_WRITE 0xC200000B

#define NPCM845X_SIP_MISC_SET_TEMP 0xC200000C

int npcm845x_kernel_entry_handler(uint32_t smc_fid, u_register_t x1,
				  u_register_t x2, u_register_t x3,
				  u_register_t x4);

uint64_t npcm845x_buildinfo_handler(uint32_t smc_fid, u_register_t x1,
				    u_register_t x2, u_register_t x3,
				    u_register_t x4);

#endif /* __NPCM845X_SIP_SVC_H__ */
