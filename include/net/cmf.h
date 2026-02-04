#ifndef CMF_H
#define CMF_H

typedef enum cmf_ret_val_e
{
    CMF_ERR_FAILED = -1,
    CMF_ERR_OK = 0,
    CMF_ERR_INPUT,
    CMF_ERR_ENTRY_INDEX,
    CMF_ERR_NULL_POINTER,
    CMF_ERR_OUT_OF_RANGE,
    CMF_ERR_NOT_INIT,
    CMF_ERR_NOT_SUPPORT,
    CMF_ERR_END
}cmf_ret_val_t;

#endif //CMF_H
