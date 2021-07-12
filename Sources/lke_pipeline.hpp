#pragma once

#include <Kore/Graphics5/Shader.h>

#include <string>
#include <vector>

namespace lke {
class LkePipeline {
public:
    LkePipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    
private:
    static Kore::Graphics5::Shader* readAndCreateShader(const std::string& filepath, Kore::Graphics5::ShaderType type);
    
    void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
};
}
