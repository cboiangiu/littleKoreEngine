#include <Kore/IO/FileReader.h>

#include <iostream>

#include "lke_pipeline.hpp"

namespace lke {
LkePipeline::LkePipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
    createGraphicsPipeline(vertFilepath, fragFilepath);
}

Kore::Graphics5::Shader* LkePipeline::readAndCreateShader(const std::string& filepath, Kore::Graphics5::ShaderType type) {
    Kore::FileReader shader(filepath.c_str());
    return new Kore::Graphics5::Shader(shader.readAll(), shader.size(), type);
}

void LkePipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
    auto vertShader = readAndCreateShader(vertFilepath, Kore::Graphics5::VertexShader);
    auto fragShader = readAndCreateShader(fragFilepath, Kore::Graphics5::FragmentShader);
    
    std::cout << "VertexShader: " << vertShader->kincShader.impl.name << '\n';
    std::cout << "FragmentShader: " << fragShader->kincShader.impl.name << '\n';
    
    delete vertShader;
    delete fragShader;
}
}
