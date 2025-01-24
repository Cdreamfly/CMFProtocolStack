#include "Icmp.h"

#include <fmt/core.h>

cm::ip::icmp::Icmp::Icmp(const u_char *ptk): icmp_hdr{
    .type = ptk[0],
    .code = ptk[1],
    .checksum = static_cast<uint16_t>(ptk[2] << 8 | ptk[3]),
    .identifier = static_cast<uint16_t>(ptk[4] << 8 | ptk[5]),
    .sequence = static_cast<uint16_t>(ptk[6] << 8 | ptk[7])
} {
}

void cm::ip::icmp::Icmp::formatPrint() const {
    switch (icmp_hdr.type) {
        case ICMP_TYPE_ECHOREPLY:
            fmt::print("ping reply\n");
            break;
        case ICMP_TYPE_ECHOREQUEST:
            fmt::print("ping request\n");
            break;
        default:
            break;
    }
}
