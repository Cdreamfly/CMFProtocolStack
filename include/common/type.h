#ifndef TYPE_H
#define TYPE_H

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef unsigned char unchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed char int8;
typedef signed short int16;
typedef int int32;
typedef long long int64;

typedef unsigned char uchar;
//typedef unsigned int size_t;

typedef enum rtk_enable_e
{
    DISABLED = 0,
    ENABLED,
    RTK_ENABLE_END
} rtk_enable_t;

typedef enum init_state_e
{
    INIT_NOT_COMPLETED = 0,
    INIT_COMPLETED,
    INIT_STATE_END
} init_state_t;

typedef enum drv_nic_rx_e
{
    NIC_RX_NOT_HANDLED = 0,
    NIC_RX_HANDLED,
    NIC_RX_HANDLED_OWNED
} drv_nic_rx_t;

#define RT_ERR_OK 0
#define RT_ERR_FAILED -1

#endif //TYPE_H
