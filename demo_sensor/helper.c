#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msg.h"
#include "sixlowpan/ip.h"
#include "transceiver.h"
#include "ieee802154_frame.h"
#include "rpl_structs.h"

#include "demo.h"
/* prints current IPv6 adresses */
void ip(char *unused)
{
    (void) unused;
    ipv6_iface_print_addrs();
}

transceiver_command_t tcmd;

void ignore(char *addr) {
    uint16_t a;
    msg_t mesg;
    mesg.type = DBG_IGN;
    mesg.content.ptr = (char*) &tcmd;

    tcmd.transceivers = TRANSCEIVER_CC1100;
    tcmd.data = &a;
    a = atoi(addr + strlen("ign "));
    if (strlen(addr) > strlen("ign ")) {
        printf("sending to transceiver (%u): %u\n", transceiver_pid, (*(uint8_t*)tcmd.data));
        msg_send(&mesg, transceiver_pid, 1);
    }
    else {
        puts("Usage:\tign <addr>");
    }
}
