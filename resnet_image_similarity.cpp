// main.cpp - C++ WebAssembly code to extract feature vectors from images using ResNet50 ONNX model
#include <emscripten.h>
#include <emscripten/bind.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <cmath>
#include <algorithm>
#include "onnxruntime_cxx_api.h"

constexpr int FEATURE_VECTOR_SIZE = 2048;

std::unordered_map<std::string, std::vector<float>> imageFeatureMap;
std::vector<std::string> allImageNames;

// Dummy image loader (replace with actual wasm-supported image tensor conversion)
std::vector<float> loadImageAsTensor(const std::string& imagePath) {
    std::vector<float> tensor(3 * 224 * 224, 0.5f); // dummy image data
    return tensor;
}

std::vector<float> extractFeatureVector(Ort::Session& session, const std::vector<float>& inputTensor) {
    Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    std::array<int64_t, 4> inputShape{1, 3, 224, 224};
    Ort::Value inputTensorOrt = Ort::Value::CreateTensor<float>(memoryInfo, const_cast<float*>(inputTensor.data()), inputTensor.size(), inputShape.data(), inputShape.size());

    const char* inputNames[] = {session.GetInputName(0, Ort::AllocatorWithDefaultOptions())};
    const char* outputNames[] = {session.GetOutputName(0, Ort::AllocatorWithDefaultOptions())};

    auto outputTensors = session.Run(Ort::RunOptions{nullptr}, inputNames, &inputTensorOrt, 1, outputNames, 1);
    float* outputData = outputTensors.front().GetTensorMutableData<float>();
    return std::vector<float>(outputData, outputData + FEATURE_VECTOR_SIZE);
}

float cosineSimilarity(const std::vector<float>& a, const std::vector<float>& b) {
    float dot = 0.0f, normA = 0.0f, normB = 0.0f;
    for (int i = 0; i < FEATURE_VECTOR_SIZE; ++i) {
        dot += a[i] * b[i];
        normA += a[i] * a[i];
        normB += b[i] * b[i];
    }
    return dot / (std::sqrt(normA) * std::sqrt(normB) + 1e-6);
}

// Called from JS to initialize feature extraction
EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("initModel", &[](emscripten::val imageList) {
        Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "image-similarity");
        Ort::SessionOptions sessionOptions;
        Ort::Session session(env, "resnet50-v1-7.onnx", sessionOptions);

        int length = imageList["length"].as<int>();
        for (int i = 0; i < length; ++i) {
            std::string imagePath = imageList[i].as<std::string>();
            allImageNames.push_back(imagePath);
            std::vector<float> tensor = loadImageAsTensor("static/" + imagePath);
            imageFeatureMap[imagePath] = extractFeatureVector(session, tensor);
        }
    });

    emscripten::function("getSimilarImages", &[](std::string selectedImage) -> emscripten::val {
        auto selectedVec = imageFeatureMap[selectedImage];
        std::vector<std::pair<std::string, float>> similarities;

        for (const auto& [name, vec] : imageFeatureMap) {
            if (name == selectedImage) continue;
            float sim = cosineSimilarity(selectedVec, vec);
            similarities.push_back({name, sim});
        }
        std::sort(similarities.begin(), similarities.end(), [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

        emscripten::val result = emscripten::val::array();
        for (int i = 0; i < 5 && i < similarities.size(); ++i) {
            result.set(i, similarities[i].first);
        }
        return result;
    });
}
