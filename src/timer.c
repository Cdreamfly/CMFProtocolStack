#include <string.h>
#include <time.h>

#include "common/def.h"
#include "common/cmf_debug.h"
#include "net/cmf.h"
#include "timer.h"

static timer_bus_t timerBus[TIMER_TYPE_END];

static uint64 get_current_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void simple_timer_init(void)
{
    memset(timerBus, 0, sizeof(timerBus));
}

int32 simple_timer_register(const timer_type_t timerType, const func_timer_f timer_func)
{
    CMF_PARAM_CHK("func_timer null", NULL == timer_func, return CMF_ERR_NULL_POINTER);
    timerBus[timerType].timer.interval_ms = INTERVAL_1000MS;
    timerBus[timerType].timer.last_tick_ms = get_current_ms();
    timerBus[timerType].func_timer = timer_func;
    return CMF_ERR_OK;
}

int32 simple_timer_is_expired(const timer_type_t timerType, const event_interval_t interval, const uint32 load_share, const uint32 load_seed)
{
    const uint64 current_ms = get_current_ms();
    for (timer_type_t i = 0; i < TIMER_TYPE_END; i++)
    {
        if (i != timerType)
        {
            continue;
        }
        if ((current_ms - timerBus[i].timer.last_tick_ms) >= timerBus[i].timer.interval_ms)
        {
            timerBus[i].func_timer(interval, load_share, load_seed);
            timerBus[i].timer.last_tick_ms = current_ms;
            break;
        }
    }
    return CMF_ERR_OK;
}
