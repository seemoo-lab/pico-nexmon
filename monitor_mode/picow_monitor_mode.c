#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define MONITOR_DISABLED        0
#define MONITOR_IEEE80211       1
/* RADIOTAP MODE REQUIRES A NEXMON FW! */
#define MONITOR_RADIOTAP        2
#define MONITOR_LOG_ONLY        16

const char *frame_type_names[3] = {
            "Management",
            "Control",
            "Data"
        };
const char *frame_subtype_names[4][16] = {
        {
            "Association Request", "Association Response", "Reassociation Request", "Reassociation Response",
            "Probe Request", "Probe Response", "Timing Advertisement", "Reserved",
            "Beacon", "ATIM", "Disassociation", "Authentication", "Deauthentication", "Action", "Action No Ack (NACK)", "Reserved"
        },
        {
            "Reserved", "Reserved", "Trigger[3]", "TACK",
            "Beamforming Report Poll", "VHT/HE NDP Announcement", "Control Frame Extension", "Control Wrapper",
            "Block Ack Request (BAR)", "Block Ack (BA)", "PS-Poll", "RTS", "CTS", "ACK", "CF-End", "CF-End + CF-ACK"
        },
        {
            "Data", "Reserved", "Reserved", "Reserved",
            "Null (no data)", "Reserved", "QoS Data", "QoS Data + CF-ACK",
            "QoS Data + CF-Poll", "QoS Data + CF-ACK + CF-Poll", "QoS Null (no data)", "Reserved", "QoS CF-Poll (no data)", "QoS CF-ACK + CF-Poll (no data)", "Reserved", "Reserved"
        },
        {
            "DMG Beacon", "S1G Beacon", "Reserved", "Reserved",
            "Reserved", "Reserved", "Reserved", "Reserved",
            "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"
        }
    };

void monitor_mode_cb(void *data, int itf, size_t len, const uint8_t *buf) {
    uint16_t offset_80211 = 0;
    if (cyw43_state.is_monitor_mode == MONITOR_RADIOTAP)
        offset_80211 = *(uint16_t*)(buf+2);
    uint8_t frame_type = buf[offset_80211] >> 2 & 3;
    uint8_t frame_subtype = buf[offset_80211] >> 4;
    printf("Frame type=%d (%s) subtype=%d (%s) len=%d data=", frame_type, frame_type_names[frame_type], frame_subtype, frame_subtype_names[frame_type][frame_subtype], len);
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
    return;
}

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("failed to initialise\n");
        return 1;
    }

    const char *ap_name = "picow_test";
    const char *password = "password";
    uint32_t channels[] = {1, 6, 11};
    uint8_t chan_idx = 0;
    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);
    cyw43_set_monitor_mode(&cyw43_state, MONITOR_IEEE80211, monitor_mode_cb);

    while(true) {
        cyw43_wifi_ap_set_channel(&cyw43_state, channels[chan_idx]);
        chan_idx = (chan_idx + chan_idx) % (sizeof(channels)/sizeof(channels[0]));
        sleep_ms(200);
    }

    cyw43_arch_deinit();
    return 0;
}