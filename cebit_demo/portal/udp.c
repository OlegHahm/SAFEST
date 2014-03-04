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
 * @brief       CeBit 2014 demo application - portal node
 *
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
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

#define UDP_BUFFER_SIZE     (128)

char udp_server_stack_buffer[KERNEL_CONF_STACKSIZE_MAIN];
char addr_str[IPV6_MAX_ADDR_STR_LEN];

static int socket = 0;



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
    }

    destiny_socket_close(sock);
}

void udp_init(void)
{


    // open a new UDP socket
    socket = destiny_socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    if (socket == -1) {
        printf("Error creating sending socket!");
        return;
    }



}

void udp_send_short(uint8_t dst_addr, char *data, size_t lenght)
{
    ipv6_addr_t dst;

    get_ip_address(&dst, dst_addr);

}

void get_ip_address(ipv6_addr_t *addr, uint8_t local_addr)
{
    ipv6_addr_init(&ipaddr, 0xabcd, 0x0, 0x0, 0x0, 0x3612, 0x00ff, 0xfe00, (uint16_t)local_addr);
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
        printf("Successful delivered %i bytes over UDP to %s to 6LoWPAN\n", 
               bytes_sent, ipv6_addr_to_str(addr_str, IPV6_MAX_ADDR_STR_LEN, &ipaddr));
    }

    destiny_socket_close(sock);
}
