#include "drv/nic.h"

int32 drv_nic_pcap_init(uint32 unit)
{
    return RT_ERR_OK;
}
;
int32 drv_nic_pcap_register(uint32 unit, uint8 priority, void * callback, void *pCookie, uint32 flags)
{
    return RT_ERR_OK;
}
nic_operations_t nic_pcap_operations = {
    .init = &drv_nic_pcap_init,
    .rx_register = &drv_nic_pcap_register
};
