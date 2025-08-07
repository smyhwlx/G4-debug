#!/bin/bash

# 编译脚本 - 重新编译DICOM示例
echo "开始重新编译DICOM示例..."

# 检查当前目录
if [ ! -f "DICOM.cc" ]; then
    echo "错误：请在DICOM示例目录下运行此脚本"
    exit 1
fi

# 删除旧的build目录
if [ -d "build" ]; then
    echo "删除旧的build目录..."
    rm -rf build
fi

# 创建新的build目录
echo "创建新的build目录..."
mkdir build
cd build

cmake ..

# 编译
make -j$(nproc)

# 检查编译是否成功
if [ $? -eq 0 ]; then
    echo "编译成功！"
    echo "可执行文件位置: $(pwd)/DICOM"
else
    echo "编译失败！"
    exit 1
fi

echo "编译完成！"
cd build