#pragma once

#include <memory>

namespace kitsune {
namespace scenegraph {

class Node;

namespace sdl {
namespace prefabs {

/**
 * This function will create a plane on the specified Node (preferrably empty!)
 * 
 * @param node The node to have plane geometry added to
 * @param addPhysics Whether to add a rigid body and collision shape
 */
void CreatePlane(std::shared_ptr<Node> node, bool addPhysics = true);

}
}
}
}
