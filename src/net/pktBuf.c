#include "net/pktBuf.h"
#include "net/cmf.h"
#include "common/def.h"
#include "common/cmf_debug.h"

static cmf_pbuf_cfg_t cmf_pbuf_fns;
static int32 pbuf_initialized = CMF_FALSE;

int32 cmf_pbuf_func_register(cmf_pbuf_cfg_t *pPbufCfg)
{
    CMF_PARAM_CHK("pPbufCfg NULL", NULL == pPbufCfg, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("pbuf_alloc_f NULL", NULL == pPbufCfg->pbuf_alloc_f, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("pbuf_free_f NULL", NULL == pPbufCfg->pbuf_free_f, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("pbuf_rx_f NULL", NULL == pPbufCfg->pbuf_rx_f, return CMF_ERR_NULLPOINT);
    CMF_PARAM_CHK("pbuf_tx_f NULL", NULL == pPbufCfg->pbuf_tx_f, return CMF_ERR_NULLPOINT);
    cmf_pbuf_fns.pbuf_alloc_f = pPbufCfg->pbuf_alloc_f;
    cmf_pbuf_fns.pbuf_free_f = pPbufCfg->pbuf_free_f;
    cmf_pbuf_fns.pbuf_rx_f = pPbufCfg->pbuf_rx_f;
    cmf_pbuf_fns.pbuf_tx_f = pPbufCfg->pbuf_tx_f;
    pbuf_initialized = CMF_TRUE;
    return CMF_OK;
}

int32 cmf_pbuf_alloc(const int32 size, const uint32 flags, pktBuf_t **ppBuf)
{
    CMF_PARAM_CHK("pbuf_initialized NOT initialized", CMF_FALSE == pbuf_initialized, return CMF_ERR_NOTINITIALIZED);
    CMF_PARAM_CHK("ppBuf IS NULL", NULL == ppBuf, return CMF_ERR_NULLPOINT);
    CMF_OP_CHK("pBuf ALLOC FAILED", cmf_pbuf_fns.pbuf_alloc_f(size, flags, ppBuf), return CMF_ERR);
    (*ppBuf)->data = (*ppBuf)->head;
    (*ppBuf)->tail = (*ppBuf)->data + size;
    (*ppBuf)->end = (*ppBuf)->tail;
    (*ppBuf)->length = size;

    (*ppBuf)->priority = 0;
    (*ppBuf)->tx_port_mask = 0;
    (*ppBuf)->tx_vlan_as = 0;
    (*ppBuf)->tx_vlan_vid = 0;
    (*ppBuf)->tx_vlan_port_mask = 0;
    return CMF_OK;
}
int32 cmf_pbuf_free(pktBuf_t *pBuf)
{
    CMF_PARAM_CHK("pbuf_initialized NOT initialized", CMF_FALSE == pbuf_initialized, return CMF_ERR_NOTINITIALIZED);
    CMF_PARAM_CHK("pBuf NOT initialized", CMF_FALSE == pBuf, return CMF_ERR_NULLPOINT);
    CMF_OP_CHK("pBuf free failed", cmf_pbuf_fns.pbuf_free_f(pBuf), return CMF_ERR);
    return CMF_OK;
}
int32 cmf_pbuf_tx(pktBuf_t *pBuf)
{
    CMF_PARAM_CHK("pbuf_initialized NOT initialized", CMF_FALSE == pbuf_initialized, return CMF_ERR_NOTINITIALIZED);
    CMF_PARAM_CHK("pBuf NOT initialized", CMF_FALSE == pBuf, return CMF_ERR_NULLPOINT);
    CMF_OP_CHK("pBuf tx failed", cmf_pbuf_fns.pbuf_tx_f(pBuf), return CMF_ERR);
    return CMF_OK;
}
int32 cmf_pbuf_rx(pktBuf_t *pBuf)
{
    netIf_t netIf;
    CMF_PARAM_CHK("pbuf_initialized NOT initialized", CMF_FALSE == pbuf_initialized, return CMF_ERR_NOTINITIALIZED);
    CMF_PARAM_CHK("pBuf NOT initialized", CMF_FALSE == pBuf, return CMF_ERR_NULLPOINT);
    pBuf->l2fmt = L2FMT_ETH;
    pBuf->l3fmt = L3FMT_NONE;
    pBuf->l4fmt = L4FMT_NONE;
    cmf_netif_info_get(&netIf);
    pBuf->netif_rx = &netIf;
    CMF_OP_CHK("pBuf rx failed", cmf_pbuf_fns.pbuf_rx_f(pBuf, &netIf), return CMF_ERR);
    CMF_OP_CHK("pBuf free failed", cmf_pbuf_fns.pbuf_free_f(pBuf), return CMF_ERR);
    return CMF_OK;
}