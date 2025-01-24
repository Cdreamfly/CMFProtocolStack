#include <iostream>
#include <fmt/format.h>

#include "Arp.h"
#include "Nps.h"
#include "EthII.h"
#include "Icmp.h"
#include "Ip.h"
#include "Utils.hpp"

int main() {
    cm::nps::NpsDevice device;
    device.openDevice("eth0");
    //device.PrintAllDevices();
#if 0
	const cm::eth::EthII eth{"b8:d4:bc:36:43:4d", "ea:1a:0a:25:c5:95", "ARP"};
	eth.formatPrint();
	const cm::eth::eth_hdr_t eth_hdr = eth.ethPackage();

	cm::eth::arp::Arp arp{"192.168.0.3", "192.168.0.1", "ea:1a:0a:25:c5:95"};
	const cm::eth::arp::ethArp_hdr_t arp_hdr = arp.arpPackage();

	uint8_t data[sizeof(cm::eth::eth_hdr_t) + sizeof(cm::eth::arp::ethArp_hdr_t)] = {0};
	memcpy(data, &eth_hdr, sizeof(eth_hdr));
	memcpy(data + sizeof(eth_hdr), &arp_hdr, sizeof(arp_hdr));

	for (unsigned char & i : data) {
		fmt::print("{:02x} ", i);
	}
	fmt::print("\n");
	for(int i = 0; i < 10; i++) {
		if (device.sendPacket(data, sizeof(data)) == cm::nps::NpsError::SUCCESS) {
			fmt::print("Sent ok\n");
		}
	}
#endif
#if 1
    device.captureCompilePacketLoop([](u_char *state, const pcap_pkthdr *header, const u_char *pkt) {
        for (bpf_u_int32 i = 0; i < header->len; ++i) {
            //fmt::print("{:02x} ", pkt[i]);
        }
        fmt::print("\n");
        const cm::eth::EthII eth(pkt);
        eth.formatPrint();
        switch (eth.getEthType()) {
            case cm::eth::ETH_TYPE_ARP: {
                const cm::eth::arp::Arp arp(pkt + sizeof(cm::eth::eth_hdr_t));
                arp.formatPrint();
            }
            break;
            case cm::eth::ETH_TYPE_IP: {
                for (bpf_u_int32 i = sizeof(cm::eth::eth_hdr_t); i < header->len; ++i) {
                   fmt::print("{:02x} ", pkt[i]);
                }
                fmt::print("\n");
                const cm::ip::Ip ip(pkt + sizeof(cm::eth::eth_hdr_t));
                ip.formatPrint();
                switch (ip.getProtocol()) {
                    case cm::ip::IP_PROTO_ICMP: {
                        for (bpf_u_int32 i = sizeof(cm::eth::eth_hdr_t) + sizeof(cm::ip::ip_hdr_t); i < header->len; ++i) {
                            fmt::print("{:02x} ", pkt[i]);
                        }
                        fmt::print("\n");
                        const cm::ip::icmp::Icmp icmp(pkt + sizeof(cm::eth::eth_hdr_t) + sizeof(cm::ip::ip_hdr_t));
                        icmp.formatPrint();
                    }
                    break;
                    default:
                        break;
                }
            }
            break;
            default:
                break;
        }
    }, "icmp", 5);
#endif
    return 0;
}
