#include "drv/nic.h"

static nic_operations_t *nic_operations_mapper[1] = { NULL };
extern nic_operations_t nic_pcap_operations;

extern int32 drv_nic_mapper_init(const uint32 unit)
{
    nic_operations_mapper[unit] = &nic_pcap_operations;
    return RT_ERR_OK;
}

extern int32 drv_nic_init(const uint32 unit)
{
    return nic_operations_mapper[unit]->init(unit);
}
extern int32 drv_nic_register(const uint32 unit, const uint8 priority, void* callback, void *pCookie, uint32 flags)
{
    return nic_operations_mapper[unit]->rx_register(unit, priority, callback, pCookie, flags);
}
