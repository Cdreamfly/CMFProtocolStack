#ifndef CMF_H
#define CMF_H

typedef enum cmf_ret_val_e
{
    CMF_OK = 0,
    CMF_ERR,
    CMF_ERR_NULLPOINT,
    CMF_ERR_OUTOFMEMORY,
    CMF_ERR_NOTINITIALIZED,
    CMF_ERR_NOTEXIST,
    CMF_ERR_NOT_SUPPORT
}cmf_ret_val_t;

#endif //CMF_H
