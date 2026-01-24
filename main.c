#include <unistd.h>

#include "evnet_handle.h"
#include "timer.h"
#include "common/cmf_debug.h"

void rldp_tick_handle()
{
    debug_printf("RLDP tick handle\n");
}



int main() {
    cmf_eventHandle_init();
    cmf_eventHandle_register(EVENT_HANDLE_RLDP, INTERVAL_1000MS, rldp_tick_handle);
    simple_timer_init();
    simple_timer_register(TIMER_TYPE_1S_0, cmf_eventHandle);

    while (1)
    {
        simple_timer_is_expired(TIMER_TYPE_1S_0, INTERVAL_1000MS, 1, 0);
        usleep(10 * 1000); // 10ms = 10*1000 微秒
    }
    return 0;
}
