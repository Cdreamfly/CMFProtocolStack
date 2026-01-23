#include "net/eth.h"
#include "net/cmf.h"
#include "net/etharp.h"

int32 eth_init(cmf_upper_input_func_t *pUpper_input)
{
    return CMF_OK;
}

int32 eth_input(pktBuf_t *pBuf, netIf_t *pNetIf)
{
    return CMF_OK;
}

int32 eth_output(pktBuf_t *pktBuf, netIf_t *pNetIf)
{
    return CMF_OK;
}

int32 cmf_nic_rxVid_get(uint32 *pVid)
{
    return CMF_OK;
}
int32 cmf_vlan_func_register(cmf_eth_vlan_callback_fun_t *pVlanCfg)
{
    return CMF_OK;
}