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
 * @file        portal.c
 * @brief       CeBIT 2014 demo application - portal node
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "portal.h"


void portal_in(int argc, char** argv)
{
    printf("Route command: %s, params: %i\n", argv[0], argc);
    if (argc == 4) {
        uint8_t dst = atoi(argv[1]);
        uint8_t type = atoi(argv[2]);
        uint8_t data = atoi(argv[3]);

        printf("Sending type%i data:%i to %i\n", type, data, dst);
    } else {
        puts("FW package has wrong format");
    }
}

void portal_out(int src, int type, int data)
{
    printf("fw %i %i %i", src, type, data);
}
