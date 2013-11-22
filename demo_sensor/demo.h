#ifndef DEMO_H
#define DEMO_H

/* RPL shell command handlers */
/**
 * @brief   Shell command to initializes RPL
 *
 * @param[in] str  Shell input
 */
void init(char *str);

/**
 * @brief   Loops through the routing table
 *
 * @param[in] unused  Guess what
 */
void loop(char *unused);

/**
 * @brief   Shows the routing table
 *
 * @param[in] unused  Guess what
 */
void table(char *unused);

/**
 * @brief   Shows the dodag 
 *
 * @param[in] unused  Guess what
 */
void dodag(char *unused);

/* UDP shell command handlers */
void udp_server(char *unused);
void udp_send(char *str);

/* helper command handlers */
void ip(char *unused);

extern radio_address_t id;
extern ipv6_addr_t std_addr;
extern char addr_str[IPV6_MAX_ADDR_STR_LEN];
#endif /* DEMO_H */
