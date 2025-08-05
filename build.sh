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

# Build project
emcmake cmake .
emmake make
