#ifndef CMF_DEBUG_H
#define CMF_DEBUG_H

#include <stdio.h>

#define DEBUG 1

#if DEBUG
#define debug_printf(format, ...) \
    printf("[ %s %s:%d ] " format, __FILE_NAME__, __func__, __LINE__, ##__VA_ARGS__)
#else
#define debug_printf(format, ...)
#endif

#define CMF_DBG_NO_MODULE   0x08
#define CMF_DBG_OFF_MODULE  0x00

#define CMF_DBG_ETH     CMF_DBG_NO_MODULE
#define CMF_DBG_ARP     CMF_DBG_NO_MODULE
#define CMF_DBG_IP4     CMF_DBG_NO_MODULE
#define CMF_DBG_UDP     CMF_DBG_NO_MODULE
#define CMF_DBG_TCP     CMF_DBG_NO_MODULE
#define CMF_DBG_ICMP    CMF_DBG_NO_MODULE
#define CMF_DBG_HTTPD   CMF_DBG_NO_MODULE
#define CMF_DBG_WEB     CMF_DBG_NO_MODULE
#define CMF_DBG_TFTPC   CMF_DBG_NO_MODULE
#define CMF_DBG_TELNETD CMF_DBG_NO_MODULE
#define CMF_DBG_PBUF    CMF_DBG_NO_MODULE
#define CMF_DBG_SOCKET  CMF_DBG_NO_MODULE

typedef enum dbg_lv_e {
    DBG_LV_FUNC_ENTER = 1,
    DBG_LV_ERR,
    DBG_LV_INFO,
    DBG_LV_MAX
} dbg_lv_t;

#define CMF_DBG_MIN_LEVEL   DBG_LV_FUNC_ENTER
#define CMF_DBG_MASK_LEVEL  DBG_LV_INFO

#if DEBUG
#define CMF_DEBUG(dbg, format, ...) \
    do { \
        if (((dbg) & CMF_DBG_NO_MODULE) && \
            (((dbg) & CMF_DBG_MASK_LEVEL) >= CMF_DBG_MIN_LEVEL) && \
            (((dbg) & CMF_DBG_MASK_LEVEL) < DBG_LV_MAX)) { \
            debug_printf(format, ##__VA_ARGS__); \
        } \
    } while (0)
#else
#define CMF_DEBUG(dbg, format, ...)
#endif

#endif //CMF_DEBUG_H
