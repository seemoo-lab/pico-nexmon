add_executable(picow_monitor_mode
        picow_monitor_mode.c
        )
target_include_directories(picow_monitor_mode PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
        )
target_link_libraries(picow_monitor_mode
        pico_cyw43_arch_lwip_threadsafe_background
        pico_stdlib
        )

# Ucomment when using NEXMON driver
# target_compile_definitions(picow_monitor_mode PRIVATE
#         CYW43_CHIPSET_FIRMWARE_INCLUDE_FILE="${NEXMON_ROOT}/patches/bcm43439a0/7_95_49_2271bb6/nexmon/w43439A0_7_95_49_00_combined.h"
#         CYW43_WIFI_NVRAM_INCLUDE_FILE="${PICO_NEXMON_PATH}/cyw43-driver/firmware/wifi_nvram_43439.h"
#         CYW43_ENABLE_BLUETOOTH=0
#         )

pico_add_extra_outputs(picow_monitor_mode)

