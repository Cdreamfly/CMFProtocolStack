#include "net/cmf_app.h"
#include "net/cmf.h"
#include "protocol.h"

int32 _cmf_pbuf_alloc_f(int32 size, uint32 flags, pktBuf_t **ppBuf)
{
    return CMF_ERR_OK;
}

int32 _cmf_pbuf_free_f(pktBuf_t *pBuf)
{
    return CMF_ERR_OK;
}

int32 _cmf_pbuf_tx_f(pktBuf_t *pBuf)
{
    return CMF_ERR_OK;
}

int32 _cmf_pbuf_rx_f(pktBuf_t *pBuf, netIf_t *pNetIf)
{
    return CMF_ERR_OK;
}

static int32 cmf_init()
{
    cmf_eth_vlan_callback_fun_t ethVlanCfg;
    cmf_pbuf_cfg_t pBufCfg;

    ethVlanCfg.eth_vid_pm_get = NULL;

    pBufCfg.pbuf_alloc_f = _cmf_pbuf_alloc_f;
    pBufCfg.pbuf_free_f = _cmf_pbuf_free_f;
    pBufCfg.pbuf_tx_f = _cmf_pbuf_tx_f;
    pBufCfg.pbuf_rx_f = _cmf_pbuf_rx_f;

    cmf_pro_module_func_register(&ethVlanCfg, &pBufCfg);
    return cmf_pro_module_init();
}

static int32 is_pkt_cmf(const rtk_proto_pkt_t* pPkt)
{
    if (pPkt == NULL)
    {
        return CMF_FALSE;
    }
    if (pPkt->pPkt == NULL)
    {
        return CMF_FALSE;
    }
    if (pPkt->etherType == 0x8899)
    {
        return CMF_FALSE;
    }
    return CMF_TRUE;
}

static int32 cmf_pkt_routine(uint32 unit, rtk_pkt_t* pPkt)
{
    pktBuf_t rxBuf;

    return cmf_pbuf_rx(&rxBuf);
}

static int32 cmf_tick_handle()
{

    return CMF_ERR_OK;
}

int32 cmf_pro_register()
{
    return rtk_proto_func_register(RTK_PROTO_CMF, cmf_init,NULL, is_pkt_cmf, cmf_pkt_routine, cmf_tick_handle);
}
