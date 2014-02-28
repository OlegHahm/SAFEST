/*
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup examples
 * @{
 *
 * @file        main.c
 * @brief       CeBIT 2014 demo application - router node
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
#include "destiny.h"
#include "kernel.h"

#include "demo.h"

const shell_command_t shell_commands[] = {
    {"init", "Initialize network", rpl_udp_init},
    {"set", "Set ID", rpl_udp_set_id},
    {"table", "Shows the routing table", rpl_udp_table},
    {"dodag", "Shows the dodag", rpl_udp_dodag},
    {"loop", "", rpl_udp_loop},
    {"server", "Starts a UDP server", udp_server},
    {"send", "Send a UDP datagram", udp_send},
    {"ip", "Print all assigned IP addresses", rpl_udp_ip},
    {"ign", "ignore node", rpl_udp_ignore},
    {NULL, NULL, NULL}
};

void bootstrap_node(void)
{
    // the the nodes address
    char *set[] = {"set", NODE_ADDRESS};
    rpl_udp_set_id(2, set);
#if IGNORE_NUMOF
    char ignore[] = IGNORE_NODES;
    char *ign[2];
    ign[0] = "ign";
    for (int i = 0; i < IGNORE_NUMOF; i++) {
        ign[1] = ignore[i];
        rpl_udp_ignore(2, ign);
    }
#endif
    char *init[] = {"init", NODE_MODE};
    rpl_udp_init(2, init);
#if NODE_ISSERVER
    char *server[] = {"server"};
    udp_server(1, server);
#endif
}

int main(void)
{
    puts("CeBIT router v"APP_VERSION);

    /* bootstrap the network stack */
    bootstrap_node();

    /* start shell */
    posix_open(uart0_handler_pid, 0);
    shell_t shell;
    shell_init(&shell, shell_commands, UART0_BUFSIZE, uart0_readc, uart0_putc);

    shell_run(&shell);
    return 0;
}
