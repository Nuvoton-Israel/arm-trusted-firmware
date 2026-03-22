/*
 * Copyright (C) 2017-2023 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __ARBEL_CLOCK_H_
#define __ARBEL_CLOCK_H_

/* CLK_BA = 0xF0801000 */
struct clk_ctl {
	unsigned int	clken1;		/* offset: 0x00, addr: 0xF0801000 */
	unsigned int	clksel;		/* offset: 0x04, addr: 0xF0801004 */
	unsigned int	clkdiv1;	/* offset: 0x08, addr: 0xF0801008 */
	unsigned int	pllcon0;	/* offset: 0x0C, addr: 0xF080100C */
	unsigned int	pllcon1;	/* offset: 0x10, addr: 0xF0801010 */
	unsigned int	swrstr;		/* offset: 0x14, addr: 0xF0801014 */
	unsigned char	res1[0x8];	/* offset: 0x18, addr: 0xF0801018 */
	unsigned int	ipsrst1;	/* offset: 0x20, addr: 0xF0801020 */
	unsigned int	ipsrst2;	/* offset: 0x24, addr: 0xF0801024 */
	unsigned int	clken2;		/* offset: 0x28, addr: 0xF0801028 */
	unsigned int	clkdiv2;	/* offset: 0x2C, addr: 0xF080102C */
	unsigned int	clken3;		/* offset: 0x30, addr: 0xF0801030 */
	unsigned int	ipsrst3;	/* offset: 0x34, addr: 0xF0801034 */
	unsigned int	wd0rcr;		/* offset: 0x38, addr: 0xF0801038 */
	unsigned int	wd1rcr;		/* offset: 0x3C, addr: 0xF080103C */
	unsigned int	wd2rcr;		/* offset: 0x40, addr: 0xF0801040 */
	unsigned int	swrstc1;	/* offset: 0x44, addr: 0xF0801044 */
	unsigned int	swrstc2;	/* offset: 0x48, addr: 0xF0801048 */
	unsigned int	swrstc3;	/* offset: 0x4C, addr: 0xF080104C */
	unsigned int	tiprstc;	/* offset: 0x50, addr: 0xF0801050 */
	unsigned int	pllcon2;	/* offset: 0x54, addr: 0xF0801054 */
	unsigned int	clkdiv3;	/* offset: 0x58, addr: 0xF0801058 */
	unsigned int	corstc;		/* offset: 0x5C, addr: 0xF080105C */
	unsigned int	pllcong;	/* offset: 0x60, addr: 0xF0801060 */
	unsigned int	ahbckfi;	/* offset: 0x64, addr: 0xF0801064 */
	unsigned int	seccnt;		/* offset: 0x68, addr: 0xF0801068 */
	unsigned int	cntr25m;	/* offset: 0x6C, addr: 0xF080106C */
	unsigned int	clken4;		/* offset: 0x70, addr: 0xF0801070 */
	unsigned int	ipsrst4;	/* offset: 0x74, addr: 0xF0801074 */
	unsigned int	busto;		/* offset: 0x78, addr: 0xF0801078 */
	unsigned int	clkdiv4;	/* offset: 0x7C, addr: 0xF080107C */
	unsigned int	wd0rcrb;	/* offset: 0x80, addr: 0xF0801080 */
	unsigned int	wd1rcrb;	/* offset: 0x84, addr: 0xF0801084 */
	unsigned int	wd2rcrb;	/* offset: 0x88, addr: 0xF0801088 */
	unsigned int	swrstc1b;	/* offset: 0x8C, addr: 0xF080108C */
	unsigned int	swrstc2b;	/* offset: 0x90, addr: 0xF0801090 */
	unsigned int	swrstc3b;	/* offset: 0x94, addr: 0xF0801094 */
	unsigned int	tiprstcb;	/* offset: 0x98, addr: 0xF0801098 */
	unsigned int	corstcb;	/* offset: 0x9C, addr: 0xF080109C */
	unsigned int	ipsrstdis1;	/* offset: 0xA0, addr: 0xF08010A0 */
	unsigned int	ipsrstdis2;	/* offset: 0xA4, addr: 0xF08010A4 */
	unsigned int	ipsrstdis3;	/* offset: 0xA8, addr: 0xF08010A8 */
	unsigned int	ipsrstdis4;	/* offset: 0xAC, addr: 0xF08010AC */
	unsigned char	res2[0x10];	/* offset: 0xB0, addr: 0xF08010B0 (CLKENDIS1-4 not used) */
	unsigned int	thrtl_cnt;	/* offset: 0xC0, addr: 0xF08010C0 */
};

#endif /* __ARBEL_CLOCK_H_ */
