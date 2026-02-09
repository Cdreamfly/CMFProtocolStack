#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common/type.h"
#include "common/def.h"

typedef enum rtk_proto_e
{
    RTK_PROTO_RLPP = 0,
    RTK_PROTO_RLDP,
    RTK_PROTO_RRCP,
    RTK_PROTO_ARP,
    RTK_PROTO_OTHER,
    RTK_PROTO_CMF,
    RTK_PROTO_IGMP,
    RTK_PROTO_END
} rtk_proto_t;

typedef struct rtk_proto_pkt_e
{
    uint8* pPkt;
    uint8 src_port;
    uint8 dmac[ETH_HWADDR_LED];
    uint8 smac[ETH_HWADDR_LED];

    uint32 is_otag;
    uint32 is_itag;

    uint16 etherType;
    uint8 rktpp_subType;

    uint32 pkt_len;
} rtk_proto_pkt_t;

typedef struct rtk_pkt_s
{
    uint8* pPkt;
    uint32 pKt_len;

    uint8 *l2_hdr;
    uint8 *l3_hdr;
    uint8 *l4_hdr;

    rtk_proto_pkt_t pktInfo;
} rtk_pkt_t;

typedef int32 (*init_func)(void);
typedef int32 (*enable_func)(rtk_enable_t status);
typedef int32 (*judge_func)(rtk_proto_pkt_t* pPkt);
typedef int32 (*routine_func)(uint32 unit, rtk_pkt_t* pPkt);
typedef int32 (*tick_handle_func)(void);

typedef struct rtk_proto_func_s
{
    init_func init;
    enable_func status_set;
    judge_func judge;
    routine_func routine;
    tick_handle_func tick_handle;
} rtk_proto_func_t;

extern rtk_proto_func_t* rtk_proto_func_get();
extern int32 rtk_proto_func_init();
extern int32 rtk_proto_func_register(rtk_proto_t id, init_func init, enable_func status_set, judge_func judge,
                                     routine_func routine, tick_handle_func tick_handle);
extern int32 rtk_proto_func_unregister(rtk_proto_t id);
extern int32 rtk_proto_registered_proto_init();
extern int32 rtk_proto_registered_proto_enable(rtk_enable_t status);
extern int32 rtk_proto_registered_proto_tick_handle();

#endif //PROTOCOL_H
