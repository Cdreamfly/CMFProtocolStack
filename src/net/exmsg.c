#include "net/exmsg.h"
#include "plat/sys_plat.h"

net_err_t exmsg_init(void)
{
    return NET_ERR_OK;
}

static void work_thread(void *arg)
{
    while (1)
    {
        sys_sleep(1);
    }
}

net_err_t exmsg_start(void)
{
    return NET_ERR_OK;
}