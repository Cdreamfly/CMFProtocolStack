#include <unistd.h>
#include "net/net.h"

int main() {
    net_init();
    net_start();
    while (1)
    {
        sleep(1);
    }
    return 0;
}
