#ifndef ETH_H
#define ETH_H

#include "common/def.h"
#include "net/pktBuf.h"
#include "net/netif.h"


typedef enum eth_type_e
{
    ETH_TYPE_STP_TCN = 0x0007,  // STP拓扑变更通知
    ETH_TYPE_STP     = 0x0026,  // 生成树协议（二层链路防环）
    ETH_TYPE_RSTP    = 0X0027,  // 快速生成树协议（STP升级版）
    ETH_TYPE_IP      = 0X0800,  // 互联网协议v4（IPv4，三层核心）
    ETH_TYPE_ARP     = 0X0806,  // 地址解析协议（二层→三层地址映射）
    ETH_TYPE_VLAN    = 0x8100   // 802.1Q VLAN标签（标识帧带VLAN标签）
} eth_type_t;

typedef struct eth_hdr_s
{
    eth_addr_t dst_mac;
    eth_addr_t src_mac;
    eth_type_t ether_type;
}eth_hdr_t;

typedef struct eth_vlan_hdr_s
{
    uint16 priority:3;  // 3位：802.1p链路层优先级（QoS）
    uint16 cfi:1;       // 1位：规范格式指示符（以太网固定为0）
    uint16 vid:12;      // 12位：VLAN ID（1~4094为可用范围）
    uint16 ether_type;
}eth_vlan_hdr_t;

typedef int32 (*cmf_eth_vid_pm_get_f)(uint32 unit, uint32 port, uint32 *pPvid, uint32 *pPm);
typedef int32 (*cmf_ip4_input_f)(pktBuf_t *pktBuf, netIf_t *pNetIf, ip_addr_t *ipAddr);
typedef int32 (*cmf_etharp_input_f)(pktBuf_t *pktBuf, netIf_t *pNetIf);

typedef struct cmf_eth_vlan_callback_fun_s
{
    cmf_eth_vid_pm_get_f eth_vid_pm_get;
} cmf_eth_vlan_callback_fun_t;

typedef struct cmf_upper_input_func_s
{
    cmf_ip4_input_f ip4_input;
    cmf_etharp_input_f arp_input;
} cmf_upper_input_func_t;

extern int32 eth_init(const cmf_upper_input_func_t *pUpper_input);
extern int32 eth_input(pktBuf_t *pBuf, netIf_t *pNetIf);
extern int32 eth_output(pktBuf_t *pktBuf, netIf_t *pNetIf);
extern int32 cmf_nic_rxVid_get(uint32 *pVid);
extern int32 cmf_vlan_func_register(const cmf_eth_vlan_callback_fun_t *pVlanCfg);

#endif //ETH_H
