#include <Kore/IO/FileReader.h>

#include "lke_pipeline.hpp"
#include "lke_model.hpp"

namespace lke
{
LkePipeline::LkePipeline(const std::string& vertFilepath, const std::string& fragFilepath,
    const PipelineConfigInfo& configInfo)
{
    auto vertShader = readAndCreateShader(vertFilepath, Kore::Graphics5::VertexShader);
    auto fragShader = readAndCreateShader(fragFilepath, Kore::Graphics5::FragmentShader);

    graphicsPipeline = new Kore::Graphics5::PipelineState();

    graphicsPipeline->vertexShader = vertShader;
    graphicsPipeline->fragmentShader = fragShader;

    auto structure = LkeModel::Vertex::getStructure();
    graphicsPipeline->inputLayout[0] = &structure;
    graphicsPipeline->inputLayout[1] = nullptr;

    // Start: configure pipeline from configInfo
    graphicsPipeline->cullMode = configInfo.cullMode;

    graphicsPipeline->depthWrite = configInfo.depthWrite;
    graphicsPipeline->depthMode = configInfo.depthMode;

    graphicsPipeline->stencilMode = configInfo.stencilMode;
    graphicsPipeline->stencilBothPass = configInfo.stencilBothPass;
    graphicsPipeline->stencilDepthFail = configInfo.stencilDepthFail;
    graphicsPipeline->stencilFail = configInfo.stencilFail;
    graphicsPipeline->stencilReferenceValue = configInfo.stencilReferenceValue;
    graphicsPipeline->stencilReadMask = configInfo.stencilReadMask;
    graphicsPipeline->stencilWriteMask = configInfo.stencilWriteMask;

    graphicsPipeline->blendSource = configInfo.blendSource;
    graphicsPipeline->blendDestination = configInfo.blendDestination;
    graphicsPipeline->alphaBlendSource = configInfo.alphaBlendSource;
    graphicsPipeline->alphaBlendDestination = configInfo.alphaBlendDestination;

    for (int i = 0; i < 8; ++i)
    {
        graphicsPipeline->colorWriteMaskRed[i] = configInfo.colorWriteMaskRed[i];
        graphicsPipeline->colorWriteMaskGreen[i] = configInfo.colorWriteMaskRed[i];
        graphicsPipeline->colorWriteMaskBlue[i] = configInfo.colorWriteMaskRed[i];
        graphicsPipeline->colorWriteMaskAlpha[i] = configInfo.colorWriteMaskRed[i];
    }

    graphicsPipeline->colorAttachmentCount = configInfo.colorAttachmentCount;
    for (int i = 0; i < 8; ++i)
    {
        graphicsPipeline->colorAttachment[i] = configInfo.colorAttachmentsFormat;
    }

    graphicsPipeline->depthAttachmentBits = configInfo.depthAttachmentBits;
    graphicsPipeline->stencilAttachmentBits = configInfo.stencilAttachmentBits;

    graphicsPipeline->conservativeRasterization = configInfo.conservativeRasterization;
    // End: configure pipeline from configInfo

    graphicsPipeline->compile();

    delete vertShader;
    delete fragShader;
}

LkePipeline::~LkePipeline()
{
    delete graphicsPipeline;
}

Kore::Graphics5::Shader* LkePipeline::readAndCreateShader(
    const std::string& filepath, Kore::Graphics5::ShaderType type)
{
    Kore::FileReader shader(filepath.c_str());
    return new Kore::Graphics5::Shader(shader.readAll(), shader.size(), type);
}

void LkePipeline::bind(Kore::Graphics5::CommandList* list)
{
    list->setPipeline(graphicsPipeline);
    list->setPipelineLayout();
}

void LkePipeline::defaultPipelineConfigInfo(PipelineConfigInfo& configInfo)
{
    configInfo.cullMode = Kore::Graphics5::NoCulling;

    configInfo.depthWrite = true;
    configInfo.depthMode = Kore::Graphics5::ZCompareLess;

    configInfo.stencilMode = Kore::Graphics5::ZCompareLess;
    configInfo.stencilBothPass = Kore::Graphics5::Keep;
    configInfo.stencilDepthFail = Kore::Graphics5::Keep;
    configInfo.stencilFail = Kore::Graphics5::Keep;
    configInfo.stencilReferenceValue = 0;
    configInfo.stencilReadMask = 0xff;
    configInfo.stencilWriteMask = 0xff;

    configInfo.blendSource = Kore::Graphics5::BlendOne;
    configInfo.blendDestination = Kore::Graphics5::BlendZero;
    configInfo.alphaBlendSource = Kore::Graphics5::BlendOne;
    configInfo.alphaBlendDestination = Kore::Graphics5::BlendZero;

    for (int i = 0; i < 8; ++i)
    {
        configInfo.colorWriteMaskRed[i] = true;
        configInfo.colorWriteMaskGreen[i] = true;
        configInfo.colorWriteMaskBlue[i] = true;
        configInfo.colorWriteMaskAlpha[i] = true;
    }

    configInfo.colorAttachmentCount = 1;
    configInfo.colorAttachmentsFormat = Kore::Graphics5::Target32Bit;

    configInfo.depthAttachmentBits = 0;
    configInfo.stencilAttachmentBits = 0;

    configInfo.conservativeRasterization = false;
}

void LkePipeline::enableAlphaBlending(PipelineConfigInfo& configInfo)
{
    configInfo.blendSource = Kore::Graphics5::SourceAlpha;
    configInfo.blendDestination = Kore::Graphics5::InverseSourceAlpha;
}
}
