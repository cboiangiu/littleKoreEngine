#pragma once

#include <Kore/Math/Vector.h>
#include <Kore/Graphics5/VertexStructure.h>
#include <Kore/Graphics5/CommandList.h>
#include <Kore/Graphics5/Graphics.h>

// std
#include <memory>
#include <vector>

namespace lke
{
class LkeModel
{
public:
    struct Vertex
    {
        Kore::vec3 position{};
        Kore::vec3 color{};

        static Kore::Graphics5::VertexStructure getStructure();

        bool operator==(const Vertex& other) const
        {
            return position == other.position && color == other.color;
        }
    };

    struct Builder
    {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string& filepath);
    };

    LkeModel(const LkeModel::Builder& builder);
    ~LkeModel();

    LkeModel(const LkeModel&) = delete;
    LkeModel& operator=(const LkeModel&) = delete;

    static std::unique_ptr<LkeModel> createModelFromFile(const std::string& filepath);

    void bind(Kore::Graphics5::CommandList* commandList);
    void draw(Kore::Graphics5::CommandList* commandList);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
    void createIndexBuffers(const std::vector<uint32_t>& indices);

    Kore::Graphics5::VertexBuffer* vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    Kore::Graphics5::IndexBuffer* indexBuffer;
    uint32_t indexCount;
};
}
