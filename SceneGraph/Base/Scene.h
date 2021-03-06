#pragma once

#include "Callback.h"
#include "Component.h"

#include <memory>
#include <map>
#include <string>

namespace kitsune::scenegraph {

    class Node;

    class Scene
            : public std::enable_shared_from_this<Scene> {
    public:
        ///! The type used for update callbacks
        typedef auto_callback<void(float)> updateCallback;

        Scene();

        virtual ~Scene();

        ///! Prepares the scene, creating all associated resources
        void initialize();

        ///! Handles the changes of the scene, propagating to all connected nodes
        void update(float DeltaTime);

        ///! Returns the root node of the scene
        Node *getRootNode() { return RootNode.get(); }

        std::shared_ptr<Node> getActiveCamera() { return ActiveCamera.lock(); }

        void setActiveCamera(const std::shared_ptr<Node> &Camera) { ActiveCamera = Camera; }

        ///! Renders the scene
        void render();

        updateCallback::auto_remover_type addPreUpdateEvent(updateCallback::function_type &&function);

        updateCallback::auto_remover_type addPreUpdateEvent(const updateCallback::function_type &function);

        updateCallback::auto_remover_type addUpdateEvent(updateCallback::function_type &&function);

        updateCallback::auto_remover_type addUpdateEvent(const updateCallback::function_type &function);

        void addTaggedNode(const std::string &Tag, std::shared_ptr<Node> Node);

        void removeTaggedNode(const std::string &Tag, std::shared_ptr<Node> Node);

        auto findNodesByTag(const std::string &Tag) {
            return this->TaggedNodes.equal_range(Tag);
        }

    protected:
        ///! Event called right when the scene is asked to initialize.
        ///
        /// \remarks The root node is not yet initialized
        virtual void onPreInit() {};

        ///! Event called when the scene is fully initialized
        virtual void onInit() {};

        ///! Event called before the scene update process take place
        virtual void onPreUpdate(float DeltaTime) {};

        ///! Event called after the scene update process take place
        virtual void onUpdate(float DeltaTime) {};

    private:
        updateCallback preUpdateEvents;

        updateCallback updateEvents;

        ///! The root node of the scene
        ///
        /// \remarks This pointer is guaranteed to be valid (i.e., RootNode != nullptr) while the instance of this scene remains valid
        std::shared_ptr<Node> RootNode;

        ///! A node that is set as current camera
        std::weak_ptr<Node> ActiveCamera;

        ///! Nodes with tags
        std::multimap<std::string, std::weak_ptr<Node>> TaggedNodes;
    };

}
