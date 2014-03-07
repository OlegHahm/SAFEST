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
 * @brief       CeBIT 2014 demo application - portal node
 *
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "posix_io.h"
#include "shell.h"
#include "shell_commands.h"
#include "board_uart0.h"
#include "kernel.h"

#include "demo.h"
#include "udpif.h"
#include "portal.h"

const shell_command_t shell_commands[] = {
    {"init", "Initialize network", rpl_udp_init},
    {"set", "Set ID", rpl_udp_set_id},
    {"table", "Shows the routing table", rpl_udp_table},
    {"dodag", "Shows the dodag", rpl_udp_dodag},
    {"loop", "", rpl_udp_loop},
    {"server", "Starts a UDP server", udpif_shell_server},
    {"send", "Send a UDP datagram", udpif_shell_send},
    {"ign", "ignore node", rpl_udp_ignore_cmd},
    {"fw", "fw an event into the net", portal_in},
    {NULL, NULL, NULL}
};


void fill_nc(void)
{
    uint8_t numne = 2;
    uint8_t numig = 4;
    uint16_t neighbors[] = {23, 31};
    uint16_t ignore[] = {32, 33, 41, 51};
    ipv6_addr_t r_addr;
    uint16_t l_addr;

    for (int i = 0; i < numne; i++) {
        l_addr = HTONS(neighbors[i]);
        udpif_get_ipv6_address(&r_addr, l_addr);
        ndp_neighbor_cache_add(0, &r_addr, &l_addr, 2, 0,
                               NDP_NCE_STATUS_REACHABLE, 
                               NDP_NCE_TYPE_TENTATIVE, 
                               0xffff);
    }
    for (int i = 0; i < numig; i++) {
        printf("Ignoring %u\n", ignore[i]);
        rpl_udp_ignore(ignore[i]);
    }

}

int main(void)
{
    puts("CeBIT demo - portal node v"APP_VERSION);

    // fill neighbor cache
    fill_nc();

    /* set the nodes address */
    char *set[] = {"set", NODE_ADDRESS};
    rpl_udp_set_id(2, set);

    /* initialize the node as RPL root node */
    char *init[] = {"init", NODE_MODE};
    rpl_udp_init(2, init);

    /* start a UDP server */
    udpif_start_server(APPLICATION_PORT, portal_out);

    /* start shell */
    posix_open(uart0_handler_pid, 0);
    shell_t shell;
    shell_init(&shell, shell_commands, UART0_BUFSIZE, uart0_readc, uart0_putc);
    shell_run(&shell);

    return 0;
}
