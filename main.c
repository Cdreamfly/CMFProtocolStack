#include <unistd.h>

#include "evnet_handle.h"
#include "protocol_app.h"
#include "timer.h"

int main() {
    cmf_eventHandle_init();
    cmf_eventHandle_register(EVENT_HANDLE_PROTO, INTERVAL_1000MS, rtk_proto_tick_handle_app);
    simple_timer_init();
    simple_timer_register(TIMER_TYPE_1S_0, cmf_eventHandle);
    rtk_proto_init();
    while (1)
    {
        simple_timer_is_expired(TIMER_TYPE_1S_0, INTERVAL_1000MS, 1, 0);
        usleep(10 * 1000); // 10ms = 10*1000 微秒
    }
    return 0;
}
