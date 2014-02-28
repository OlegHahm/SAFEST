/*
 * Copyright (C) 2013 INRIA
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     cebit_demo
 * @{
 *
 * @file        udp.c
 * @brief       CeBIT 2014 demo application - light node
 *
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "kernel.h"
#include "thread.h"
#include "destiny/socket.h"
#include "net_help.h"

#include "demo.h"
#include "light.h"
#include "config.h"
#include "viz.h"

#define UDP_BUFFER_SIZE     (128)

char udp_server_stack_buffer[KERNEL_CONF_STACKSIZE_MAIN];
char addr_str[IPV6_MAX_ADDR_STR_LEN];

void init_udp_server(void);

/* UDP server thread */
void udp_server(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    int udp_server_thread_pid = thread_create(udp_server_stack_buffer, KERNEL_CONF_STACKSIZE_MAIN, PRIORITY_MAIN, CREATE_STACKTEST, init_udp_server, "init_udp_server");
    printf("UDP SERVER ON PORT %d (THREAD PID: %d)\n", HTONS(APPLICATION_PORT), udp_server_thread_pid);
}

void init_udp_server(void)
{
    sockaddr6_t sa;
    char buffer_main[UDP_BUFFER_SIZE];
    int32_t recsize;
    uint32_t fromlen;
    int sock = destiny_socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    memset(&sa, 0, sizeof(sa));

    sa.sin6_family = AF_INET;
    sa.sin6_port = HTONS(APPLICATION_PORT);

    fromlen = sizeof(sa);

    if (-1 == destiny_socket_bind(sock, &sa, sizeof(sa))) {
        printf("Error bind failed!\n");
        destiny_socket_close(sock);
    }

    while (1) {
        recsize = destiny_socket_recvfrom(sock, (void *)buffer_main, UDP_BUFFER_SIZE, 0,
                                          &sa, &fromlen);

        if (recsize < 0) {
            printf("ERROR: recsize < 0!\n");
        }

        printf("UDP packet received, payload: %s\n", buffer_main);

        if (recsize >= 3) {
            uint8_t src = sa.sin6_addr.uint8[15];
            light_recv_cmd(src, buffer_main[0], buffer_main[1], buffer_main[2]);

            viz_udp_pkt();
        }
    }

    destiny_socket_close(sock);
}

/* UDP send command */
void udp_send(int argc, char **argv)
{
    int sock;
    sockaddr6_t sa;
    ipv6_addr_t ipaddr;
    int bytes_sent;
    int address;
    char text[5];

    if (argc != 3) {
        printf("usage: send <addr> <text>\n");
        return;
    }

    address = atoi(argv[1]);

    strncpy(text, argv[2], sizeof (text));
    text[sizeof (text) - 1] = 0;

    sock = destiny_socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    if (-1 == sock) {
        printf("Error Creating Socket!");
        return;
    }

    memset(&sa, 0, sizeof sa);

    ipv6_addr_init(&ipaddr, 0xabcd, 0x0, 0x0, 0x0, 0x3612, 0x00ff, 0xfe00, (uint16_t)address);

    sa.sin6_family = AF_INET;
    memcpy(&sa.sin6_addr, &ipaddr, 16);
    sa.sin6_port = HTONS(APPLICATION_PORT);

    bytes_sent = destiny_socket_sendto(sock, (char *)text,
                                       strlen(text) + 1, 0, &sa,
                                       sizeof sa);

    if (bytes_sent < 0) {
        printf("Error sending packet!\n");
    }
    else {
        printf("Successful deliverd %i bytes over UDP to %s to 6LoWPAN\n", bytes_sent, ipv6_addr_to_str(addr_str, &ipaddr));
    }

    destiny_socket_close(sock);
}
