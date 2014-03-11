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
#include "events.h"
#include "demo.h"
#include "udpif.h"

//static uint8_t sequ_list[5][10];

static uint8_t next_sequ = 1;


extern void _reboot_handler(int argc, char **argv);
void handle_event(char id, char data);


void portal_in(int argc, char** argv)
{
    printf("Route command: %s, params: %i\n", argv[0], argc);
    if (argc == 4) {
        uint16_t dst = (uint16_t)atoi(argv[1]);
        uint16_t addr = (uint16_t)atoi(NODE_ADDRESS);
        char id = (char)atoi(argv[2]);
        char data = (char)atoi(argv[3]);

        if (dst == addr) {
            handle_event(id, data);
        } else {
            char sendbuf[3];
            sendbuf[0] = id;
            sendbuf[1] = data;
            sendbuf[2] = (char)next_sequ++;         // seq
            printf("Sending type%i data:%i to %i\n", sendbuf[0], sendbuf[1], dst);
            udpif_send(dst, APPLICATION_PORT, sendbuf, 3);
        }
    } else {
        puts("FW package has wrong format");
    }
}

void portal_out(uint16_t src, char* data, int length)
{
    uint8_t id;
    uint8_t payload;
    //uint8_t sequ;

    if (length == 3) {
        id = (uint8_t)data[0];
        payload = (uint8_t)data[1];
        //sequ = (uint8_t)data[2]; --> TODO implement sequence number check
        printf("bw %i %i %i\n", src, id, payload);
    } else {
        printf("UDP: unidentified data, from %i, length: %i\n", src, length);
    }
}

void handle_event(char id, char data)
{
    char *reboot[] = {"reboot"};
    switch(id) {
        case RESET:
            _reboot_handler(1, reboot);
        break;
    }
}
