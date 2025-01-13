#pragma once

#include <string>
#include <arpa/inet.h>

#include "Hdr.h"

namespace cm {
	inline std::string mac_to_str(const eth_addr_t &eth_addr) {
		char mac[ETH_II_MAC_STR_LEN] = {0};
		std::snprintf(mac, sizeof(mac), "%02x:%02x:%02x:%02x:%02x:%02x", eth_addr.addr[0], eth_addr.addr[1], eth_addr.addr[2], eth_addr.addr[3], eth_addr.addr[4],
		              eth_addr.addr[5]);
		return mac;
	}

	inline eth_addr_t str_to_mac(const std::string_view &mac) {
		eth_addr_t eth_addr;
		if (std::sscanf(mac.data(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &eth_addr.addr[0], &eth_addr.addr[1], &eth_addr.addr[2], &eth_addr.addr[3], &eth_addr.addr[4], &eth_addr.addr[5]) != ETH_II_MAC_LEN) {
			throw std::runtime_error(mac.data() + std::string(" is an invalid MAC address"));
		}
		return eth_addr;
	}

	inline std::string ip_to_str(const ip_addr_t &ip_addr) {
		char ip[IPV4_ADDR_STR_LEN] = {0};
		std::snprintf(ip, sizeof(ip), "%d.%d.%d.%d", ip_addr.addr[0], ip_addr.addr[1], ip_addr.addr[2], ip_addr.addr[3]);
		return ip;
	}

	inline ip_addr_t str_to_ip(const std::string_view &ip) {
		const uint32_t ip_addr = inet_addr(ip.data());
		ip_addr_t addr = {0};
		std::memcpy(&addr, &ip_addr, sizeof(uint32_t));
		return addr;
	}
}
