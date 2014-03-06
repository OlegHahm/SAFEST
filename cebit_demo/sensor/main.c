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
 * @file        main.c
 * @brief       CeBIT 2014 demo application - sensor node
 *
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "net_if.h"
#include "posix_io.h"
#include "shell.h"
#include "shell_commands.h"
#include "board_uart0.h"
#include "kernel.h"

#include "demo.h"
#include "udpif.h"
#include "sense.h"

const shell_command_t shell_commands[] = {
    {"init", "Initialize network", rpl_udp_init},
    {"set", "Set ID", rpl_udp_set_id},
    {"table", "Shows the routing table", rpl_udp_table},
    {"dodag", "Shows the dodag", rpl_udp_dodag},
    {"loop", "", rpl_udp_loop},
    {"server", "Starts a UDP server", udpif_shell_server},
    {"send", "Send a UDP datagram", udpif_shell_send},
    {"ign", "ignore node", rpl_udp_ignore},
    {NULL, NULL, NULL}
};


void fill_nc(void)
{
    uint8_t numof = 5;
    uint16_t neighbors[] = {23, 31, 32, 33, 51};
    ipv6_addr_t r_addr;
    uint16_t l_addr;

    for (int i = 0; i < numof; i++) {
        l_addr = neighbors[i];
        udpif_get_ipv6_address(&r_addr, l_addr);
        ndp_neighbor_cache_add(0, &r_addr, &l_addr, 2, (l_addr == 23),
                               NDP_NCE_STATUS_REACHABLE, 
                               NDP_NCE_TYPE_TENTATIVE, 
                               0xffff);
    }
}

int main(void)
{
    puts("CeBIT demo - sensor node v" APP_VERSION);

    // fill neighbor cache
    fill_nc();

    /* set the nodes address */
    char *set[] = {"set", NODE_ADDRESS};
    rpl_udp_set_id(2, set);

    /* initialize node as normal RPL node */
    char *init[] = {"init", NODE_MODE};
    rpl_udp_init(2, init);

    // /* set ignore nodes */
    // char *nodes[] = IGNORE_NODES;
    // for (int i = 0; i < IGNORE_NUMOF; i++) {
    //     char *ign[] = {"ign", nodes[i]};
    //     rpl_udp_ignore(2, ign);
    // }

    /* start looking for accelerometer events */
    sense_init();

    /* start shell */
    posix_open(uart0_handler_pid, 0);
    shell_t shell;
    shell_init(&shell, shell_commands, UART0_BUFSIZE, uart0_readc, uart0_putc);
    shell_run(&shell);

    return 0;
}
