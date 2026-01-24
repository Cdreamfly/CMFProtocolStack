#include <string.h>

#include "evnet_handle.h"

static event_handle_t event_handle[EVENT_HANDLE_END];

void cmf_eventHandle_init()
{
    memset(event_handle, 0, EVENT_HANDLE_END * sizeof(event_handle_t));
}
void cmf_eventHandle_register(const event_handle_type_t handleType, const event_interval_t interval, func_event_handle handle)
{
    event_handle[handleType].interval = interval;
    event_handle[handleType].event_handle = handle;
}
void cmf_eventHandle(const event_interval_t interval, const uint32 load_share, const uint32 load_seed)
{
    for (event_handle_type_t handleType = 0; handleType < EVENT_HANDLE_END; handleType++)
    {
        if (NULL == event_handle[handleType].event_handle)
        {
            continue;
        }
        if (handleType % load_share != load_seed)
        {
            continue;
        }
        if (event_handle[handleType].interval == interval)
        {
            event_handle[handleType].event_handle();
        }
    }
}
