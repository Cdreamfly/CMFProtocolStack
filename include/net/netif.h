#ifndef NETIF_H
#define NETIF_H

#include "common/def.h"

typedef struct netIf_s
{
    eth_addr_t mac_addr;
    ip_addr_t ip_addr;
    ip_addr_t ip_netmask;
    ip_addr_t ip_gateway;
    ip_addr_t ip_network;
    ip_addr_t ip_broadcast;
    ip_addr_t ip_dns_server;
    uint16 mtu;
    uint16 reserved;
} netIf_t;


extern void cmf_netif_info_set(netIf_t *pNetif);
extern void cmf_netif_info_get(netIf_t *pNetif);


#endif //NETIF_H
