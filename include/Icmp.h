#pragma once
#include <sys/types.h>

#include "Hdr.h"

namespace cm::ip::icmp {
    class Icmp {
    public:
        explicit Icmp(const u_char *);

        void formatPrint() const;

        ~Icmp() =default;
    private:
        icmp_hdr_t icmp_hdr;
    };
}
