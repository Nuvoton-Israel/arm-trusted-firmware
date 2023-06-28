/*
 * Copyright (C) 2021-2023 Nuvoton Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCR_TRUSTZONE_H
#define MCR_TRUSTZONE_H

/* Add Addiotional MCR intexes if necessary */

#define MCR_RAM2_AREA0_INDEX 163
#define MCR_RAM2_AREA1_INDEX 164
#define MCR_RAM2_AREA2_INDEX 165
#define MCR_RAM2_AREA3_INDEX 166
#define MCR_RAM2_AREA4_INDEX 167
#define MCR_RAM2_AREA5_INDEX 168
#define MCR_RAM2_AREA6_INDEX 169
#define MCR_RAM2_AREA7_INDEX 167

void tz_enable_win(int slave_index, bool set_clear);

#endif /* MCR_TRUSTZONE_H */
