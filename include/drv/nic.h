#ifndef DRV_NIC_H
#define DRV_NIC_H

#include <pcap/pcap.h>

#include "common/type.h"

typedef struct nic_operations_s
{
    int32 (*init)(uint32);
    int32 (*rx_register)(uint32, uint8, void *, void *, uint32);
} nic_operations_t;

extern int32 drv_nic_mapper_init(uint32 unit);

extern int32 drv_nic_init(uint32 unit);
extern int32 drv_nic_register(uint32 unit, uint8 priority, void * callback, void *pCookie, uint32 flags);


#endif //DRV_NIC_H
