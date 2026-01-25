#include <string.h>

#include "protocol.h"

#include "common/cmf_debug.h"

static rtk_proto_func_t proto_func[RTK_PROTO_END] = {0};

rtk_proto_func_t* rtk_proto_func_get()
{
    return proto_func;
}

int32 rtk_proto_func_init()
{
    memset(proto_func, 0, sizeof(proto_func));
    return RT_ERR_OK;
}

int32 rtk_proto_func_register(const rtk_proto_t id, const init_func init, const enable_func status_set,
                              const judge_func judge, const routine_func routine, const tick_handle_func tick_handle)
{
    CMF_PARAM_CHK("rtk_proto_t err", id >= RTK_PROTO_END, return RT_ERR_FAILED);
    proto_func[id].init = init;
    proto_func[id].status_set = status_set;
    proto_func[id].judge = judge;
    proto_func[id].routine = routine;
    proto_func[id].tick_handle = tick_handle;
    return RT_ERR_OK;
}

int32 rtk_proto_func_unregister(const rtk_proto_t id)
{
    CMF_PARAM_CHK("rtk_proto_t err", id >= RTK_PROTO_END, return RT_ERR_FAILED);
    proto_func[id].init = NULL;
    proto_func[id].status_set = NULL;
    proto_func[id].judge = NULL;
    proto_func[id].routine = NULL;
    proto_func[id].tick_handle = NULL;
    return RT_ERR_OK;
}

int32 rtk_proto_registered_proto_init()
{
    int32 ret = RT_ERR_FAILED;
    for (rtk_proto_t id = 0; id < RTK_PROTO_END; id++)
    {
        if (NULL != proto_func[id].init)
        {
            RT_ERR_CHK(proto_func[id].init(), ret);
        }
    }
    return RT_ERR_OK;
}

int32 rtk_proto_registered_proto_enable(const rtk_enable_t status)
{
    int32 ret = RT_ERR_FAILED;
    for (rtk_proto_t id = 0; id < RTK_PROTO_END; id++)
    {
        if (NULL != proto_func[id].status_set)
        {
            RT_ERR_CHK(proto_func[id].status_set(status), ret);
        }
    }
    return RT_ERR_OK;
}

int32 rtk_proto_registered_proto_tick_handle()
{
    int32 ret = RT_ERR_FAILED;
    for (rtk_proto_t id = 0; id < RTK_PROTO_END; id++)
    {
        if (NULL != proto_func[id].tick_handle)
        {
            RT_ERR_CHK(proto_func[id].tick_handle(), ret);
        }
    }
    return RT_ERR_OK;
}
