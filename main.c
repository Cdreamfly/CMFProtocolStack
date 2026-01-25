#include <unistd.h>
#include <pcap.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include "evnet_handle.h"
#include "protocol_app.h"
#include "timer.h"
void printPKInfo(const struct pcap_pkthdr *header, const u_char *packet){
    printf("数据包捕获时间: %s", ctime(&header->ts.tv_sec));
    printf("数据包捕获长度: %d\n", header->caplen);
    printf("数据包长度 %d\n", header->len);

    for (int i = 0; i < header->caplen; ++i) {
        if (i % 8 == 0 && i > 7)
            printf("\n");

        printf("%x\t", packet[i]);
    }
    printf("\n");

    struct ether_header *eth_header = (struct ether_header *) packet;
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        return;
    }

    /* Pointers to start point of various headers */
    const u_char *ip_header;
    const u_char *tcp_header;
    const u_char *payload;
    /* Header lengths in bytes */
    const int ethernet_header_length = 14; /* Doesn't change */
    /* Find start of IP header */
    ip_header = packet + ethernet_header_length;
    //ip层前8个位分别四位版本 四位长度
    int ip_header_length = ((*ip_header) & 0x0F);
    //首部长度的记录都是按照4个字节的单位进行增减的，所以我们算出4bit的首部长度的值之后，乘以4个字节，就可以知道首部的长度了，一个字节代表了8bit
    ip_header_length = ip_header_length * 4;
    printf("IP 头长度: %d\n", ip_header_length);

    //获取协议层
    u_char protocol = *(ip_header + 9);
    if (protocol != IPPROTO_TCP) {
        printf("Not a TCP packet. Skipping...\n\n");
        return;
    }

    //获取ip地址
    int src, dest;
    memcpy(&src, ip_header + 12, 4);
    memcpy(&dest, ip_header + 16, 4);

    char ipv4_str[64] = {'\0'};
    inet_ntop(AF_INET, &src, ipv4_str, 64);
    printf("源IP：%s\n", ipv4_str);


    char ipv4_dest[64] = {'\0'};
    inet_ntop(AF_INET, &dest, ipv4_dest, 64);
    printf("目标IP：%s\n", ipv4_dest);

    //tcp头开始位置
    tcp_header = packet + ethernet_header_length + ip_header_length;

    uint16_t srcport1, srcport2,destport1,destport2;
    memcpy(&srcport1, tcp_header , 2);
    memcpy(&destport1, tcp_header+ 2, 2);

    srcport2 = ntohs(srcport1);
    destport2 = ntohs(destport1);

    printf("srcPort:%d\n",srcport2);
    printf("destport:%d\n",destport2);

    //tcp长度 tcp首部偏移12字节后 前四位为长度
    int tcp_header_length = ((*(tcp_header + 12)) & 0xF0) >> 4;
    tcp_header_length = tcp_header_length * 4;
    printf("TCP 头长度: %d\n", tcp_header_length);

    /* 协议头总大小 */
    int total_headers_size = ethernet_header_length + ip_header_length + tcp_header_length;
    printf("所有协议头总长度: %d bytes\n", total_headers_size);

    //数据长度
    int payload_length = header->caplen - (ethernet_header_length + ip_header_length + tcp_header_length);
    printf("有效数据长度: %d bytes\n", payload_length);
    //数据头
    payload = packet + total_headers_size;
    printf("有效数据内存地址: %p\n", payload);
    printf("有效数据:[\t");
    if (payload_length > 0) {
        const u_char *temp_pointer = payload;
        int byte_count = 0;
        while (byte_count++ < payload_length) {
            printf("%c", *temp_pointer);
            temp_pointer++;
        }

    }
    printf("]\n\n");
}
void my_loop(u_char *args, const struct pcap_pkthdr *header, const u_char *packet){

    printPKInfo(header,packet);
}
int main() {
#if 0
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
#else
    char error_buffer[PCAP_ERRBUF_SIZE];
    /* 打开网络接口 */
    pcap_t *handle = pcap_open_live(
                         "eth0",//设备名称
                         BUFSIZ,//抓取个数 最大不超过65535
                         PCAP_OPENFLAG_PROMISCUOUS,     //混合模式
                         0, //超时时间
                         error_buffer);

    pcap_loop(handle, 0, my_loop, NULL);
    pcap_close(handle);
#endif
    return 0;
}
