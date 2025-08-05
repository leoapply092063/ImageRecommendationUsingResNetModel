# ImageRecommendationUsingResNetModel

This project uses a ResNet50 ONNX model compiled to WebAssembly (via Emscripten) to extract feature vectors from images and recommend similar ones using cosine similarity.

## Features
- All processing is client-side using WebAssembly (no Python/OpenCV)
- Node.js + Express server to serve assets and HTML
- C++ performs feature extraction and cosine similarity
- Frontend in vanilla JavaScript displays interactive image selection and recommendations

## Structure
```
/
├── static/                  # Image files to recommend from
├── main.cpp                 # C++ source with ONNX feature extraction and cosine similarity
├── index.html               # UI with image grid and interactivity
├── server.js                # Express server
├── CMakeLists.txt           # CMake build definition for Emscripten
├── resnet50-v1-7.onnx       # ONNX model
```

## Build Instructions (Render Linux environment)
```
# Install Emscripten in your environment first
emcmake cmake .
emmake make
```

## Run Server
```
npm install
node server.js
```

Then go to: `http://localhost:8080` to test.

## Deployment
Compatible with Render (Linux). Configure it to:
- **Build Command**: `emcmake cmake . && emmake make`
- **Start Command**: `node server.js`
- **Node Build Environment**: With Emscripten installed

## License
MIT (or set your own)
