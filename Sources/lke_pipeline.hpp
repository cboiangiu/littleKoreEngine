#pragma once

#include <Kore/Graphics5/PipelineState.h>
#include <Kore/Graphics5/Shader.h>

#include <string>
#include <vector>

namespace lke {
struct PipelineConfigInfo {
    Kore::Graphics5::CullMode cullMode;
    bool depthWrite;
    Kore::Graphics5::ZCompareMode depthMode;
    Kore::Graphics5::ZCompareMode stencilMode;
    Kore::Graphics5::StencilAction stencilBothPass;
    Kore::Graphics5::StencilAction stencilDepthFail;
    Kore::Graphics5::StencilAction stencilFail;
    int stencilReferenceValue;
    int stencilReadMask;
    int stencilWriteMask;
    Kore::Graphics5::BlendingOperation blendSource;
    Kore::Graphics5::BlendingOperation blendDestination;
    Kore::Graphics5::BlendingOperation alphaBlendSource;
    Kore::Graphics5::BlendingOperation alphaBlendDestination;
    bool colorWriteMaskRed[8]; // Per render target
    bool colorWriteMaskGreen[8];
    bool colorWriteMaskBlue[8];
    bool colorWriteMaskAlpha[8];
    int colorAttachmentCount;
    Kore::Graphics5::RenderTargetFormat colorAttachment[8];
    int depthAttachmentBits;
    int stencilAttachmentBits;
    bool conservativeRasterization;
};

class LkePipeline {
public:
    LkePipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    ~LkePipeline();
    
    LkePipeline(const LkePipeline &) = delete;
    void operator=(const LkePipeline &) = delete;
    
    static PipelineConfigInfo defaultPipelineConfigInfo();
    
private:
    static Kore::Graphics5::Shader* readAndCreateShader(const std::string& filepath, Kore::Graphics5::ShaderType type);
    
    void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    
    Kore::Graphics5::PipelineState* graphicsPipeline;
};
}
