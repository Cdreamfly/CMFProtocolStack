#ifndef DEF_H
#define DEF_H

#include "common/type.h"

#if !defined(NULL)
  #if defined(__GNUC__)
    #define NULL (__null)
  #elif !defined(__cplusplus)
    #define NULL ((void *)0)
  #else
    #define NULL (0)
  #endif
#endif

#define CMF_TRUE 1
#define CMF_FALSE 0

#define ETH_HWADDR_LED  (6)

#define IP_ADDR_LED (4)

typedef struct eth_addr_s
{
    uint8 addr[ETH_HWADDR_LED];
} eth_addr_t;

typedef struct ip_addr_s
{
    uint8 addr[IP_ADDR_LED];
} ip_addr_t;

#define CMF_PARAM_CHK(message,expression,handler)  do { if (expression) { \
    CMF_DEBUG(CMF_DBG_NO_MODULE | DBG_LV_ERR,"%s", message); handler; }} while(0)

#define CMF_OP_CHK(message, op, handler) do { if ((op) != CMF_OK) { \
    CMF_DEBUG(CMF_DBG_NO_MODULE | DBG_LV_ERR, "%s",message); handler; }} while(0)

#define ETH_ADDR_COPY(da, sa)    \
    do {   \
        da[0] = sa[0];  \
        da[1] = sa[1];  \
        da[2] = sa[2];  \
        da[3] = sa[3];  \
        da[4] = sa[4];  \
        da[5] = sa[5];  \
    } while(0)

#define IS_ETH_ADDR_EQUAL(eth1, eth2)   \
    ((eth1[0] == eth2[0]) && \
    (eth1[1] == eth2[1]) && \
    (eth1[2] == eth2[2]) && \
    (eth1[3] == eth2[3]) && \
    (eth1[4] == eth2[4]) && \
    (eth1[5] == eth2[5]))


#define IPADDR_COPY(dip, sip)   \
    do {    \
        dip[0] = sip[0]; \
        dip[1] = sip[1]; \
        dip[2] = sip[2]; \
        dip[3] = sip[3]; \
    } while(0)

#define IS_IPADDR_EQUAL(ip1, ip2)   \
    ((ip1[0]==ip2[0]) && \
    (ip1[1]==ip2[1])  && \
    (ip1[2]==ip2[2])  && \
    (ip1[3]==ip2[3]))

#define IS_IPADDR_ZERO(ip)   \
    ((ip[0]==0) && \
    (ip[1]==0)  && \
    (ip[2]==0)  && \
    (ip[3]==0))

#define SET_IPADDR_ZERO(ip)   \
    do { \
        ip[0]=0; \
        ip[1]=0; \
        ip[2]=0; \
        ip[3]=0; \
    } while(0)


#endif //DEF_H
