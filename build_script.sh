#!/bin/bash

set -e  # Exit on first error

# Step 1: Install Emscripten SDK (if not already installed)
echo "Cloning and installing Emscripten SDK..."
git clone https://github.com/emscripten-core/emsdk.git || true
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..

# Step 2: Build using CMake + Make
echo "Building project with Emscripten..."
emcmake cmake .
emmake make

echo "Build completed successfully."
