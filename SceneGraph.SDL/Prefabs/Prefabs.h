#pragma once

#include <memory>

namespace kitsune::scenegraph {

    class Node;

    namespace sdl::prefabs {

        /**
         * This function will create a plane on the specified Node (preferrably empty!)
         *
         * @param node The node to have plane geometry added to
         * @param addPhysics Whether to add a rigid body and collision shape
         */
        void CreatePlane(std::shared_ptr<Node> node, bool addPhysics = true);

        /**
         * This function will create a cube on the specified Node, having each edge with unitary length
         */
        void CreateCube(std::shared_ptr<Node> node, bool addPhysics = true);

    }

}
