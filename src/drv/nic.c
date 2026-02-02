#include "drv/nic.h"

#include <pcap/pcap.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#include <net/ethernet.h>   // 以太网标准头
#include <netinet/ip.h>     // IPv4标准头
#include <netinet/udp.h>    // UDP标准头
#include <netinet/tcp.h>    // TCP标准头
#include <arpa/inet.h>      // IP格式化
#include <netinet/ip_icmp.h>
#include "protocol.h"

typedef struct nic_pcap_e
{
    pcap_t* pcap_handle;
    char err[PCAP_ERRBUF_SIZE];
    void (*rx_callback)(uint32 unit, void* pPacket, void* pCookie);
    void* pCookie;
    uint8_t inited;
    pthread_t rx_thread;
    uint8_t rx_thread_running;
} nic_pcap_t;

static nic_pcap_t nic_pcap[MAX_UNIT] = {0};

void pcap_rx_dump(const struct pcap_pkthdr* header, const u_char* pkt_data)
{
    const struct ether_header* eth = (const struct ether_header*)pkt_data;
    const struct ip* ip = (const struct ip*)(pkt_data + 14);
    const uint32_t ip_hlen = (ip->ip_hl & 0x0F) * 4; // IPv4头长度

    // 仅处理IPv4报文（帧类型0x0800），其他协议直接跳过
    if (ntohs(eth->ether_type) != ETHERTYPE_IP) return;

    // ====================== 以太网层 ======================
    printf("  [以太网层] 源MAC：%02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],
           eth->ether_shost[3],eth->ether_shost[4],eth->ether_shost[5]);
    printf("  [以太网层] 目的MAC：%02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_dhost[0],eth->ether_dhost[1],eth->ether_dhost[2],
           eth->ether_dhost[3],eth->ether_dhost[4],eth->ether_dhost[5]);
    printf("  [以太网层] 帧类型：0x%04x(IPv4)\n", ntohs(eth->ether_type));

    // ====================== IPv4层 ======================
    printf("  [IPv4层] 源IP：%s\n", inet_ntoa(ip->ip_src));
    printf("  [IPv4层] 目的IP：%s\n", inet_ntoa(ip->ip_dst));
    printf("  [IPv4层] 协议号：%d ", ip->ip_p);
    switch(ip->ip_p) {
        case IPPROTO_ICMP: printf("(ICMP)\n"); break;
        case IPPROTO_UDP:  printf("(UDP)\n"); break;
        case IPPROTO_TCP:  printf("(TCP)\n"); break;
        default:           printf("(其他)\n"); break;
    }

    // ====================== 传输层 ======================
    printf("  [传输层] ");
    switch (ip->ip_p) {
        case IPPROTO_ICMP: // ICMP(ping)
            const struct icmp* icmp = (const struct icmp*)((u_char*)ip + ip_hlen);
            printf("ICMP：类型%d(%s) 序列号：%d\n", icmp->icmp_type,
                   icmp->icmp_type==ICMP_ECHO?"请求":"响应", ntohs(icmp->icmp_seq));
            break;
        case IPPROTO_UDP: // UDP
            const struct udphdr* udp = (const struct udphdr*)((u_char*)ip + ip_hlen);
            printf("UDP：源端口%d → 目的端口%d\n", ntohs(udp->uh_sport), ntohs(udp->uh_dport));
            break;
        case IPPROTO_TCP: // TCP
            const struct tcphdr* tcp = (const struct tcphdr*)((u_char*)ip + ip_hlen);
            printf("TCP：源端口%d → 目的端口%d\n", ntohs(tcp->th_sport), ntohs(tcp->th_dport));
            break;
        default: // 其他传输层协议
            printf("暂不解析\n");
            break;
    }
    printf("---------------------------------------------------\n"); // 包分隔线
}

static void* nic_pcap_rx_thread(void* arg)
{
    const uint32 unit = (uint32)(uintptr_t)arg;
    nic_pcap_t* pNic = &nic_pcap[unit];
    struct pcap_pkthdr* pPktHdr = NULL;
    const uint8* pPacketData = NULL;

    if (CMF_FALSE == pNic->inited || NULL == pNic->pcap_handle)
    {
        fprintf(stderr, "nic unit %u not inited, rx thread exit\n", unit);
        pthread_exit(NULL);
    }

    pNic->rx_thread_running = CMF_TRUE;
    fprintf(stdout, "nic unit %u rx thread start success\n", unit);

    while (CMF_TRUE == pNic->rx_thread_running)
    {
        const int ret = pcap_next_ex(pNic->pcap_handle, &pPktHdr, &pPacketData);
        switch (ret)
        {
        case 1:
            pcap_rx_dump(pPktHdr, pPacketData);
            if (NULL != pNic->rx_callback)
            {
                pNic->rx_callback(unit, (void*)pPacketData, pNic->pCookie);
            }
            break;
        case 0:
            usleep(1000);
            break;
        default: // 抓包错误（-1）或离线抓包结束（-2），退出循环
            fprintf(stderr, "nic unit %u pcap capture error: %s\n", unit, pcap_geterr(pNic->pcap_handle));
            pNic->rx_thread_running = CMF_FALSE;
            break;
        }
    }
    pNic->rx_thread_running = CMF_FALSE;
    fprintf(stdout, "nic unit %u rx thread exit normal\n", unit);
    pthread_exit(NULL);
}

int32 drv_nic_pcap_init(const uint32 unit)
{
    if (unit >= MAX_UNIT)
    {
        return RT_ERR_FAILED;
    }
    if (nic_pcap[unit].inited)
    {
        return RT_ERR_OK;
    }
    nic_pcap[unit].pcap_handle = pcap_open_live("eth0", 65535, 1, 1, nic_pcap[unit].err);
    if (NULL == nic_pcap[unit].pcap_handle)
    {
        fprintf(stderr, "open dev %s failed: %s\n", "eth0", nic_pcap[unit].err);
        return RT_ERR_FAILED;
    }
    nic_pcap[unit].inited = CMF_TRUE;
    return RT_ERR_OK;
}

int32 drv_nic_pcap_exit(const uint32 unit)
{
    if (unit >= MAX_UNIT || CMF_FALSE == nic_pcap[unit].inited)
    {
        return RT_ERR_OK;
    }

    nic_pcap_t* pNic = &nic_pcap[unit];

    if (CMF_TRUE == pNic->rx_thread_running)
    {
        pNic->rx_thread_running = CMF_FALSE;
        pthread_join(pNic->rx_thread, NULL);
    }

    if (NULL != pNic->pcap_handle)
    {
        pcap_close(pNic->pcap_handle);
        pNic->pcap_handle = NULL;
    }

    memset(pNic, 0, sizeof(nic_pcap_t));
    fprintf(stdout, "nic unit %u exit success, all resource released\n", unit);
    return RT_ERR_OK;
}

int32 drv_nic_rx_pcap_register(const uint32 unit, uint8 priority, void* callback, void* pCookie, uint32 flags)
{
    if (unit >= MAX_UNIT || NULL == callback)
    {
        return RT_ERR_FAILED;
    }
    if (CMF_FALSE == nic_pcap[unit].inited)
    {
        return RT_ERR_FAILED;
    }
    nic_pcap[unit].rx_callback = callback;
    nic_pcap[unit].pCookie = pCookie;
    return RT_ERR_OK;
}

int32 drv_nic_tx_pcap_register(const uint32 unit, uint8* pPacket, void* callback, void* pCookie)
{
    if (unit >= MAX_UNIT || CMF_FALSE == nic_pcap[unit].inited)
    {
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}

int32 drv_nic_rx_pcap_start(const uint32 unit)
{
    int ret = RT_ERR_OK;
    if (unit >= MAX_UNIT || CMF_FALSE == nic_pcap[unit].inited)
    {
        fprintf(stderr, "nic unit %u not inited, rx start failed\n", unit);
        return RT_ERR_FAILED;
    }

    nic_pcap_t* pNic = &nic_pcap[unit];
    if (CMF_TRUE == pNic->rx_thread_running)
    {
        return RT_ERR_OK;
    }
    if (NULL == pNic->rx_callback)
    {
        fprintf(stderr, "nic unit %u no rx callback registered, rx start failed\n", unit);
        return RT_ERR_FAILED;
    }
    ret = pthread_create(&pNic->rx_thread, NULL, nic_pcap_rx_thread, (void*)(uintptr_t)unit);
    if (0 != ret)
    {
        fprintf(stderr, "nic unit %u create rx thread failed, err: %d\n", unit, ret);
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}

int32 drv_nic_rx_pcap_stop(const uint32 unit)
{
    if (unit >= MAX_UNIT || CMF_FALSE == nic_pcap[unit].inited || CMF_FALSE == nic_pcap[unit].rx_thread_running)
    {
        return RT_ERR_OK;
    }

    nic_pcap[unit].rx_thread_running = CMF_FALSE;
    fprintf(stdout, "nic unit %u rx stop command send, waiting thread exit\n", unit);
    return RT_ERR_OK;
}

nic_operations_t nic_pcap_operations = {
    .init = &drv_nic_pcap_init,
    .exit = &drv_nic_pcap_exit,
    .rx_register = &drv_nic_rx_pcap_register,
    .tx_register = &drv_nic_tx_pcap_register,
    .rx_start = &drv_nic_rx_pcap_start,
    .rx_stop = &drv_nic_rx_pcap_stop
};
