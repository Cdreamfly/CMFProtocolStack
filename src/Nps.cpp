#include <fmt/core.h>
#include <arpa/inet.h>
#include "Nps.h"

cm::nps::NpsDevice::NpsDevice() : m_fp(nullptr), m_allDevs(nullptr) {
	if (char err[PCAP_ERRBUF_SIZE] = {0}; pcap_findalldevs(&m_allDevs, err) == PCAP_ERROR) {
		fmt::print("find all devs err {}\n", err);
		exit(0);
	}
	for (pcap_if_t *dev = m_allDevs; dev != nullptr; dev = dev->next) {
		m_devices.push_back(dev);
	}
}

cm::nps::NpsDevice::~NpsDevice() {
	if (m_fp != nullptr) {
		pcap_close(m_fp);
	}
	if (m_allDevs != nullptr) {
		pcap_freealldevs(m_allDevs);
	}
};

const pcap_if_t *cm::nps::NpsDevice::findDevice(const std::string_view &name) const {
	for (const auto &dev: m_devices) {
		if (dev->name == name) {
			return dev;
		}
	}
	return nullptr;
}

cm::nps::NpsError cm::nps::NpsDevice::openDevice(const std::string_view &device) {
	char err[PCAP_ERRBUF_SIZE] = {0};
	if (findDevice(device) == nullptr) {
		fmt::print("find device err {}\n", device);
		return NpsError::NO_DEVICE;
	}
	m_fp = pcap_open_live(device.data(), 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, err);
	if (m_fp == nullptr) {
		fmt::print("open device err {}\n", device);
		return NpsError::NO_DEVICE;
	}
	return NpsError::SUCCESS;
}


cm::nps::NpsError cm::nps::NpsDevice::sendPacket(const uint8_t ptk[], const int ptkLen) const {
	if (pcap_sendpacket(m_fp, ptk, ptkLen) == PCAP_ERROR) {
		fmt::print("capturePacketLoop: send packet err {}\n", pcap_geterr(m_fp));
		return NpsError::OPEN_DEVICE_ERROR;
	}
	return NpsError::SUCCESS;
}

void cm::nps::NpsDevice::capturePacketLoop(const pcap_handler callback, const int cnt) const {
	pcap_loop(m_fp, cnt, callback, nullptr);
}

cm::nps::NpsError cm::nps::NpsDevice::captureCompilePacketLoop(const pcap_handler callback, const std::string_view &protocol, const int cnt) const {
	bpf_program bp{};
	if (constexpr bpf_u_int32 bpf = 0; pcap_compile(m_fp, &bp, protocol.data(), 0, bpf) == PCAP_ERROR) {
		fmt::print("capturePacketLoop: pcap_compile err {}\n", pcap_geterr(m_fp));
		return NpsError::OPEN_DEVICE_ERROR;
	}

	if (pcap_setfilter(m_fp, &bp) == PCAP_ERROR) {
		fmt::print("Error setting the filter {}", pcap_geterr(m_fp));
		return NpsError::OPEN_DEVICE_ERROR;
	}

	pcap_loop(m_fp, cnt, callback, nullptr);
	return NpsError::SUCCESS;
}

void cm::nps::NpsDevice::PrintAllDevices() {
	for (const auto dev: m_devices) {
		PrintDevice(dev);
	}
}

void cm::nps::NpsDevice::PrintDevice(const pcap_if_t *const dev) {
	char ip6_addr[INET6_ADDRSTRLEN] = {0};
	fmt::print("{}\n", dev->name);
	if (dev->description) {
		fmt::print("\tDescription: {}\n", dev->description);
	}
	fmt::print("\tLoopback: {}\n", (dev->flags & PCAP_IF_LOOPBACK) ? "yes" : "no");

	for (const pcap_addr_t *addr = dev->addresses; addr; addr = addr->next) {
		fmt::print("\tAddress Family: #{}\n", addr->addr->sa_family);
		switch (addr->addr->sa_family) {
			case AF_INET:
				fmt::print("\tAddress Family Name: AF_INET\n");
				if (addr->addr)
					fmt::print("\tAddress: {}\n", inet_ntoa(reinterpret_cast<sockaddr_in *>(addr->addr)->sin_addr));
				if (addr->netmask)
					fmt::print("\tNetmask: {}\n", inet_ntoa(reinterpret_cast<sockaddr_in *>(addr->netmask)->sin_addr));
				if (addr->broadaddr)
					fmt::print("\tBroadcast Address: {}\n", inet_ntoa(reinterpret_cast<sockaddr_in *>(addr->broadaddr)->sin_addr));
				if (addr->dstaddr)
					fmt::print("\tDestination Address: {}\n", inet_ntoa(reinterpret_cast<sockaddr_in *>(addr->dstaddr)->sin_addr));
				break;

			case AF_INET6:
				fmt::print("\tAddress Family Name: AF_INET6\n");
				if (addr->addr) {
					fmt::print("\tAddress: {}\n", inet_ntop(AF_INET6, addr->addr, ip6_addr, sizeof(ip6_addr)));
				}
				break;

			default:
				printf("\tAddress Family Name: Unknown\n");
				break;
		}
	}
}
