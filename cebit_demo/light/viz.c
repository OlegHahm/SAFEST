/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cebit_demo
 * @{
 *
 * @file        viz.c
 * @brief       CeBIT 2014 demo application - light node
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "viz.h"
#include "config.h"
#include "events.h"


void viz_udp_pkt(void)
{
// #ifdef VIZ_REMOTE

// #else

// #endif
    printf("VIZ: UPD pkt\n");
}

void viz_parent_select(uint8_t parent)
{
    printf("VIZ: RPL parent selected: %i\n", parent);
}

void viz_parent_deselect(uint8_t parent)
{
    printf("VIZ: RPL parent deselect: %i\n", parent);
}

void viz_dio(uint8_t src)
{
    printf("VIZ: RPL dio from %i\n", src);
}