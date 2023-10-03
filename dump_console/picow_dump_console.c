#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cyw43_internal.h"

#define DEBUG_INFO_PTR 0xF4
#define CONSOLE_LENGTH 1024

extern uint32_t cyw43_read_backplane_external(void *self, uint32_t addr, size_t size);

int main() {
    uint32_t address;
    uint8_t buf[CONSOLE_LENGTH+1] = {0};

    stdio_init_all();
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY)) {
        printf("Wi-Fi init failed");
        return -1;
    }
    cyw43_arch_enable_sta_mode();

    void *debug_info = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), DEBUG_INFO_PTR, 4);
    void *console = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), (uint32_t)debug_info + 48, 4);
    void *buffer = (void *)cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), (uint32_t)console + 8, 4);

    for (size_t i = 0; i < CONSOLE_LENGTH; i++) {
        address = (uint32_t)buffer + i;
        buf[i] = cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), address, 1);
    }

    printf("Console:\n");
    printf("%s", buf);
}
