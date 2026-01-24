#include "net/netif.h"

netIf_t cmf_netif;

void cmf_netif_info_set(const netIf_t *pNetif)
{
    cmf_netif.mtu = pNetif->mtu;
}

void cmf_netif_info_get(netIf_t *pNetif)
{
    pNetif->mtu = cmf_netif.mtu;
}