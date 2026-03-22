/*
 * Copyright (C) 2022-2023 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __NPCM845x_GCR_H_
#define __NPCM845x_GCR_H_

/* GCR_BA = 0xF0800000 */
struct npcm845x_gcr {
	unsigned int pdid;		/* offset: 0x000, addr: 0xF0800000 */
	unsigned int pwron;		/* offset: 0x004, addr: 0xF0800004 */
	unsigned int swstrps;		/* offset: 0x008, addr: 0xF0800008 */
	unsigned int rsvd1[2];		/* offset: 0x00C, addr: 0xF080000C */
	unsigned int miscpe;		/* offset: 0x014, addr: 0xF0800014 */
	unsigned int spldcnt;		/* offset: 0x018, addr: 0xF0800018 */
	unsigned int rsvd2[1];		/* offset: 0x01C, addr: 0xF080001C */
	unsigned int flockr2;		/* offset: 0x020, addr: 0xF0800020 */
	unsigned int flockr3;		/* offset: 0x024, addr: 0xF0800024 */
	unsigned int rsvd3[3];		/* offset: 0x028, addr: 0xF0800028 (VSNSCALR at 0x028 not used) */
	unsigned int a35_mode;		/* offset: 0x034, addr: 0xF0800034 */
	unsigned int spswc;		/* offset: 0x038, addr: 0xF0800038 */
	unsigned int intcr;		/* offset: 0x03C, addr: 0xF080003C */
	unsigned int intsr;		/* offset: 0x040, addr: 0xF0800040 */
	unsigned int obscr1;		/* offset: 0x044, addr: 0xF0800044 */
	unsigned int obsdr1;		/* offset: 0x048, addr: 0xF0800048 */
	unsigned int rsvd4[1];		/* offset: 0x04C, addr: 0xF080004C */
	unsigned int hifcr;		/* offset: 0x050, addr: 0xF0800050 */
	unsigned int rsvd5[3];		/* offset: 0x054, addr: 0xF0800054 */
	unsigned int intcr2;		/* offset: 0x060, addr: 0xF0800060 */
	unsigned int rsvd6[1];		/* offset: 0x064, addr: 0xF0800064 */
	unsigned int srcnt;		/* offset: 0x068, addr: 0xF0800068 */
	unsigned int ressr;		/* offset: 0x06C, addr: 0xF080006C */
	unsigned int rlockr1;		/* offset: 0x070, addr: 0xF0800070 */
	unsigned int flockr1;		/* offset: 0x074, addr: 0xF0800074 */
	unsigned int dscnt;		/* offset: 0x078, addr: 0xF0800078 */
	unsigned int mdlr;		/* offset: 0x07C, addr: 0xF080007C */
	unsigned int scrpad_c;		/* offset: 0x080, addr: 0xF0800080 */
	unsigned int scrpad_b;		/* offset: 0x084, addr: 0xF0800084 */
	unsigned int rsvd7[4];		/* offset: 0x088, addr: 0xF0800088 */
	unsigned int daclvlr;		/* offset: 0x098, addr: 0xF0800098 */
	unsigned int intcr3;		/* offset: 0x09C, addr: 0xF080009C */
	unsigned int pcirctl;		/* offset: 0x0A0, addr: 0xF08000A0 */
	unsigned int rsvd8[2];		/* offset: 0x0A4, addr: 0xF08000A4 */
	unsigned int vsintr;		/* offset: 0x0AC, addr: 0xF08000AC */
	unsigned int rsvd9[1];		/* offset: 0x0B0, addr: 0xF08000B0 */
	unsigned int sd2sur1;		/* offset: 0x0B4, addr: 0xF08000B4 */
	unsigned int sd2sur2;		/* offset: 0x0B8, addr: 0xF08000B8 */
	unsigned int sd2irv3;		/* offset: 0x0BC, addr: 0xF08000BC */
	unsigned int intcr4;		/* offset: 0x0C0, addr: 0xF08000C0 */
	unsigned int obscr2;		/* offset: 0x0C4, addr: 0xF08000C4 */
	unsigned int obsdr2;		/* offset: 0x0C8, addr: 0xF08000C8 */
	unsigned int rsvd10[5];		/* offset: 0x0CC, addr: 0xF08000CC */
	unsigned int i2csegsel;		/* offset: 0x0E0, addr: 0xF08000E0 */
	unsigned int i2csegctl;		/* offset: 0x0E4, addr: 0xF08000E4 */
	unsigned int vsrcr;		/* offset: 0x0E8, addr: 0xF08000E8 */
	unsigned int mlockr;		/* offset: 0x0EC, addr: 0xF08000EC */
	unsigned int rsvd11[8];		/* offset: 0x0F0, addr: 0xF08000F0 */
	unsigned int etsr;		/* offset: 0x110, addr: 0xF0800110 */
	unsigned int dft1r;		/* offset: 0x114, addr: 0xF0800114 */
	unsigned int dft2r;		/* offset: 0x118, addr: 0xF0800118 */
	unsigned int dft3r;		/* offset: 0x11C, addr: 0xF080011C */
	unsigned int edffsr;		/* offset: 0x120, addr: 0xF0800120 */
	unsigned int rsvd12[1];		/* offset: 0x124, addr: 0xF0800124 */
	unsigned int intcrpce3;		/* offset: 0x128, addr: 0xF0800128 */
	unsigned int intcrpce2;		/* offset: 0x12C, addr: 0xF080012C */
	unsigned int intcrpce0;		/* offset: 0x130, addr: 0xF0800130 */
	unsigned int intcrpce1;		/* offset: 0x134, addr: 0xF0800134 */
	unsigned int dactest;		/* offset: 0x138, addr: 0xF0800138 */
	unsigned int scrpad;		/* offset: 0x13C, addr: 0xF080013C */
	unsigned int usb1phyctl;	/* offset: 0x140, addr: 0xF0800140 */
	unsigned int usb2phyctl;	/* offset: 0x144, addr: 0xF0800144 */
	unsigned int usb3phyctl;	/* offset: 0x148, addr: 0xF0800148 */
	unsigned int intsr2;		/* offset: 0x14C, addr: 0xF080014C */
	unsigned int intcrpce2b;	/* offset: 0x150, addr: 0xF0800150 */
	unsigned int intcrpce0b;	/* offset: 0x154, addr: 0xF0800154 */
	unsigned int intcrpce1b;	/* offset: 0x158, addr: 0xF0800158 */
	unsigned int intcrpce3b;	/* offset: 0x15C, addr: 0xF080015C */
	unsigned int rsvd13[4];		/* offset: 0x160, addr: 0xF0800160 */
	unsigned int intcrpce2c;	/* offset: 0x170, addr: 0xF0800170 */
	unsigned int intcrpce0c;	/* offset: 0x174, addr: 0xF0800174 */
	unsigned int intcrpce1c;	/* offset: 0x178, addr: 0xF0800178 */
	unsigned int intcrpce3c;	/* offset: 0x17C, addr: 0xF080017C */
	unsigned int rsvd14[40];	/* offset: 0x180, addr: 0xF0800180 */
	unsigned int sd2irv4;		/* offset: 0x220, addr: 0xF0800220 */
	unsigned int sd2irv5;		/* offset: 0x224, addr: 0xF0800224 */
	unsigned int sd2irv6;		/* offset: 0x228, addr: 0xF0800228 */
	unsigned int sd2irv7;		/* offset: 0x22C, addr: 0xF080022C */
	unsigned int sd2irv8;		/* offset: 0x230, addr: 0xF0800230 */
	unsigned int sd2irv9;		/* offset: 0x234, addr: 0xF0800234 */
	unsigned int sd2irv10;		/* offset: 0x238, addr: 0xF0800238 */
	unsigned int sd2irv11;		/* offset: 0x23C, addr: 0xF080023C */
	unsigned int rsvd15[8];		/* offset: 0x240, addr: 0xF0800240 */
	unsigned int mfsel1;		/* offset: 0x260, addr: 0xF0800260 */
	unsigned int mfsel2;		/* offset: 0x264, addr: 0xF0800264 */
	unsigned int mfsel3;		/* offset: 0x268, addr: 0xF0800268 */
	unsigned int mfsel4;		/* offset: 0x26C, addr: 0xF080026C */
	unsigned int mfsel5;		/* offset: 0x270, addr: 0xF0800270 */
	unsigned int mfsel6;		/* offset: 0x274, addr: 0xF0800274 */
	unsigned int mfsel7;		/* offset: 0x278, addr: 0xF0800278 */
	unsigned int rsvd16[1];		/* offset: 0x27C, addr: 0xF080027C */
	unsigned int mfsel_lk1;		/* offset: 0x280, addr: 0xF0800280 */
	unsigned int mfsel_lk2;		/* offset: 0x284, addr: 0xF0800284 */
	unsigned int mfsel_lk3;		/* offset: 0x288, addr: 0xF0800288 */
	unsigned int mfsel_lk4;		/* offset: 0x28C, addr: 0xF080028C */
	unsigned int mfsel_lk5;		/* offset: 0x290, addr: 0xF0800290 */
	unsigned int mfsel_lk6;		/* offset: 0x294, addr: 0xF0800294 */
	unsigned int mfsel_lk7;		/* offset: 0x298, addr: 0xF0800298 */
	unsigned int rsvd17[1];		/* offset: 0x29C, addr: 0xF080029C */
	unsigned int mfsel_set1;	/* offset: 0x2A0, addr: 0xF08002A0 */
	unsigned int mfsel_set2;	/* offset: 0x2A4, addr: 0xF08002A4 */
	unsigned int mfsel_set3;	/* offset: 0x2A8, addr: 0xF08002A8 */
	unsigned int mfsel_set4;	/* offset: 0x2AC, addr: 0xF08002AC */
	unsigned int mfsel_set5;	/* offset: 0x2B0, addr: 0xF08002B0 */
	unsigned int mfsel_set6;	/* offset: 0x2B4, addr: 0xF08002B4 */
	unsigned int mfsel_set7;	/* offset: 0x2B8, addr: 0xF08002B8 */
	unsigned int rsvd18[1];		/* offset: 0x2BC, addr: 0xF08002BC */
	unsigned int mfsel_clr1;	/* offset: 0x2C0, addr: 0xF08002C0 */
	unsigned int mfsel_clr2;	/* offset: 0x2C4, addr: 0xF08002C4 */
	unsigned int mfsel_clr3;	/* offset: 0x2C8, addr: 0xF08002C8 */
	unsigned int mfsel_clr4;	/* offset: 0x2CC, addr: 0xF08002CC */
	unsigned int mfsel_clr5;	/* offset: 0x2D0, addr: 0xF08002D0 */
	unsigned int mfsel_clr6;	/* offset: 0x2D4, addr: 0xF08002D4 */
	unsigned int mfsel_clr7;	/* offset: 0x2D8, addr: 0xF08002D8 */
};

#endif
