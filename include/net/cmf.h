#ifndef CMF_H
#define CMF_H

#include "net/etharp.h"

typedef enum cmf_ret_val_e
{
    CMF_ERR_FAILED = -1,
    CMF_ERR_OK = 0,
    CMF_ERR_INPUT,
    CMF_ERR_ENTRY_INDEX,
    CMF_ERR_NULL_POINTER,
    CMF_ERR_OUT_OF_RANGE,
    CMF_ERR_NOT_INIT,
    CMF_ERR_NOT_SUPPORT,
    CMF_ERR_END
}cmf_ret_val_t;

extern int32 cmf_pro_module_init();
extern int32 cmf_pro_module_func_register(const cmf_eth_vlan_callback_fun_t *pVlanCfg, const cmf_pbuf_cfg_t *pBufCfg);

#endif //CMF_H
