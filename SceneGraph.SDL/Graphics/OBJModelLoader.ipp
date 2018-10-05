#include "ModelLoader.h"
#include "Material.h"
#include "Vertex.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace kitsune::scenegraph::sdl::graphics::implementations {

class OBJModelLoader : public ModelLoader {
    typedef ArrayBuffer<unsigned int, 1> IBType;
    typedef ArrayBuffer<vertex::PositionNormalUVVertex, 1> VBType;

public:
    void load(std::shared_ptr<Node> node,
            const std::string& filename,
            BufferAccess indexBufferAccess = BufferAccess::Draw,
            BufferFrequency indexBufferFrequency = BufferFrequency::Static,
            BufferAccess vertexBufferAccess = BufferAccess::Draw,
            BufferFrequency vertexBufferFrequency = BufferFrequency::Static) override {
        std::ifstream filestream(filename, std::ios::in);

        load(node, filestream, indexBufferAccess, indexBufferFrequency, vertexBufferAccess, vertexBufferFrequency);

        filestream.close();
    }

    void load(std::shared_ptr<Node> node,
            std::istream& stream,
            BufferAccess indexBufferAccess = BufferAccess::Draw,
            BufferFrequency indexBufferFrequency = BufferFrequency::Static,
            BufferAccess vertexBufferAccess = BufferAccess::Draw,
            BufferFrequency vertexBufferFrequency = BufferFrequency::Static) override {
        auto model = node->createComponent<Model>();
        auto material = node->createComponent<Material<vertex::PositionNormalUVVertex, unsigned int>>();

        std::vector<btVector3> positions;
        std::vector<btVector4> UVs;
        std::vector<btVector3> normals;
        std::vector<vertex::PositionNormalUVVertex> vertices;
        std::vector<unsigned int> indices;

        std::string line, descriptor;
        float x, y, z;

        std::regex faceRegex("(\\d+)(\\/(\\d+)?(\\/(\\d+))?)?");

        while (std::getline(stream, line)) {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream ss;
            ss.str(line);

            ss >> descriptor;

            if (descriptor.compare("v") == 0) {
                // Vertex: x,y,z
                ss >> x >> y >> z;
                positions.emplace_back(x, y, z);
            }
            else if (descriptor.compare("vt") == 0) {
                // Texture: u,v
                ss >> x >> y;
                UVs.emplace_back(x, y, 0.0f, 0.0f);
            }
            else if (descriptor.compare("vn") == 0) {
                // Normal: x,y,z
                ss >> x >> y >> z;
                normals.emplace_back(x, y, z);
            }
            else if (descriptor.compare("f") == 0) {
                auto f = [&]() {
                    auto current = vertices.size();
                    int faces = 0;
                    // Face: v1 v2 v3 | v1/t1 v2/t2 v3/t3 | v1/[t1]/n1 v2/[t2]/n2 v3/[t3]/n3
                    while (true) {
                        ss >> descriptor;
                        std::smatch m;
                        std::regex_match(descriptor, m, faceRegex);

                        if (m.empty()) {
                            break;
                        }
                        faces++;

                        btVector3 p, n;
                        btVector4 t;
                        int vi = std::stoi(m[1]);
                        int vn = 0;
                        int vt = 0;
                        if (m[3].matched) {
                            vt = std::stoi(m[3]);
                        }
                        if (m[5].matched) {
                            vn = std::stoi(m[5]);
                        }

                        if (vi > 0) {
                            p = positions[vi - 1]; 
                        }
                        else if (vi < 0) {
                            p = positions[positions.size() - vi];
                        }

                        if (vn > 0) {
                            n = normals[vn - 1];
                        }
                        else if (vn < 0) {
                            n = normals[normals.size() - vn];
                        }

                        if (vt > 0) {
                            t = UVs[vt - 1];
                        }
                        else if (vt < 0) {
                            t = UVs[UVs.size() - vt];
                        }

                        // This formula should work with everything
                        if (faces >= 3) {
                            indices.emplace_back(current);
                            indices.emplace_back(vertices.size() - 1);
                            indices.emplace_back(vertices.size());
                        }
                        vertices.emplace_back(p, n, t);
                    }

                    // Calculate normals if needed
                    if (vertices[current].normal.isZero()) {
                        for (auto max = vertices.size(), i = current + 1; i < max - 1; ++i) {
                            auto p = vertices[i].position - vertices[current].position;
                            auto n = vertices[i + 1].position - vertices[i].position;

                            vertices[i - 1].normal = vertices[i].normal = vertices[i + 1].normal = p.cross(n).normalized();
                        }
                    }
                };

                f();
            }
            // TODO: material -> set current vertices/indices to current material -> reset vectors
        }

        // Now we have vertices and indices populated, add them to the material
        material->beginSetup();

        auto ib = std::unique_ptr<IBType>(new IBType(BufferTarget::Index));
        ib->create(indices.data(), indices.size(), indexBufferFrequency, indexBufferAccess);
        auto vb = std::unique_ptr<VBType>(new VBType(BufferTarget::Vertex));
        vb->create(vertices.data(), vertices.size(), vertexBufferFrequency, vertexBufferAccess);

        material->endSetup();
    }
};

}
