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

pico_enable_stdio_usb(picow_monitor_mode 1)
pico_enable_stdio_uart(picow_monitor_mode 0)

pico_add_extra_outputs(picow_monitor_mode)

