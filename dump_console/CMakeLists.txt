add_executable(picow_dump_console
               picow_dump_console.c
               )

target_link_libraries(picow_dump_console
                      pico_stdlib
                      pico_cyw43_arch_none
                      )

target_compile_definitions(picow_dump_console PRIVATE
                           CYW43_CHIPSET_FIRMWARE_INCLUDE_FILE="${PICO_NEXMON_PATH}/cyw43-driver/firmware/w43439A0_7_95_49_00_combined.h"
                           CYW43_WIFI_NVRAM_INCLUDE_FILE="${PICO_NEXMON_PATH}/cyw43-driver/firmware/wifi_nvram_43439.h"
                           CYW43_ENABLE_BLUETOOTH=0
                           )
