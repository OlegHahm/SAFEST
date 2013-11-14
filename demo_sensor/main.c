#include <stdio.h>

#include "posix_io.h"
#include "shell.h"
#include "shell_commands.h"
#include "board_uart0.h"
#include "destiny.h"
#include "transceiver.h"
#include "debug.h"

#include "demo.h"
#include "sense.h"



const shell_command_t shell_commands[] = {
    {"init", "", init},
    {"table", "", table},
    {"dodag", "", dodag},
    {"loop", "", loop},
    {"server", "Starts a UDP server", udp_server},
    {"send", "Send a UDP datagram", udp_send},
    {"ip", "Print all assigned IP addresses", ip},
    {"sense_init", "Initialize the sense module", sense_init},
    {"sense_en", "Enable sensing", sense_enable},
    {"sense_dis", "Disable sensing", sense_disable},
    {NULL, NULL, NULL}
};

int main(void)
{
    puts("SAFEST demo sensor v0.1");
    DEBUG("Hello debug");

    printf("Transceiver tid: %i\n", transceiver_pid);
    transceiver_init(TRANSCEIVER_CC1100);
    transceiver_start();
    printf("Transceiver tid: %i\n", transceiver_pid);

    destiny_init_transport_layer();

    /* start shell */
    posix_open(uart0_handler_pid, 0);


    shell_t shell;
    shell_init(&shell, shell_commands, uart0_readc, uart0_putc);

    shell_run(&shell);
    return 0;
}
