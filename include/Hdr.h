#pragma once
#include <cstdint>

namespace cm {
    constexpr uint8_t ETH_II_MAC_LEN = 6;
    constexpr uint8_t ETH_II_MAC_STR_LEN = 18;
    constexpr uint8_t IPV4_ADDR_LEN = 4;
    constexpr uint8_t IPV4_ADDR_STR_LEN = 16;

    typedef struct ip_addr_s {
        uint8_t addr[IPV4_ADDR_LEN];
    } ip_addr_t;

    typedef struct eth_addr_s {
        uint8_t addr[ETH_II_MAC_LEN];
    } eth_addr_t;

    namespace eth {
        enum eth_type_e :uint16_t {
            ETH_TYPE_STP_TCN = 0x0007,
            ETH_TYPE_STP = 0x0026,
            ETH_TYPE_RSTP = 0x0027,
            ETH_TYPE_IP = 0x0800,
            ETH_TYPE_ARP = 0x0806,
            ETH_TYPE_VLAN = 0x8100,
            ETH_TYPE_IPV6 = 0x86DD,
            ETH_TYPE_LACP = 0x8809,
            ETH_TYPE_REALTEK = 0x8899,
            ETH_TYPE_LLDP = 0x88CC
        };

        typedef struct eth_hdr_s {
            eth_addr_t dst_mac;
            eth_addr_t src_mac;
            uint16_t ether_type;
        }__attribute__((__packed__)) eth_hdr_t;

        namespace arp {
            typedef enum ethArp_opcode_e :uint16_t {
                ETHARP_OPCODE_ARP_REQUEST = 0x0001,
                ETHARP_OPCODE_ARP_REPLY = 0x0002,
                ETHARP_OPCODE_RARP_REQUEST = 0x0003,
                ETHARP_OPCODE_RARP_REPLY = 0x0004
            } ethArp_opcode_t;

            typedef enum ethArp_hwType_e {
                ETHARP_HWTYPE_ETHERNET = 0x0001,
            } ethArp_hwType_t;

            typedef struct ethArp_hdr_s {
                uint16_t hardware_type;
                uint16_t protocol_type;
                uint8_t hardware_size;
                uint8_t protocol_size;
                uint16_t opcode;
                eth_addr_t sender_mac;
                ip_addr_t sender_ip;
                eth_addr_t target_mac;
                ip_addr_t target_ip;
            } __attribute__((__packed__)) ethArp_hdr_t;
        }
    }

    namespace ip {
        typedef enum ip_proto_e :uint8_t {
            IP_PROTO_ICMP = 0x01,
            IP_PROTO_IGMP = 0x02,
            IP_PROTO_TCP = 0x06,
            IP_PROTO_UDP = 0x11,
            IP_PROTO_RAW = 0xFF
        } ip_proto_t;

        typedef struct ip_hdr_s {
            uint8_t version: 4;
            uint8_t hdr_len: 4;
            uint8_t tos;
            uint16_t length;
            uint16_t id_number;

            union {
                uint16_t flag_fo;

                struct {
                    uint16_t flag: 3;
                    u_int16_t fo: 13;
                };
            } frag_offset;

            uint8_t ttl;
            uint8_t protocol;
            uint16_t checksum;
            ip_addr_t src_ip;
            ip_addr_t dst_ip;
        } __attribute__((__packed__)) ip_hdr_t;

        namespace icmp {
            typedef enum icmp_type_e :uint8_t {
                ICMP_TYPE_ECHOREPLY = 0x00,
                ICMP_TYPE_ECHOREQUEST = 0X08
            } icmp_type_t;

            typedef struct icmp_hdr_s {
                uint8_t type;
                uint8_t code;
                uint16_t checksum;
                uint16_t identifier;
                uint16_t sequence;
            }__attribute__((__packed__)) icmp_hdr_t;
        }
    }
}
