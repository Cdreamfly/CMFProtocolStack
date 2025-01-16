#include "Ip.h"

#include <fmt/format.h>

#include "Utils.hpp"

cm::ip::Ip::Ip(const u_char *ptk) : ip_hdr{
	.version = static_cast<uint8_t>(ptk[0] >> 4),
	.hdr_len = static_cast<uint8_t>(ptk[0] & 0x0f),
	.tos = ptk[1],
	.length = static_cast<uint16_t>(ptk[2] << 8 | ptk[3]),
	.id_number = static_cast<uint16_t>(ptk[4] << 8 | ptk[5]),
	.frag_offset = static_cast<uint16_t>(ptk[6] << 8 | ptk[7]),
	.ttl = ptk[8],
	.protocol = ptk[9],
	.checksum = static_cast<uint16_t>(ptk[10] << 8 | ptk[11]),
	.src_ip = {ptk[12], ptk[13], ptk[14], ptk[15]},
	.dst_ip = {ptk[16], ptk[17], ptk[18], ptk[19]}
} {
	if (checksum(reinterpret_cast<const uint16_t *>(ptk), sizeof(ip_hdr_t))) {
		fmt::print("ip checksum error\n");
	}
}

cm::ip::ip_hdr_t cm::ip::Ip::ipPackage() const {
	return ip_hdr;
}

std::string_view cm::ip::Ip::ipProtocolToString() const {
	return ip_protocol_str[ip_hdr.protocol];
}

uint8_t cm::ip::Ip::getProtocol() const {
	return ip_hdr.protocol;
}

void cm::ip::Ip::formatPrint() const {
	fmt::print("{} {} {} -> {}\n", ip_protocol_str[ip_hdr.protocol], ip_hdr.length, ip_to_str(ip_hdr.src_ip), ip_to_str(ip_hdr.dst_ip));
}
