#include <fmt/core.h>

#include "Arp.h"
#include "Utils.hpp"

cm::eth::arp::Arp::Arp(const u_char *ptk) : ethArp_hdr{
	.hardware_type = static_cast<uint16_t>(ptk[0] << 8 | ptk[1]),
	.protocol_type = static_cast<uint16_t>(ptk[2] << 8 | ptk[3]),
	.hardware_size = ptk[4],
	.protocol_size = ptk[5],
	.opcode = static_cast<uint16_t>(ptk[6] << 8 | ptk[7]),
	.sender_mac = {ptk[8], ptk[9], ptk[10], ptk[11], ptk[12], ptk[13]},
	.sender_ip = {ptk[14], ptk[15], ptk[16], ptk[17]},
	.target_mac = {ptk[18], ptk[19], ptk[20], ptk[21], ptk[22], ptk[23]},
	.target_ip = {ptk[24], ptk[25], ptk[26], ptk[27]}
} {
}

cm::eth::arp::Arp::Arp(const std::string_view &sender_ip, const std::string_view &target_ip, const std::string_view &sender_mac): ethArp_hdr{
	.hardware_type = htons(ETHARP_HWTYPE_ETHERNET),
	.protocol_type = htons(ETH_TYPE_IP),
	.hardware_size = ETH_II_MAC_LEN,
	.protocol_size = IPV4_ADDR_LEN,
	.opcode = htons(ETHARP_OPCODE_ARP_REQUEST),
	.sender_mac = str_to_mac(sender_mac),
	.sender_ip = str_to_ip(sender_ip.data()),
	.target_mac = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
	.target_ip = str_to_ip(target_ip.data()),
} {
}

cm::eth::arp::ethArp_hdr_t cm::eth::arp::Arp::arpPackage() const {
	return ethArp_hdr;
}

cm::eth::arp::ethArp_hdr_t cm::eth::arp::Arp::arpPackage(const ip_addr_t &sender_ip, const ip_addr_t &target_ip, const eth_addr_t &sender_mac) {
	return ethArp_hdr = {
		       .hardware_type = htons(ETHARP_HWTYPE_ETHERNET),
		       .protocol_type = htons(ETH_TYPE_IP),
		       .hardware_size = ETH_II_MAC_LEN,
		       .protocol_size = IPV4_ADDR_LEN,
		       .opcode = htons(ETHARP_OPCODE_ARP_REQUEST),
		       .sender_mac = sender_mac,
		       .sender_ip = sender_ip,
		       .target_mac = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
		       .target_ip = target_ip,
	       };
}

cm::eth::arp::ethArp_hdr_t cm::eth::arp::Arp::arpPackage(const ip_addr_t &sender_ip, const ip_addr_t &target_ip, const eth_addr_t &sender_mac, const eth_addr_t &target_mac) {
	return ethArp_hdr = {
		       .hardware_type = htons(ETHARP_HWTYPE_ETHERNET),
		       .protocol_type = htons(ETH_TYPE_IP),
		       .hardware_size = ETH_II_MAC_LEN,
		       .protocol_size = IPV4_ADDR_LEN,
		       .opcode = htons(ETHARP_OPCODE_ARP_REPLY),
		       .sender_mac = sender_mac,
		       .sender_ip = sender_ip,
		       .target_mac = target_mac,
		       .target_ip = target_ip,
	       };
}

cm::eth::arp::ethArp_hdr_t cm::eth::arp::Arp::arpPackage(const std::string_view &sender_ip, const std::string_view &target_ip, const std::string_view &sender_mac) {
	return ethArp_hdr = {
		       .hardware_type = htons(ETHARP_HWTYPE_ETHERNET),
		       .protocol_type = htons(ETH_TYPE_IP),
		       .hardware_size = ETH_II_MAC_LEN,
		       .protocol_size = IPV4_ADDR_LEN,
		       .opcode = htons(ETHARP_OPCODE_ARP_REQUEST),
		       .sender_mac = str_to_mac(sender_mac),
		       .sender_ip = str_to_ip(sender_ip.data()),
		       .target_mac = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
		       .target_ip = str_to_ip(target_ip.data()),
	       };
}

void cm::eth::arp::Arp::formatPrint() const {
	switch (ethArp_hdr.opcode) {
		case ETHARP_OPCODE_ARP_REQUEST:
			fmt::print("Who has {}? Tell {}\n", ip_to_str(ethArp_hdr.target_ip), ip_to_str(ethArp_hdr.sender_ip));
			break;
		case ETHARP_OPCODE_ARP_REPLY:
			fmt::print("{} is at {}\n", ip_to_str(ethArp_hdr.sender_ip), mac_to_str(ethArp_hdr.sender_mac));
			break;
		default:
			fmt::print("Unknown arp opcode {:x}\n", ethArp_hdr.opcode);
			break;
	}
}
