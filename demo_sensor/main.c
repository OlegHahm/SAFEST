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
    {"ign", "ignore node", ignore},
    {"sense_init", "Initialize the sense module", sense_init},
    {"sense_en", "Enable sensing", sense_enable},
    {"sense_dis", "Disable sensing", sense_disable},
    {"sense_set", "Set seinsing parameters", sense_setparam},
    {NULL, NULL, NULL}
};

int main(void)
{
    puts("SAFEST demo sensor v0.3");

    destiny_init_transport_layer();

    /* start shell */
    posix_open(uart0_handler_pid, 0);

    /* init udp and network stack */
    init("init n 55");
    ignore("ign 16");
    sense_init("dummy");
    sense_enable("dummy");
    LED_GREEN_OFF;
    LED_RED_ON;
    puts("station: FUB");

    shell_t shell;
    shell_init(&shell, shell_commands, uart0_readc, uart0_putc);

    shell_run(&shell);
    return 0;
}
