#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

extern int cyw43_set_monitor_mode(cyw43_t *, int, void (*cb)(void *, int, size_t, const uint8_t *));

void monitor_mode_cb(void *data, int itf, size_t len, const uint8_t *buf) {
    char types [3][15] = {"Management", "Control", "Data"};
    char subtypes [20][20] = {"Association", "Association resp", "Reassociation", "Reassociation", "Probe req", "Probe resp", "Timing", "Reserved", "Beacon", "", "Disassociation", "Authentication", "Deauthentication", "", "Action"};
    CYW43_PRINTF("type=%d (%s) subtype=%d (%s) len=%d data=", buf[0] >> 2 & 3, types[buf[0] >> 2 & 3], buf[0] >> 4, subtypes[buf[0] >> 4], len);
    for (size_t i = 0; i < len; ++i) {
        CYW43_PRINTF(" %02x", buf[i]);
    }
    CYW43_PRINTF("\n");
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    const char *ap_name = "picow_test";
    const char *password = "password";
    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);
    cyw43_set_monitor_mode(&cyw43_state, 1, monitor_mode_cb);

    while(true) {
        sleep_ms(1000);
    }

    cyw43_arch_deinit();
    return 0;
}