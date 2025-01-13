#pragma once

#include <pcap/pcap.h>

#include "Hdr.h"

namespace cm::eth::arp {
	class Arp {
	public:
		explicit Arp(const u_char *);

		Arp(const std::string_view &, const std::string_view &, const std::string_view &);

		Arp() = default;

		~Arp() = default;

		[[nodiscard]] ethArp_hdr_t arpPackage() const;

		ethArp_hdr_t arpPackage(const ip_addr_t &, const ip_addr_t &, const eth_addr_t &);

		ethArp_hdr_t arpPackage(const ip_addr_t &, const ip_addr_t &, const eth_addr_t &, const eth_addr_t &);

		ethArp_hdr_t arpPackage(const std::string_view &, const std::string_view &, const std::string_view &);

		void formatPrint() const;

	private:
		ethArp_hdr_t ethArp_hdr;
	};
}
