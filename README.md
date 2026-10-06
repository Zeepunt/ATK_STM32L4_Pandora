# 环境

- 开发板：ATK-STM32L4-Pandora v2.61

# 编译

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

# 调试

## VSCode

