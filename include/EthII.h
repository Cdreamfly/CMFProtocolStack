#pragma once

#include <cstdint>
#include <map>
#include <string>
#include "Hdr.h"

namespace cm::eth {
	inline std::map<uint16_t, std::string_view> eth_type_str = {
		{ETH_TYPE_IP, "IPV4"},
		{ETH_TYPE_ARP, "ARP"}
	};

	class comp {
	public:
		bool operator()(const std::string_view &lhs, const std::string_view &rhs) const {
			return strcasecmp(lhs.data(), rhs.data()) < 0;
		}
	};

	inline std::map<std::string_view, uint16_t, comp> eth_str_type = {
		{"IPV4", ETH_TYPE_IP},
		{"ARP", ETH_TYPE_ARP}
	};

	class EthII {
	public:
		explicit EthII(const u_char *);

		EthII(const std::string_view &, const std::string_view &, const std::string_view &);

		EthII(const eth_addr_t &, const eth_addr_t &, const std::string_view &);

		EthII() = default;

		~EthII() = default;

		[[nodiscard]] eth_hdr_t ethPackage() const;

		[[nodiscard]] std::string_view etherTypeToString() const;

		void formatPrint() const;

		[[nodiscard]] uint16_t getEthType() const;

	private:
		eth_hdr_t eth_hdr;
	};
}
