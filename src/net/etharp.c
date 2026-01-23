#include "net/etharp.h"
#include "net/cmf.h"
#include "net/cmf_opts.h"

static etharp_entry_t arp_type[CMF_ETHARP_ARP_TABLE_SIZE] = {0};

int32 etharp_init()
{
    return CMF_OK;
}
