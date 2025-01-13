#include <string>
#include <fmt/core.h>
#include <netinet/in.h>

#include "Utils.hpp"
#include "EthII.h"

cm::eth::EthII::EthII(const u_char *ptk) : eth_hdr{
	.dst_mac = {ptk[0], ptk[1], ptk[2], ptk[3], ptk[4], ptk[5]},
	.src_mac = {ptk[6], ptk[7], ptk[8], ptk[9], ptk[10], ptk[11]},
	.ether_type = static_cast<uint16_t>(ptk[12] << 8 | ptk[13])
} {
}

cm::eth::EthII::EthII(const std::string_view &dst_mac, const std::string_view &src_mac, const std::string_view &type): eth_hdr{
	.dst_mac = str_to_mac(dst_mac),
	.src_mac = str_to_mac(src_mac),
	.ether_type = htons(eth_str_type[type])
} {
}

cm::eth::EthII::EthII(const eth_addr_t &src_mac, const eth_addr_t &dst_mac, const std::string_view &type) : eth_hdr{
	.dst_mac = dst_mac,
	.src_mac = src_mac,
	.ether_type = htons(eth_str_type[type])
} {
}

cm::eth::eth_hdr_t cm::eth::EthII::ethPackage() const {
	return eth_hdr;
}

std::string_view cm::eth::EthII::etherTypeToString() const {
	return eth_type_str[eth_hdr.ether_type];
}

uint16_t cm::eth::EthII::getEthType() const {
	return eth_hdr.ether_type;
}

void cm::eth::EthII::formatPrint() const {
	fmt::print("{:X} {} -> {}\n", eth_hdr.ether_type, mac_to_str(eth_hdr.src_mac), mac_to_str(eth_hdr.dst_mac));
}
