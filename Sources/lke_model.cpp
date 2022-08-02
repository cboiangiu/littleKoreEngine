#include "lke_model.hpp"
#include "lke_utils.hpp"

#include <Kore/IO/FileReader.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tinyobjloader/tiny_obj_loader.h"

// std
#include <cassert>
#include <cstring>
#include <unordered_map>

namespace std
{
//template <>
//struct hash<Kore::vec3>
//{
//    size_t operator()(Kore::vec3 const& vec3) const
//    {
//        std::size_t seed = 3;
//        for(int i = 0; i < 3; ++i) {
//          auto x = vec3[i];
//          x = ((x >> 16) ^ x) * 0x45d9f3b;
//          x = ((x >> 16) ^ x) * 0x45d9f3b;
//          x = (x >> 16) ^ x;
//          seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
//        }
//        return seed;
//    }
//};

template <>
struct hash<lke::LkeModel::Vertex>
{
    size_t operator()(lke::LkeModel::Vertex const& vertex) const
    {
        size_t seed = 0;
//              lke::LkeUtils::hashCombine(seed, vertex.position, vertex.color);
        return seed;
    }
};
}

namespace lke
{
LkeModel::LkeModel(const LkeModel::Builder& builder)
{
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

LkeModel::~LkeModel()
{
    delete vertexBuffer;
    delete indexBuffer;
}

std::unique_ptr<LkeModel> LkeModel::createModelFromFile(const std::string& filepath)
{
    Builder builder{};
    builder.loadModel(filepath);
    return std::make_unique<LkeModel>(builder);
}

std::unique_ptr<LkeModel> LkeModel::createFromBuffers(const std::vector<Vertex>& vertices,
                                                      const std::vector<uint32_t>& indices)
{
    Builder builder{};
    builder.loadModelFromBuffers(vertices, indices);
    return std::make_unique<LkeModel>(builder);
}

void LkeModel::createVertexBuffers(const std::vector<Vertex>& vertices)
{
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");

    vertexBuffer = new Kore::Graphics5::VertexBuffer(vertexCount, Vertex::getStructure(), false);
    float* v = vertexBuffer->lock();

    int i = -1;
    for (auto vertex : vertices)
    {
        v[++i] = vertex.position.x();
        v[++i] = vertex.position.y();
        v[++i] = vertex.position.z();
        v[++i] = vertex.color.x();
        v[++i] = vertex.color.y();
        v[++i] = vertex.color.z();
    }
    vertexBuffer->unlock();
}

void LkeModel::createIndexBuffers(const std::vector<uint32_t>& indices)
{
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 0;

    if (!hasIndexBuffer)
    {
        return;
    }

    indexBuffer = new Kore::Graphics5::IndexBuffer(indexCount, true);
    int* data = indexBuffer->lock();

    for (int i = 0; i < indexCount; ++i)
    {
        data[i] = indices[i];
    }

    indexBuffer->unlock();
}

void LkeModel::draw(Kore::Graphics5::CommandList* commandList)
{
    if (hasIndexBuffer)
    {
        commandList->drawIndexedVertices();
    }
    else
    {
        //      vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        // FIXME: wtf
    }
}

void LkeModel::bind(Kore::Graphics5::CommandList* commandList)
{
    int offsets[1] = { 0 };
    commandList->setVertexBuffers(&vertexBuffer, offsets, 1);

    if (hasIndexBuffer)
    {
        commandList->setIndexBuffer(*indexBuffer);
    }
}

Kore::Graphics5::VertexStructure LkeModel::Vertex::getStructure()
{
    Kore::Graphics5::VertexStructure structure{};

    structure.add("position", Kore::Graphics5::VertexData::Float3VertexData);
    structure.add("color", Kore::Graphics5::VertexData::Float3VertexData);

    return structure;
}

void LkeModel::Builder::loadModel(const std::string& filepath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    
    Kore::FileReader model(filepath.c_str());
    imemstream in(static_cast<char*>(model.readAll()), model.size());

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &in))
    {
        throw std::runtime_error(warn + err);
    }

    vertices.clear();
    indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                };
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void LkeModel::Builder::loadModelFromBuffers(const std::vector<Vertex>& vertices,
                                             const std::vector<uint32_t>& indices)
{
    this->vertices.clear();
    this->indices.clear();
    
    this->vertices = vertices;
    this->indices = indices;
}
}
