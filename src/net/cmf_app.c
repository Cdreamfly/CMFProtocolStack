#include "net/cmf_app.h"
#include "net/cmf.h"
#include "protocol.h"


static int32 cmf_init()
{
    return CMF_ERR_OK;
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

static int32 cmf_pkt_routine(uint32 unit, rtk_proto_pkt_t* pPkt)
{

    return CMF_ERR_OK;
}

static int32 cmf_tick_handle()
{

    return CMF_ERR_OK;
}

int32 cmf_pro_register()
{
    rtk_proto_func_register(RTK_PROTO_CMF, cmf_init,NULL, is_pkt_cmf, cmf_pkt_routine, cmf_tick_handle);
    return CMF_ERR_OK;
}
