#pragma once

#include <Kore/Graphics5/CommandList.h>
#include <Kore/Graphics5/PipelineState.h>
#include <Kore/Graphics5/Shader.h>
#include <Kore/Graphics5/CommandList.h>
#include <Kore/Graphics5/VertexStructure.h>

// std
#include <string>

namespace lke
{
struct PipelineConfigInfo
{
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
    Kore::Graphics5::BlendingFactor blendSource;
    Kore::Graphics5::BlendingFactor blendDestination;
    Kore::Graphics5::BlendingFactor alphaBlendSource;
    Kore::Graphics5::BlendingFactor alphaBlendDestination;
    Kore::Graphics5::BlendingOperation blendOperation;
    Kore::Graphics5::BlendingOperation alphaBlendOperation;
    bool colorWriteMaskRed[8]; // Per render target
    bool colorWriteMaskGreen[8];
    bool colorWriteMaskBlue[8];
    bool colorWriteMaskAlpha[8];
    int colorAttachmentCount;
    int depthAttachmentBits;
    int stencilAttachmentBits;
    bool conservativeRasterization;
    Kore::Graphics5::RenderTargetFormat colorAttachmentsFormat;
};

class LkePipeline
{
public:
    LkePipeline(const std::string& vertFilepath, const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo);
    ~LkePipeline();

    LkePipeline(const LkePipeline&) = delete;
    void operator=(const LkePipeline&) = delete;

    void bind(Kore::Graphics5::CommandList* list);

    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
    static void enableAlphaBlending(PipelineConfigInfo& configInfo);

private:
    static Kore::Graphics5::Shader* readAndCreateShader(
        const std::string& filepath, Kore::Graphics5::ShaderType type);

    Kore::Graphics5::PipelineState* graphicsPipeline;
};
}
