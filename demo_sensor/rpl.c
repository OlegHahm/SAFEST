#include <stdio.h>
#include <string.h>
#include "vtimer.h"
#include "thread.h"
#include "sixlowpan.h"
#include "destiny.h"
#include "rpl.h"
#include "rpl_dodag.h"
#include "demo.h"

radio_address_t id;
ipv6_addr_t std_addr;

void init(char *str)
{
    transceiver_command_t tcmd;
    msg_t m;
    uint8_t chan = 10;

    char command;

    int res = sscanf(str, "init %c %hu", &command, &id);

    if (res < 1) {
        printf("Usage: init address\n");
        printf("\tr\tinitialize as root\n");
        printf("\tn\tinitialize as node router\n");
        printf("\taddress must be an 8 bit integer\n");
    }

    uint8_t state;

    if ((command == 'n') || (command == 'r')) {
        printf("INFO: Initialize as %s on address %d\n", ((command == 'n') ? "node" : "root"), id);
        if (id > 255) {
            printf("ERROR: address not an 8 bit integer\n");
            return;
        }

        state = rpl_init(TRANSCEIVER_CC1100, id);

        if (state != SIXLOWERROR_SUCCESS) {
            printf("Error initializing RPL\n");
        }

        if (command == 'r') {
            rpl_init_root();
        }
    }
    else {
        printf("ERROR: Unknown command '%c'\n", command);
        return;
    }

    /* TODO: check if this works as intended */
    ipv6_addr_t prefix, tmp;
    ipv6_addr_init(&std_addr, 0xABCD, 0xEF12, 0, 0, 0x1034, 0x00FF, 0xFE00, id);
    ipv6_addr_init_prefix(&prefix, &std_addr, 64);
    plist_add(&prefix, 64, NDP_OPT_PI_VLIFETIME_INFINITE, 0, 1, ICMPV6_NDP_OPT_PI_FLAG_AUTONOM);
    ipv6_init_iface_as_router();
    /* add global address */
    ipv6_addr_set_by_eui64(&tmp, &std_addr);
    ipv6_iface_add_addr(&tmp, IPV6_ADDR_TYPE_GLOBAL, NDP_ADDR_STATE_PREFERRED, 0, 0);
            
    /* set channel to 10 */
    tcmd.transceivers = TRANSCEIVER_CC1100;
    tcmd.data = &chan;
    m.type = SET_CHANNEL;
    m.content.ptr = (void *) &tcmd;

    msg_send_receive(&m, &m, transceiver_pid);

    destiny_init_transport_layer();
    puts("Destiny initialized");
    /* start transceiver watchdog */
}

void loop(char *unused)
{
    (void) unused;

    rpl_routing_entry_t *rtable;

    while (1) {
        rtable = rpl_get_routing_table();
        rpl_dodag_t *mydodag = rpl_get_my_dodag();

        if (mydodag == NULL) {
            vtimer_usleep(20 * 1000 * 1000);
            continue;
        }

        printf("---------------------------\n");
        printf("OUTPUT\n");
        printf("my rank: %d\n", mydodag->my_rank);
        printf("my preferred parent:\n");
        printf("%s\n", ipv6_addr_to_str(addr_str, (&mydodag->my_preferred_parent->addr)));
        printf("parent lifetime: %d\n", mydodag->my_preferred_parent->lifetime);
        printf("---------------------------$\n");

        for (int i = 0; i < RPL_MAX_ROUTING_ENTRIES; i++) {
            if (rtable[i].used) {
                printf("%s\n", ipv6_addr_to_str(addr_str, (&rtable[i].address)));
                puts("next hop");
                printf("%s\n", ipv6_addr_to_str(addr_str, (&rtable[i].next_hop)));
                printf("entry %d lifetime %d\n", i, rtable[i].lifetime);

                if (!rpl_equal_id(&rtable[i].address, &rtable[i].next_hop)) {
                    puts("multi-hop");
                }

                printf("---------------------------$\n");
            }
        }

        printf("########################\n");
        vtimer_usleep(20 * 1000 * 1000);
    }


}

void table(char *unused)
{
    (void) unused;

    rpl_routing_entry_t *rtable;
    rtable = rpl_get_routing_table();
    printf("---------------------------\n");
    printf("OUTPUT\n");
    printf("---------------------------\n");

    for (int i = 0; i < RPL_MAX_ROUTING_ENTRIES; i++) {
        if (rtable[i].used) {
            printf("%s\n", ipv6_addr_to_str(addr_str, (&rtable[i].address)));
            printf("entry %d lifetime %d\n", i, rtable[i].lifetime);

            if (!rpl_equal_id(&rtable[i].address, &rtable[i].next_hop)) {
                puts("multi-hop");
            }

            printf("--------------\n");
        }
    }

    printf("$\n");
}

void dodag(char *unused)
{
    (void) unused;

    printf("---------------------------\n");
    rpl_dodag_t *mydodag = rpl_get_my_dodag();

    if (mydodag == NULL) {
        printf("Not part of a dodag\n");
        printf("---------------------------$\n");
        return;
    }

    printf("Part of Dodag:\n");
    printf("%s\n", ipv6_addr_to_str(addr_str, (&mydodag->dodag_id)));
    printf("my rank: %d\n", mydodag->my_rank);
    printf("my preferred parent:\n");
    printf("%s\n", ipv6_addr_to_str(addr_str, (&mydodag->my_preferred_parent->addr)));
    printf("---------------------------$\n");
}
