#include "ModelLoader.h"
#include "Vertex.h"

#include <fstream>
#include <regex>
#include <sstream>

namespace kitsune {
namespace scenegraph {
namespace sdl {
namespace graphics {
namespace implementations {

class OBJModelLoader : public ModelLoader {
public:
    void load(std::shared_ptr<Node> node, const std::string& filename) override {
        std::ifstream filestream(filename, std::ios::in);

        load(node, filestream);

        filestream.close();
    }

    void load(std::shared_ptr<Node> node, std::istream& stream) override {
        auto model = node->createComponent<Model>();

        std::vector<btVector3> positions;
        std::vector<btVector4> UVs;
        std::vector<btVector3> normals;
        std::vector<vertex::PositionNormalUVVertex> vertices;

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
                    // Face: v1 v2 v3 | v1/t1 v2/t2 v3/t3 | v1/[t1]/n1 v2/[t2]/n2 v3/[t3]/n3
                    for (int i = 0; i < 3; ++i) {
                        ss >> descriptor;
                        std::smatch m;
                        std::regex_match(descriptor, m, faceRegex);

                        if (m.empty()) {
                            return;
                        }

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

                        vertices.emplace_back(p, n, t);
                    }

                    // Calculate normal if needed
                    if (vertices[current].normal.isZero()) {
                        auto p = vertices[current + 1].position - vertices[current].position;
                        auto n = vertices[current + 2].position - vertices[current + 1].position;

                        vertices[current].normal = vertices[current + 1].normal = vertices[current + 2].normal = p.cross(n).normalized();
                    }
                };

                f();
            }
        }
    
        
    }
};

}
}
}
}
}
