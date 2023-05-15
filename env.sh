#!/bin/bash
set -x
CC_PREFIX=/home/li/Templates/new_pj/Embedded-system-competition/project/config/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.1
export PATH=$CC_PREFIX/bin/:$PATH
export LD_LIBRARY_PATH=$CC_PREFIX/lib:$LD_LIBRARY_PATH
set +x
