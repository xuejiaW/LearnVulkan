#include "ModelsMgr.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include "../Vertex/Vertex.h"

#include <unordered_map>

#include "../Vertex/VertexDataMgr.h"

const std::string MODEL_PATH = "../Models/viking_room.obj";
const std::string TEXTURE_PATH = "../Textures/viking_room.png";

void ModelsMgr::loadModel()
{
    VertexDataMgr::vertices.clear();
    VertexDataMgr::indices.clear();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, MODEL_PATH.c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

            vertex.color = {1.0f, 1.0f, 1.0f};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(VertexDataMgr::vertices.size());
                VertexDataMgr::vertices.push_back(vertex);
            }

            VertexDataMgr::indices.push_back(uniqueVertices[vertex]);
        }
    }
}

