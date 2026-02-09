#include "net/etharp.h"
#include "net/cmf.h"
#include "net/cmf_opts.h"

static etharp_entry_t arp_type[CMF_ETHARP_ARP_TABLE_SIZE] = {0};

void etharp_timer()
{

}

int32 etharp_init()
{
    return CMF_ERR_OK;
}

int32 etharp_entry_update(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr)
{
    return CMF_ERR_OK;
}

int32 etharp_entry_find(ip_addr_t* pIpAddr, eth_addr_t* pEthAddr)
{
    return CMF_ERR_OK;
}

int32 etharp_entry_update_with_macInfo(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr, etharp_macinfo_t macInfo)
{
    return CMF_ERR_OK;
}

int32 etharp_entry_find_with_macInfo(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr, etharp_macinfo_t *pMacInfo)
{
    return CMF_ERR_OK;
}

int32 etharp_input(pktBuf_t *pBuf)
{
    return CMF_ERR_OK;
}

int32 etharp_check(ip_addr_t ip)
{
    return CMF_ERR_OK;
}

int32 etharp_request(ip_addr_t ip)
{
    return CMF_ERR_OK;
}

int32 etharp_request_vlan(ip_addr_t ip, uint16 vid, int32 tx_port_mask)
{
    return CMF_ERR_OK;
}