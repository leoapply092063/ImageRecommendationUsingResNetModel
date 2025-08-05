#!/bin/bash
set -e

# Save current path
PROJECT_DIR=$(pwd)

# Install Emscripten
git clone https://github.com/emscripten-core/emsdk.git || true
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd "$PROJECT_DIR"


# -------------------------------
# Install CMake 3.28.3 locally
# -------------------------------
CMAKE_VERSION=3.28.3
wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz
tar -xzf cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz
export PATH=$PWD/cmake-${CMAKE_VERSION}-linux-x86_64/bin:$PATH


# Clone and build ONNX Runtime for WASM
git clone --recursive https://github.com/microsoft/onnxruntime.git || true
cd onnxruntime

./build.sh \
  --config Release \
  --build_wasm \
  --parallel \
  --skip_tests \
  --cmake_extra_defines CMAKE_BUILD_TYPE=Release

# Copy ONNX Runtime headers and library to a known location in your project
cd "$PROJECT_DIR"
mkdir -p onnxruntime/include onnxruntime/lib
cp -r onnxruntime/include/onnxruntime onnxruntime/include/
cp onnxruntime/build/wasm/Release/libonnxruntime.a onnxruntime/lib/

# Build your project
emcmake cmake .
emmake make