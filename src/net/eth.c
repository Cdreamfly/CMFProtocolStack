#include "net/eth.h"
#include "net/pktBuf.h"
#include "net/cmf.h"
#include "net/etharp.h"
#include "net/cmf_opts.h"
#include "common/cmf_debug.h"

static uint32 current_vid = CMF_NIC_NULL_VID;
static cmf_eth_vid_pm_get_f cmf_vlan_pvid_pm_get;
static cmf_ip4_input_f cmf_ip4_input;
static cmf_etharp_input_f cmf_etharp_input;

static void _eth_hdr_dump(eth_hdr_t* pEthHdr)
{
    CMF_DEBUG(CMF_DBG_ETH | DBG_LV_DEBUG, "-- [ETH HDR dump] -----------------------------\n");
    CMF_DEBUG(CMF_DBG_ETH | DBG_LV_DEBUG, " Dst-MAC: %02X:%02X:%02X:%02X:%02X:%02X \n",
              pEthHdr->dst_mac.addr[0],
              pEthHdr->dst_mac.addr[1],
              pEthHdr->dst_mac.addr[2],
              pEthHdr->dst_mac.addr[3],
              pEthHdr->dst_mac.addr[4],
              pEthHdr->dst_mac.addr[5]);
    CMF_DEBUG(CMF_DBG_ETH | DBG_LV_DEBUG, " Src-MAC: %02X:%02X:%02X:%02X:%02X:%02X \n",
              pEthHdr->src_mac.addr[0],
              pEthHdr->src_mac.addr[1],
              pEthHdr->src_mac.addr[2],
              pEthHdr->src_mac.addr[3],
              pEthHdr->src_mac.addr[4],
              pEthHdr->src_mac.addr[5]);
    CMF_DEBUG(CMF_DBG_ETH | DBG_LV_DEBUG, " Ethtype: 0x%04X\n", pEthHdr->ether_type);
    CMF_DEBUG(CMF_DBG_ETH | DBG_LV_DEBUG, "-----------------------------------------------\n");
}


int32 eth_init(const cmf_upper_input_func_t* pUpper_input)
{
    CMF_PARAM_CHK("pUpper_input NULL", NULL == pUpper_input, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("ip4_input NULL", NULL == pUpper_input->ip4_input, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("arp_input NULL", NULL == pUpper_input->arp_input, return CMF_ERR_NULLPOINT);
    cmf_ip4_input = pUpper_input->ip4_input;
    cmf_etharp_input = pUpper_input->arp_input;
    return CMF_OK;
}

int32 eth_input(pktBuf_t* pBuf, netIf_t* pNetIf)
{
    CMF_PARAM_CHK("ethernet packet dropped, too short", pBuf->length < sizeof(eth_hdr_t), return CMF_ERR);
    pBuf->l2_hdr = pBuf->data;
    eth_hdr_t* pEthHdr = (eth_hdr_t*)pBuf->l2_hdr;
    _eth_hdr_dump(pEthHdr);

    pBuf->vlan_ctagif = 0;
    switch (pEthHdr->ether_type)
    {
    case ETH_TYPE_IP:
        break;
    case ETH_TYPE_ARP:
        break;
    default:
        cmf_pbuf_free(pBuf);
        return CMF_ERR_NOT_SUPPORT;
    }
    return CMF_OK;
}

int32 eth_output(pktBuf_t* pktBuf, netIf_t* pNetIf)
{
    return CMF_OK;
}

int32 cmf_nic_rxVid_get(uint32* pVid)
{
    *pVid = current_vid;
    return CMF_OK;
}

int32 cmf_vlan_func_register(const cmf_eth_vlan_callback_fun_t* pVlanCfg)
{
    CMF_PARAM_CHK("pVlanCfg NULL", NULL == pVlanCfg, return CMF_ERR_NULLPOINT);
    cmf_vlan_pvid_pm_get = pVlanCfg->eth_vid_pm_get;
    return CMF_OK;
}
