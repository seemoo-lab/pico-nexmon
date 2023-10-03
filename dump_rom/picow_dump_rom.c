#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "cyw43_internal.h"

/* dump parameters */
#define SI_ARMCM3_ROM       0x1e000000
#define SI_ARMCM3_ROM_SZ    0xa0000
#define BASE_ADDR           SI_ARMCM3_ROM
#define LENGTH              SI_ARMCM3_ROM_SZ
#define BLOCK_SZ            0x10000
#define ROUNDS              LENGTH / BLOCK_SZ
#define READ_SZ             4

extern uint32_t cyw43_read_backplane_external(void *self, uint32_t addr, size_t size);

/* print bytes as hex */
static
void
printf_xxd(size_t len, const uint8_t *buf, bool newline)
{
    while (len--) {
        printf("%02x", *buf++);
        if (len > 0) 
            printf(" ");
    }
    if (newline) 
        printf("\n");
}

int
main()
{
    /* chip init begin */
    stdio_init_all();
    if (cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY)) {
        printf("Wi-Fi init failed");
        return -1;
    }
    cyw43_arch_enable_sta_mode();
    /* chip init end */

    uint32_t buf[BLOCK_SZ >> 2] = {0};
    uint32_t address;

    for (size_t i = 0; i < ROUNDS; i++) {
        address = BASE_ADDR + i * BLOCK_SZ;
        for (size_t ii = 0; ii < (BLOCK_SZ / READ_SZ); ii++) {
            buf[ii] = cyw43_read_backplane_external((cyw43_int_t *)(&cyw43_state.cyw43_ll), address + READ_SZ * ii, READ_SZ);
        }
        printf_xxd(BLOCK_SZ, (uint8_t *)buf, false);
    }
}
