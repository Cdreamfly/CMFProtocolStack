#include "net/cmf.h"
#include "common/cmf_debug.h"

int32 cmf_pro_module_init()
{
    cmf_upper_input_func_t eth_upper_input;
    eth_upper_input.ip4_input = eth_input;
    eth_upper_input.arp_input = etharp_input;
    CMF_OP_CHK("ethernet modele init failed", eth_init(&eth_upper_input), return CMF_ERR_FAILED);
    CMF_OP_CHK("etharp modele init failed", etharp_init(), return CMF_ERR_FAILED);
    return CMF_ERR_OK;
}
int32 cmf_pro_module_func_register(const cmf_eth_vlan_callback_fun_t *pVlanCfg, const cmf_pbuf_cfg_t *pBufCfg)
{
    CMF_PARAM_CHK("pVlanCfg NULL", NULL == pBufCfg, CMF_ERR_NULL_POINTER);
    CMF_OP_CHK("pbuf func register failed", cmf_pbuf_func_register(pBufCfg), return CMF_ERR_FAILED);
    CMF_PARAM_CHK("pVlanCfg NULL", NULL == pVlanCfg, CMF_ERR_NULL_POINTER);
    CMF_OP_CHK("vlan func register failed", cmf_vlan_func_register(pVlanCfg), return CMF_ERR_FAILED);
    return CMF_ERR_OK;
}
