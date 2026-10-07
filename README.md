# 环境

- 开发板：ATK-STM32L4-Pandora v2.61
- 芯片：STM32L475VETx

# 使用

## 编译

```shell
$ cd project/led_flash
$ rm -rf build

# gcc
$ cmake -B build \
        -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain.cmake \
        -DTOOLCHAIN=gcc \
        -DTOOLCHAIN_BIN_DIR=xxx
$ cmake --build build

# armclang
$ cmake -B build \
        -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchain.cmake \
        -DTOOLCHAIN=armclang \
        -DTOOLCHAIN_BIN_DIR=xxx
$ cmake --build build
```

注意：

1. 如果工具链路径已经加载到系统的 `PATH` 里面，则不需要添加 `TOOLCHAIN_BIN_DIR`。

## 打印信息

打印信息使用的是 SEGGER RTT 组件（开发板自带 ST-Link）。

### PyOCD

安装：

> 依赖 Python

```shell
# 安装 PyOCD
$ pip install pyocd

# 安装 Pack
$ pyocd pack install STM32L4

# 查找芯片对应的 Pack 名称
$ pyocd pack find STM32L4
  Part             Vendor               Pack                 Version   Installed
----------------------------------------------------------------------------------
  ...
  STM32L475VETx    STMicroelectronics   Keil.STM32L4xx_DFP   3.1.0     True
  ...
```

查看地址：

```shell
# gcc
$ arm-none-eabi-nm ./build/led_flash.elf | grep _SEGGER_RTT
200005e4 B _SEGGER_RTT

# armclang
$ fromelf --text -s ./build/led_flash.elf | grep _SEGGER_RTT
    586  _SEGGER_RTT                0x20000010   Gb    3  Data  Hi   0xa8
```

连接：

```shell
# 检测是否识别到 ST-Link
$ pyocd list
  #   Probe/Board    Unique ID                  Target
--------------------------------------------------------
  0   STM32 STLink   xxxxxxxxxxxxxxxxxxxxxxxx   n/a

# 连接 ST-Link
# -a 表示 --address, 手动指定 RTT 控制块在目标 MCU 内存中的地址
$ pyocd rtt -t STM32L475VETx -a 0x20000010
```

当前测试发现，使用 PyOCD 查看的话，输出打印信息的速度很慢。

### probe-rs

[Github 仓库](https://github.com/probe-rs/probe-rs)

安装：

```shell
# Windows
$ irm https://github.com/probe-rs/probe-rs/releases/latest/download/probe-rs-tools-installer.ps1 | iex

# Linux	/ MacOS
$ curl --proto '=https' --tlsv1.2 -LsSf https://github.com/probe-rs/probe-rs/releases/latest/download/probe-rs-tools-installer.sh | sh
```

连接：

```shell
# 连接到一个已经在运行的设备并查看 RTT 日志
$ probe-rs attach --chip STM32L475VETx --rtt-up-channels 0 ./build/led_flash.elf

# 烧录固件并查看 RTT 日志
$ probe-rs run --chip STM32L475VETx --rtt-up-channels 0 ./build/led_flash.elf
```

当前测试发现，使用 probe-rs 查看的话，输出打印信息的速度还行。

# 调试

## VSCode

