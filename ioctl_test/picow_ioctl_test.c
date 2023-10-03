#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cyw43_internal.h"
#include <stdlib.h>

#define DEBUG_INFO_PTR 0xF4
#define CONSOLE_LENGTH 1024

#define NEX_WRITE_TO_CONSOLE 401

#define IOCTL_CMD_GET(cmd) (cmd<<1)
#define IOCTL_CMD_SET(cmd) ((cmd<<1)+1)

extern uint32_t cyw43_read_backplane_external(void *self, uint32_t addr, size_t size);

void dump_from_address(void *base_address, size_t length) {
    uint32_t address;
    uint8_t *buf = malloc(length);
    if (buf != 0) {
        for (size_t i = 0; i < length; i++) {
            address = (uint32_t)base_address + i;
            buf[i] = cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), address, 1);
        }
        printf("%s", buf);
    }
}

int main() {
    char str[] = "IOCTL test";
    size_t str_len = 11;

    stdio_init_all();
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY)) {
        printf("Wi-Fi init failed");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    void *debug_info = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), DEBUG_INFO_PTR, 4);
    void *console = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), (uint32_t)debug_info + 48, 4);
    void *buffer = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), (uint32_t)console + 8, 4);

    cyw43_ioctl(&cyw43_state, IOCTL_CMD_SET(NEX_WRITE_TO_CONSOLE), str_len, (uint8_t *)str, CYW43_ITF_STA);

    printf("Console:\n");
    dump_from_address(buffer, CONSOLE_LENGTH);
}
