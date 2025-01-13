#pragma once

#include <pcap.h>
#include <vector>

namespace cm::nps {
	enum class NpsError {
		SUCCESS = 0,
		NO_DEVICE,
		OPEN_DEVICE_ERROR
	};


	class NpsDevice {
	public:
		NpsDevice();

		~NpsDevice();

		[[nodiscard]] const pcap_if_t *findDevice(const std::string_view &) const;

		NpsError openDevice(const std::string_view & = "any");

		NpsError sendPacket(const uint8_t[], int) const;

		void capturePacketLoop(pcap_handler callback, int = 0) const;

		NpsError captureCompilePacketLoop(pcap_handler callback, const std::string_view &, int = 0) const;

		void PrintAllDevices();

	private:
		void PrintDevice(const pcap_if_t *);

		pcap_t *m_fp;
		pcap_if_t *m_allDevs;
		std::vector<pcap_if_t *> m_devices;
	};
}
