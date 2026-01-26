#include "drv/nic.h"

static nic_operations_t* nic_operations_mapper[MAX_UNIT] = {NULL};
extern nic_operations_t nic_pcap_operations;

int32 drv_nic_mapper_init(const uint32 unit)
{
    nic_operations_mapper[unit] = &nic_pcap_operations;
    return RT_ERR_OK;
}

int32 drv_nic_init(const uint32 unit)
{
    return nic_operations_mapper[unit]->init(unit);
}

int32 drv_nic_exit(const uint32 unit)
{
    return nic_operations_mapper[unit]->exit(unit);
}

int32 drv_nic_rx_register(const uint32 unit, const uint8 priority, void* callback, void* pCookie, const uint32 flags)
{
    return nic_operations_mapper[unit]->rx_register(unit, priority, callback, pCookie, flags);
}

int32 drv_nic_tx_register(const uint32 unit, uint8* pPacket, void * callback, void *pCookie)
{
    return nic_operations_mapper[unit]->tx_register(unit, pPacket, callback, pCookie);
}

int32 drv_nic_rx_start(const uint32 unit)
{
    return nic_operations_mapper[unit]->rx_start(unit);
}

int32 drv_nic_rx_stop(const uint32 unit)
{
    return nic_operations_mapper[unit]->rx_stop(unit);
}