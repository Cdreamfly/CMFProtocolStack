#pragma once
#include <map>
#include <string_view>
#include <sys/types.h>

#include "Hdr.h"

namespace cm::ip {
	inline std::map<uint16_t, std::string_view> ip_protocol_str = {
		{IP_PROTO_ICMP, "ICMP"},
		{IP_PROTO_IGMP, "IGMP"},
		{IP_PROTO_TCP, "TCP"},
		{IP_PROTO_UDP, "UDP"},
		{IP_PROTO_RAW, "RAW"},

	};

	class Ip {
	public:
		explicit Ip(const u_char *);

		~Ip() = default;

		[[nodiscard]] ip_hdr_t ipPackage() const;

		[[nodiscard]] std::string_view ipProtocolToString() const;

		[[nodiscard]] uint8_t getProtocol() const;

		void formatPrint() const;

	private:
		ip_hdr_t ip_hdr;
	};
}
