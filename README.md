# Raspberry Pi Pico W Nexmon Applications
Applications for the Raspberry Pi Pico W related to WLAN firmware patching for the [bcm/cyw43439a0](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-4-802.11n/cyw43439/) using [Nexmon: The C-based Firmware Patching Framework](https://github.com/seemoo-lab/nexmon) for Broadcom/Cypress WiFi chips.

## Applications

App|Description
---|---
[dump_rom<sup>1</sup>](dump_rom) | Read ROM content from WLAN SoC and hex dump it to Pico's console.
[dump_console<sup>1</sup>](dump_console) | Read WLAN SoC's ARM core internal console and dump it to Pico's console.
[ioctl_test<sup>2</sup>](ioctl_test) | Write string via IOCTL to SoC's internal console, read back internal console, and dump it to Pico's console.

<sup>1</sup> Works with unmodified firmware provided by [cyw43-driver](https://github.com/georgerobotics/cyw43-driver).  
<sup>2</sup> Requires firmware that implements the `NEX_WRITE_TO_CONSOLE` IOCTL, uses [this patch](https://github.com/seemoo-lab/nexmon/tree/master/patches/bcm43439a0/7_95_49_2271bb6/nexmon) by default.

## Setup
There are various standard setups that allow working with a Raspberry Pi Pico W. Our setup exists of a single [Raspberry Pi Pico W](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html) powered over USB and a [Raspberry Pi Debug Probe](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html). The Debug Probe is connected to a Linux host system via USB and to the Pico's UART Serial and SWD Debug ports, as described [here](https://www.raspberrypi.com/documentation/microcontrollers/debug-probe.html#getting-started). The applications provided here do not depend on the setup around your Pico W, hence, if you are running into general setup problems, please consult the official documentation and getting started guides for the Raspberry Pi Pico (W): [documentation page](https://www.raspberrypi.com/documentation/microcontrollers/).

## Getting started
1. Clone this repository, and check out submodules ([cyw43-driver](https://github.com/georgerobotics/cyw43-driver)):
    ```
    git clone --recursive https://github.com/seemoo-lab/pico-nexmon.git
    ```
2. Patch submodules:
    ```
    cd pico-nexmon
    chmod u+x script/patch && script/patch
    ```
3. Setup [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk):  
    - This repository depends on the [pico-sdk](https://github.com/raspberrypi/pico-sdk). We recommend to head over to the [pico-sdk](https://github.com/raspberrypi/pico-sdk) repository, create a working setup by following its [README](https://github.com/raspberrypi/pico-sdk/blob/master/README.md), and build and try at least some of the [pico-examples](https://github.com/raspberrypi/pico-examples) for the Pico W before continuing here.
    - **If you have a working Pico SDK and Pico W setup**, ensure that the environment variable `PICO_SDK_PATH` is set to the SDK's root before calling CMake, or directly pass the SDK location to CMake with `-DPICO_SDK_PATH=<path to pico-sdk>`.
4. Setup [Nexmon](https://github.com/seemoo-lab/nexmon.git):  
    - You might skip setting up Nexmon, if you are only interested in the `dump_rom` and/or `dump_console` applications. In this case, provide `-DNO_NEXMON=1` to CMake.
    - **If you already have a working copy of the Nexmon framework**, ensure that the environment variable `NEXMON_ROOT` is set to the framework's root before calling CMake, or directly pass the framework location to CMake with `-DNEXMON_ROOT=<path to nexmon>`.
5. Build the applications:  
    1. Create and enter build directory.  
        ```
        mkdir build && cd build
        ```
    2. Use CMake to build the targets of your choice. Below are a couple examples:  
        - *Example A)* `PICO_SDK_PATH` and `NEXMON_ROOT` directly provided to CMake, build all applications:
            ```
            cmake -DPICO_SDK_PATH=<path to pico-sdk> -DNEXMON_ROOT=<path to nexmon> ..
            make
            ```
        - *Example B)* `PICO_SDK_PATH` and `NEXMON_ROOT` set as environment variables, build all applications:  
            ```
            export PICO_SDK_PATH=<path to pico-sdk>
            export NEXMON_ROOT=<path to nexmon>
            cmake ..
            make
            ```
        - *Example C)* `PICO_SDK_PATH` set as environment variable, do not use nexmon, build all applications that do not depend on nexmon:
            ```
            export PICO_SDK_PATH=<path to pico-sdk>
            cmake -DNO_NEXMON=1 ..
            make
            ```
        - *Example D)* `PICO_SDK_PATH` directly provided to CMake, do not use nexmon, build only `dump_rom` application:  
            ```
            cmake -DPICO_SDK_PATH=<path to pico-sdk> -DNO_NEXMON=1 ..
            make picow_dump_rom
            ```
6. Load and operate the application of your choice on your Raspberry Pi Pico W:
    - **This step depends on your setup.** Please follow the official instructions from Raspberry Pi's documentation and getting started guides that match your setup.  
    - One example that works with our [setup](#setup) is to listen to the UART output using `minicom` and load the `dump_console` application with `openocd` as follows:  
        ```
        minicom -b 115200 -o -D /dev/ttyACM0
        ```  
        ```
        openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" -c "program dump_console/picow_dump_console.elf verify reset exit"
        ```
        ![](gfx/dump_console_example.gif)

## Quickstart
We provide scripts that automate the setup of `pico-sdk`, `openocd`, `minicom`, and `nexmon` on Ubuntu 22.04.3 LTS machines. Just copy and paste the following:  
*We tested the scripts on a VM running [Ubuntu Server 22.04.3 LTS](https://releases.ubuntu.com/22.04.3/ubuntu-22.04.3-live-server-amd64.iso). They probably work on other versions and distributions too, but this is up to you to find out.*  
```
sudo apt update && \
sudo apt install -y git && \
git clone --recursive https://github.com/seemoo-lab/pico-nexmon.git && \
cd pico-nexmon && \
./script/patch && \
./script/setup_pico && \
./script/setup_nexmon && \
source ~/.bashrc
```
Once the setup is done, you might built all applications by executing the following:
```
mkdir build && \
cd build && \
cmake .. && \
make
```
For an example on how to use the applications check out step 6 of the [getting started section](#getting-started).

## Contact
[Jakob Link](https://www.seemoo.tu-darmstadt.de/team/jlink/) <<jlink@seemoo.tu-darmstadt.de>>

## Reference the `pico-nexmon` project and the `nexmon` project:
Any use of this project which results in an academic publication or other publication which includes a bibliography should include a citation to the pico-nexmon project and the Nexmon project:  
- J. Link and M. Hollick, ‘pico-nexmon: Raspberry Pi Pico W Nexmon Applications’, 2023. Available: https://github.com/seemoo-lab/pico-nexmon.  
    [**CITATION.cff**](CITATION.cff)
- M. Schulz, D. Wegemer, and M. Hollick, ‘Nexmon: The C-based Firmware Patching Framework’, 2017. Available: https://nexmon.org.  
    [**CITATION.cff**](https://github.com/seemoo-lab/nexmon/blob/master/CITATION.cff) 

# Powered By
## Secure Mobile Networking Lab (SEEMOO)
![SEEMOO](gfx/logo_seemoo.png)
## Multi-Mechanisms Adaptation for the Future Internet (MAKI)
![MAKI](gfx/logo_maki.png)
## Technische Universität Darmstadt
![TU Darmstadt](gfx/logo_tud.png)


