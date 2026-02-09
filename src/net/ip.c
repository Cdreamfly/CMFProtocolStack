#include "net/ip.h"

#include "net/cmf.h"

int32 ip_init(void)
{
    return CMF_ERR_OK;
}

int32 ip_input(pktBuf_t* pBuf)
{
    return CMF_ERR_OK;
}

int32 ip_output(pktBuf_t* pBuf, ip_addr_t* pIpAddr)
{
    return CMF_ERR_OK;
}

uint16 ip_checksum(register uint16* addr, register int len)
{
    return CMF_ERR_OK;
}
