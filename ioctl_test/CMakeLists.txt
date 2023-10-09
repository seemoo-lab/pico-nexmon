add_custom_target(nexmon_firmware ALL
                  COMMAND bash -c \"source ${NEXMON_ROOT}/setup_env.sh && make w43439A0_7_95_49_00_combined.h\"
                  WORKING_DIRECTORY ${NEXMON_ROOT}/patches/bcm43439a0/7_95_49_2271bb6/nexmon
                  )

add_executable(picow_ioctl_test
               picow_ioctl_test.c
               )

add_dependencies(picow_ioctl_test
                 nexmon_firmware
                 )

target_link_libraries(picow_ioctl_test
                      pico_stdlib
                      pico_cyw43_arch_none
                      )

target_compile_definitions(picow_ioctl_test PRIVATE
                           CYW43_CHIPSET_FIRMWARE_INCLUDE_FILE="${NEXMON_ROOT}/patches/bcm43439a0/7_95_49_2271bb6/nexmon/w43439A0_7_95_49_00_combined.h"
                           CYW43_WIFI_NVRAM_INCLUDE_FILE="${PICO_NEXMON_PATH}/cyw43-driver/firmware/wifi_nvram_43439.h"
                           CYW43_ENABLE_BLUETOOTH=0
                           )
