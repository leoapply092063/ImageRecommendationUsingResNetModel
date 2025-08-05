cmake_minimum_required(VERSION 3.0)
project(ImageSimilarity)

# Required for WebAssembly builds
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Detect if building with Emscripten
if (NOT EMSCRIPTEN)
    message(FATAL_ERROR "This project must be built with Emscripten.")
endif()

# Define source files
set(SOURCES
    main.cpp
)

# Build target
add_executable(resnet_simd ${SOURCES})

# Link ONNX Runtime if available
# (You must precompile or include it manually, this project assumes prebuilt wasm-compatible ONNX runtime)

# Emscripten-specific flags
set_target_properties(resnet_simd PROPERTIES
    LINK_FLAGS "
        -O3 
        -s WASM=1
        -s ALLOW_MEMORY_GROWTH=1
        -s EXPORT_NAME=\"Module\"
        -s MODULARIZE=1
        -s ENVIRONMENT=web
        --preload-file static@/static
        --no-entry
        -std=c++17
    "
)
