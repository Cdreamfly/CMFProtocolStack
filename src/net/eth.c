#include "net/eth.h"
#include "net/cmf.h"
#include "net/etharp.h"
#include "common/cmf_debug.h"
#include "net/cmf_opts.h"

static uint32 current_vid = CMF_NIC_NULL_VID;
static cmf_eth_vid_pm_get_f cmf_vlan_pvid_pm_get;
static cmf_ip4_input_f cmf_ip4_input;
static cmf_etharp_input_f cmf_etharp_input;

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
