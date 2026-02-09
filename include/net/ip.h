#ifndef IP_H
#define IP_H

#include "pktBuf.h"
#include "common/def.h"


typedef enum ip_flag_e
{
    IP_FLAG_RSVD = 0x8000, // 保留位（1位）
    IP_FLAG_DF = 0x4000, // 不分片标志（Don't Fragment，1位）
    IP_FLAG_MF = 0x2000, // 更多分片标志（More Fragments，1位）
} ip_flag_t;

/*
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Version|  IHL  |Type of Service|          Total Length         |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |         Identification        |Flags|      Fragment Offset    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |  Time to Live |    Protocol   |         Header Checksum       |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                       Source Address                          |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Destination Address                        |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |                    Options                    |    Padding    |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

                    Example Internet Datagram Header
 */
typedef struct ip_hdr_s
{
    // 第1字节：版本+头部长度（各4位，共8位）
    uint8 version : 4; /* 4位：IP版本，IPv4固定为4 */
    uint8 hdr_len : 4; /* 4位：IP头部长度，关键！单位为「32位字（4字节）」 */
    uint8 tos; /* 1字节：服务类型，用于QoS（如DSCP/ECN） */
    uint16 length; /* 2字节：IP报文总长度（头部+数据），最大65535字节 */

    uint16 id_number; /* 2字节：报文标识，用于分片重组（同一片段的报文id相同） */
    uint16 frag_offset; /* 2字节：分片偏移+标志位，核心见ip_flag_t解析 */

    uint8 ttl; /* 1字节：生存时间，每经过一个路由跳数减1，为0则丢弃 */
    uint8 proto; /* 1字节：上层协议类型，对应ip_proto_t枚举值 */
    uint16 chksum; /* 2字节：IP头部校验和，仅校验头部，不含数据 */

    uint8 src_ip[4]; /* 4字节：源IPv4地址，网络字节序，与ip_addr_t兼容 */
    uint8 dst_ip[4]; /* 4字节：目的IPv4地址，网络字节序，与ip_addr_t兼容 */
} ip_hdr_t;

typedef enum ip_proto_e
{
    IP_PROTO_ICMP = 0x01, // ICMP：互联网控制报文协议
    IP_PROTO_IGMP = 0x02, // IGMP：互联网组管理协议
    IP_PROTO_TCP = 0x06, // TCP：传输控制协议
    IP_PROTO_UDP = 0x11, // UDP：用户数据报协议
    IP_PROTO_RAW = 0xFF // 原始IP报文（自定义协议，无上层协议）
} ip_proto_t;

extern int32 ip_init(void);
extern int32 ip_input(pktBuf_t* pBuf);
extern int32 ip_output(pktBuf_t* pBuf, ip_addr_t* pIpAddr);
extern uint16 ip_checksum(register uint16* addr, register int len);

#endif //IP_H
