#include "protocol_app.h"
#include "protocol.h"
#include "common/cmf_debug.h"

static uint32 rtk_proto_initialed = INIT_NOT_COMPLETED;

void rldp_tick_handle()
{
    debug_printf("RLDP tick handle\n");
}

static int32 rtk_proto_dispatch(const uint32 unit, rtk_pkt_t*pPacket, void *pCookie)
{
    const rtk_proto_func_t *pProto_func = rtk_proto_func_get();
    for (rtk_proto_t id = 0; id < RTK_PROTO_END; id++)
    {
        if (NULL == pProto_func[id].judge || NULL == pProto_func[id].routine)
        {
            continue;
        }
        if (CMF_TRUE == pProto_func[id].judge(&pPacket->pktInfo))
        {
            pProto_func[id].routine(unit, &pPacket->pktInfo);
            return NIC_RX_HANDLED_OWNED;
        }
    }
    return NIC_RX_NOT_HANDLED;
}

int32 rtk_proto_init()
{
    int32 ret = RT_ERR_OK;
    rtk_proto_initialed = INIT_NOT_COMPLETED;
    RT_ERR_CHK(rtk_proto_func_init(), ret);
    rtk_proto_func_register(RTK_PROTO_RLDP, NULL,NULL,NULL,NULL, rldp_tick_handle);
    RT_ERR_CHK(rtk_proto_registered_proto_init(), ret);
    //todo drv_nic_rx_register
    RT_ERR_CHK(rtk_proto_registered_proto_enable(ENABLED), ret);
    rtk_proto_initialed = INIT_COMPLETED;
    return RT_ERR_OK;
}

int32 rtk_proto_tick_handle_app()
{
    if (INIT_COMPLETED != rtk_proto_initialed)
    {
        return RT_ERR_FAILED;
    }
    return rtk_proto_registered_proto_tick_handle();
}
