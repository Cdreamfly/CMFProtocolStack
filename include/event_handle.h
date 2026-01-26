#ifndef EVENT_HANDLE_H
#define EVENT_HANDLE_H

#include "common/type.h"

typedef enum event_interval_e
{
    INTERVAL_0 = 0,
    INTERVAL_500MS = 500,
    INTERVAL_1000MS = 1000,
    INTERVAL_END
} event_interval_t;

typedef enum event_handle_type_e
{
    EVENT_HANDLE_CLI,
    EVENT_HANDLE_PROTO,
    EVENT_HANDLE_HTTPD,
    EVENT_HANDLE_OTHER,
    EVENT_HANDLE_END
} event_handle_type_t;

typedef int32 (*func_event_handle)(void);

typedef struct event_handle_e{
    event_interval_t interval;
    func_event_handle event_handle;
} event_handle_t;

extern void cmf_eventHandle_init();
extern void cmf_eventHandle_register(event_handle_type_t handleType, event_interval_t interval, func_event_handle handle);
extern void cmf_eventHandle(event_interval_t interval, uint32 load_share, uint32 load_seed);

#endif //EVENT_HANDLE_H
