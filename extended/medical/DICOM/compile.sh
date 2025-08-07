#!/bin/bash

# 设置 Geant4 安装路径
GEANT4_DIR=/Users/hjx/Documents/Geant4/geant4-v11.3.2-install

# 设置 DCMTK 安装路径
DCMTK_DIR=/Users/hjx/dcmtk-3.6.7-install

# 删除旧构建目录并创建新目录
rm -rf build
mkdir build
cd build

# 运行 cmake 配置
cmake .. \
  -DGeant4_DIR=${GEANT4_DIR}/lib/Geant4-11.3.2 \
  -DDCMTK_DIR=${DCMTK_DIR}/lib/cmake/dcmtk \
  -DDCMTK_INCLUDE_DIRS=${DCMTK_DIR}/include \
  -DDCMTK_LIBRARIES="${DCMTK_DIR}/lib/libdcmdata.a;${DCMTK_DIR}/lib/libofstd.a;${DCMTK_DIR}/lib/liboflog.a" \
  -DDICOM_USE_DCMTK=ON

# 编译
make -j$(sysctl -n hw.ncpu)