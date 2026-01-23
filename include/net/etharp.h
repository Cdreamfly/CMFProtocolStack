#ifndef ETHARP_H
#define ETHARP_H

#include "common/type.h"
#include "net/ip.h"
#include "net/eth.h"

typedef enum etharp_hwType_e
{
    ETHARP_HWTYPE_ARP,
}etharp_hwType_t;

typedef enum etharp_prototype_e
{
    ETHARP_PROTOTYPE_IP = 0x0800,
} etharp_prototype_t;

typedef enum etharp_state_e {
    ETHARP_STATE_EMPTY,    // 空状态（表项未使用）
    ETHARP_STATE_PENDING,  // 挂起状态（已发送ARP请求，等待回复）
    ETHARP_STATE_STABLE,   // 稳定状态（IP-MAC映射有效，可正常使用）
    ETHARP_STATE_EXPIRED   // 过期状态（映射失效，等待老化删除）
} etharp_state_t;

typedef enum etharp_opcode_e {
    ETHARP_OPCODE_ARP_REQUEST   = 1, // ARP请求（广播，查询IP对应的MAC）
    ETHARP_OPCODE_ARP_REPLY     = 2, // ARP回复（单播，回应IP对应的MAC）
    ETHARP_OPCODE_RARP_REQUEST  = 3, // RARP请求（反向ARP，MAC查IP，几乎废弃）
    ETHARP_OPCODE_RARP_REPLY    = 4, // RARP回复（反向ARP，几乎废弃）
} etharp_opcode_t;

typedef struct etharp_macinfo_s {
    uint8 port_ori;  // 【工程自定义】原始物理端口（开发人员添加，适配特定设备逻辑）
    uint8 port;      // 【工程核心】IP-MAC映射对应的物理端口号
    uint8 ctagif;    // 【VLAN适配】C-TAG（客户VLAN）接口标识（适配QinQ/单层VLAN）
    uint16 cvid;     // 【VLAN适配】C-TAG VLAN ID（实际可用1~4094）
} etharp_macinfo_t;

typedef	struct etharp_entry_s {
    ip_addr_t ipaddr;    // 三层：目标IPv4地址（映射的核心键值）
    ip_addr_t ethaddr;   // 二层：目标MAC地址（映射的结果值）
    etharp_state_t state;     // 表项生命周期状态（对应etharp_state_t）
    uint8 ctime;                  // 表项创建/更新时间（老化计时，单位一般为秒）
    etharp_macinfo_t macinfo;      // 扩展：VLAN/端口关联信息（工程适配）
    uint8 flag;                   // 预留：扩展标志位（如静态/动态表项标识）
} etharp_entry_t;

typedef struct etharp_hdr_s {
    uint16 hw_type;        // 硬件类型（对应etharp_hwtype_t，固定1=以太网）
    uint16 proto_type;     // 上层协议类型（对应etharp_prototype_t，固定0x0800=IPv4）

    uint8  hlen;           // 硬件地址长度（固定6，以太网MAC地址长度）
    uint8  plen;           // 协议地址长度（固定4，IPv4地址长度）
    uint16 opcode;         // 操作码（对应etharp_opcode_t，1=请求/2=回复）

    eth_addr_t sender_hwaddr; // 发送方MAC地址（以太网硬件地址）
    eth_addr_t sender_ipaddr; // 发送方IP地址（IPv4协议地址）
    eth_addr_t target_hwaddr; // 目标方MAC地址（以太网硬件地址）
    ip_addr_t  target_ipaddr; // 目标方IP地址（IPv4协议地址）
} etharp_hdr_t;

extern void etharp_timer();
extern int32 etharp_init();
extern int32 etharp_entry_update(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr);
extern int32 etharp_entry_find(ip_addr_t* pIpAddr, eth_addr_t* pEthAddr);
extern int32 etharp_entry_update_with_macInfo(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr, etharp_macinfo_t macInfo);
extern int32 etharp_entry_find_with_macInfo(ip_addr_t *pIpAddr, eth_addr_t *pEthAddr, etharp_macinfo_t *pMacInfo);
extern int32 etharp_input(pktBuf_t *pBuf);
extern int32 etharp_check(ip_addr_t ip);
extern int32 etharp_request(ip_addr_t ip);
extern int32 etharp_request_vlan(ip_addr_t ip, uint16 vid, int32 tx_port_mask);
#endif //ETHARP_H
