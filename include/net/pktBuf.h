#ifndef PKTBUF_H
#define PKTBUF_H
#include "netif.h"
#include "common/type.h"

typedef enum layer2_fmt_e
{
    L2FMT_NONE, // 未识别/无二层协议
    L2FMT_ETH, // 以太网帧（802.3，含普通帧/VLAN帧）
    L2FMT_ETHARP, // 以太网封装的ARP报文（二层核心协议）
    L2FMT_LEN // 枚举长度（工程用，标识二层协议类型数量）
} layer2_fmt_t;

typedef enum layer3_fmt_e
{
    L3FMT_NONE, // 未识别/无三层协议
    L3FMT_IP, // 互联网协议v4（IPv4，核心三层协议）
    L3FMT_IPV6, // 互联网协议v6（IPv6，预留扩展）
    L3FMT_RSTP /* LLC+RSTP is in L3 in this system */
} layer3_fmt_t;

typedef enum layer4_fmt_e
{
    L4FMT_NONE, // 未识别/无四层协议
    L4FMT_ICMP, // 互联网控制报文协议（IPv4配套，如ping/路由通告）
    L4FMT_IGMP, // 互联网组管理协议（组播管理，如组播组加入/离开）
    L4FMT_TCP, // 传输控制协议（面向连接、可靠传输）
    L4FMT_UDP // 用户数据报协议（无连接、高速传输）
} layer4_fmt_t;


typedef struct pktBuf_s
{
    uint8* head; // 缓冲区**物理起始地址**（整个缓冲区的首地址，不可变）
    uint8* data; // 有效数据**起始地址**（可移动，用于头预留，如封装协议头）
    uint8* tail; // 有效数据**末尾地址**（可移动，用于尾追加，如添加数据）
    uint8* end; // 缓冲区**物理结束地址**（整个缓冲区的尾地址，不可变）
    int32 length; // 有效数据**总长度**（= tail - data，缓存值避免重复计算）

    netIf_t* netif_rx; /* Rx NetIf：接收该数据包的物理网口结构体指针 */
    netIf_t* netif_tx; /* Tx NetIf：待发送该数据包的物理网口结构体指针 */

    uint8* l2_hdr; /* 二层协议头起始地址 → 强转eth_hdr_t/eth_vlan_hdr_t */
    uint8* l3_hdr; /* 三层协议头起始地址 → 强转ip_hdr_t/etharp_hdr_t */
    uint8* l4_hdr; /* 四层协议头起始地址 → 强转tcp_hdr_t/udp_hdr_t/icmp_hdr_t */
    uint8* payload; /* 应用层原始数据起始地址 → 裸数据/应用层协议（如HTTP/DNS） */

    uint32 unit;
    uint8 priority; // 收包的链路层优先级（802.1p，从VLAN标签/以太网帧头提取）
    uint8 rx_port_ori; // 【工程扩展】原始接收物理端口（开发人员添加，适配设备特定逻辑）
    uint8 rx_port; // 【核心】实际接收物理端口号（如0/1/2，对应设备网口）
    // 位域：3位保留（4字节对齐） + 1位C-TAG接口 + 12位C-TAG VLAN ID（802.1Q标准）
    uint16 rx_rsvd3 : 3; /* for 4bytes-align：3位保留，保证后续字段4字节对齐 */
    uint16 rx_ctagif : 1; // C-TAG VLAN接口标识（适配QinQ/单层VLAN，与etharp_macinfo_t->ctagif一致）
    uint16 rx_cvid : 12; // C-TAG VLAN ID（1~4094，802.1Q标准，与eth_vlan_hdr_t->vid一致）

    uint32 tx_port_mask; // 发包端口掩码（如0x00000003表示从端口0、1同时发出，支持多播/广播）
    // 位域：19位保留 + 1位VLAN标签使能 + 12位发包VLAN ID（802.1Q标准）
    uint32 tx_vlan_reserved : 19;
    uint32 tx_vlan_as : 1; // 【核心】是否添加/覆盖VLAN标签（1=使能，0=不处理）
    uint32 tx_vlan_vid : 12; // 发包的C-TAG VLAN ID（1~4094，802.1Q标准）
    uint32 tx_vlan_port_mask; // VLAN专属发包端口掩码（仅在tx_vlan_as=1时生效，实现VLAN内端口转发）

    uint8 l2_learning; // 二层MAC地址学习使能（1=使能，0=禁止，如禁止组播包的MAC学习）
    uint8 l2fmt; // 二层协议格式 → 赋值layer2_fmt_t枚举值（L2FMT_ETH/L2FMT_ETHARP）
    uint8 l3fmt; // 三层协议格式 → 赋值layer3_fmt_t枚举值（L3FMT_IP/L3FMT_RSTP）
    uint8 l4fmt; // 四层协议格式 → 赋值layer4_fmt_t枚举值（L4FMT_TCP/L4FMT_UDP）
    uint8 ctrl_buff[48]; /* 控制缓冲区：协议处理临时存储（如ARP学习、MAC表更新、路由查表的临时数据） */
    uint8 vlan_ctagif : 1; /* Add VLAN tag：独立的VLAN标签添加使能位（备用，与tx_vlan_as联动） */
} pktBuf_t;

typedef int32 (*cmf_pbuf_alloc_f)(int32 size, uint32 flags, pktBuf_t **ppBuf);
typedef int32 (*cmf_pbuf_free_f)(pktBuf_t *pBuf);
typedef int32 (*cmf_pbuf_tx_f)(pktBuf_t *pBuf);
typedef int32 (*cmf_pbuf_rx_f)(pktBuf_t *pBuf, netIf_t *pNetIf);

typedef struct cmf_pbuf_cfg_s
{
    cmf_pbuf_alloc_f pbuf_alloc_f;
    cmf_pbuf_free_f pbuf_free_f;
    cmf_pbuf_tx_f pbuf_tx_f;
    cmf_pbuf_rx_f pbuf_rx_f;
} cmf_pbuf_cfg_t;

extern int32 cmf_pbuf_func_register(cmf_pbuf_cfg_t *pPbufCfg);
extern int32 cmf_pbuf_alloc(int32 size, uint32 flags, pktBuf_t **ppBuf);
extern int32 cmf_pbuf_free(pktBuf_t *pBuf);
extern int32 cmf_pbuf_tx(pktBuf_t *pBuf);
extern int32 cmf_pbuf_rx(pktBuf_t *pBuf);

#endif //PKTBUF_H

