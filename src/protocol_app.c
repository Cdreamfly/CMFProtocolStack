#include "protocol_app.h"
#include "protocol.h"
#include "common/cmf_debug.h"
#include "drv/nic.h"
#include "net/cmf_app.h"
#include "drv/nic_common.h"

static uint32 rtk_proto_initialed = INIT_NOT_COMPLETED;

static int32 rtk_rx_parser(rtk_pkt_t* pPacket, drv_nic_ptk_t* pkt)
{
    rtk_proto_pkt_t* pktInfo = &pPacket->pktInfo;
    return RT_ERR_OK;
}

static int32 rtk_proto_dispatch(const uint32 unit, drv_nic_ptk_t* pPacket, void* pCookie)
{
    int32 ret = RT_ERR_OK;
    rtk_pkt_t pkt;
    RT_ERR_CHK(rtk_rx_parser(&pkt, pPacket), ret);
    const rtk_proto_func_t* pProto_func = rtk_proto_func_get();
    for (rtk_proto_t id = 0; id < RTK_PROTO_END; id++)
    {
        if (NULL == pProto_func[id].judge || NULL == pProto_func[id].routine)
        {
            continue;
        }
        if (CMF_TRUE == pProto_func[id].judge(&pkt.pktInfo))
        {
            pProto_func[id].routine(unit, &pkt);
            return NIC_RX_HANDLED_OWNED;
        }
    }
    return NIC_RX_NOT_HANDLED;
}

int32 rtk_proto_init()
{
    int32 ret = RT_ERR_OK;
    const uint32 unit = 0;
    rtk_proto_initialed = INIT_NOT_COMPLETED;
    RT_ERR_CHK(rtk_proto_func_init(), ret);
    RT_ERR_CHK(cmf_pro_register(), ret);
    RT_ERR_CHK(rtk_proto_registered_proto_init(), ret);

    drv_nic_mapper_init(unit);
    drv_nic_init(unit);
    drv_nic_rx_register(unit, 0, rtk_proto_dispatch, NULL, 0);
    drv_nic_rx_start(unit);

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
