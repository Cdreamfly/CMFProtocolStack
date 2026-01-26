#ifndef DRV_NIC_H
#define DRV_NIC_H

#include "common/def.h"

typedef struct nic_operations_s
{
    int32 (*init)(uint32);
    int32 (*exit)(uint32);
    int32 (*rx_register)(uint32, uint8, void *, void *, uint32);
    int32 (*tx_register)(uint32, uint8*, void *, void *);
    int32 (*rx_start)(uint32);
    int32 (*rx_stop)(uint32);
} nic_operations_t;

extern int32 drv_nic_mapper_init(uint32 unit);

extern int32 drv_nic_init(uint32 unit);
extern int32 drv_nic_exit(uint32 unit);
extern int32 drv_nic_rx_register(uint32 unit, uint8 priority, void * callback, void *pCookie, uint32 flags);
extern int32 drv_nic_tx_register(uint32 unit, uint8* pPacket, void * callback, void *pCookie);
extern int32 drv_nic_rx_start(uint32 unit);
extern int32 drv_nic_rx_stop(uint32 unit);

#endif //DRV_NIC_H
