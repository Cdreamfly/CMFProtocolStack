#ifndef TIMER_H
#define TIMER_H

#include "evnet_handle.h"
#include "common/type.h"

typedef enum timer_type_e
{
    TIMER_TYPE_SYS_TIME = 0,
    TIMER_TYPE_1S_0,
    TIMER_TYPE_1S_1,
    TIMER_TYPE_1S_2,
    TIMER_TYPE_500MS_0,
    TIMER_TYPE_END
} timer_type_t;

typedef struct simple_timer_s{
    uint32 interval_ms;  // 注册的定时间隔（毫秒）
    uint64 last_tick_ms; // 上次定时到达的时间戳（毫秒，内部使用）
} simple_timer_t;

typedef int32 (*func_timer_f)(const event_interval_t interval, const uint32 load_share, const uint32 load_seed);
typedef struct {
    simple_timer_t timer;          // 定时句柄
    func_timer_f func_timer;       // 该定时触发后执行的业务回调
} timer_bus_t;

extern void simple_timer_init(void);
extern int32 simple_timer_register(timer_type_t timerType, func_timer_f timer_func);
extern int32 simple_timer_is_expired(timer_type_t timerType, const event_interval_t interval, const uint32 load_share, const uint32 load_seed);

#endif //TIMER_H
