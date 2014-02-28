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
 * @brief       CeBIT 2014 demo application - light node
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
#include "light.h"
#include "config.h"

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
    {"color", "Set a color [r] [g] [b]", light_set_shell},
    {"light-off", "Turn off the light", light_off_shell},
    {NULL, NULL, NULL}
};


int main(void)
{
    puts("RPL router v"APP_VERSION);

    /* set the nodes address */
    char *set[] = {"set", NODE_ADDRESS};
    rpl_udp_set_id(2, set);

    /* start the node as normal RPL node */
    char *init[] = {"init", NODE_MODE};
    rpl_udp_init(2, init);

    /* start the UDP server */
    char *server[] = {"server"};
    udp_server(1, server);

    /* initialize the light */
    light_init();

    /* start shell */
    posix_open(uart0_handler_pid, 0);

    shell_t shell;
    shell_init(&shell, shell_commands, UART0_BUFSIZE, uart0_readc, uart0_putc);

    shell_run(&shell);
    return 0;
}
