#pragma once

#include "Callback.h"
#include "Hash.h"
#include "Node.h"

#include <memory>

#define KIT_SG_COMPONENT(t) \
    public: \
        constexpr static std::uint32_t componentNameHash = #t ## _crc32; \
        virtual std::uint32_t getComponentNameHash() const override { return componentNameHash; }

namespace kitsune::scenegraph {

    class Scene;

    class Component {
    public:
        constexpr static std::uint32_t componentNameHash = "kitsune::scenegraph::Component"_crc32;

        virtual std::uint32_t getComponentNameHash() const { return componentNameHash; }

    public:
        typedef auto_callback<void(std::weak_ptr<Node>)> onNodeSetCallback;

        Component();

        virtual ~Component() = default;

        bool isLocalActive() const { return Active; }

        bool isActive() const;

        virtual void setActive(bool State) { Active = State; }

        virtual void initialize() {}

        virtual void render() {}

        void setNode(std::weak_ptr<Node> Node);

        std::shared_ptr<Node> getNode();

        std::shared_ptr<Scene> getScene();

        onNodeSetCallback::auto_remover_type addOnNodeSetEvent(onNodeSetCallback::function_type &&function);

        onNodeSetCallback::auto_remover_type addOnNodeSetEvent(const onNodeSetCallback::function_type &function);

    private:
        std::weak_ptr<Node> ParentNode;

        std::weak_ptr<Scene> ParentScene;

        bool Active;

        onNodeSetCallback onNodeSetEvent;

    protected:
        virtual void onNodeSet() {}
    };

}
