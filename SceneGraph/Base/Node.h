#pragma once

#include "Callback.h"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace kitsune::scenegraph {

    class Component;

    class Scene;

    class alignas(16) Node
            : public std::enable_shared_from_this<Node> {
    public:
        typedef auto_callback<void(void)> invalidatedByParentCallback;
        typedef auto_callback<void(Component *)> componentChangedCallback;

        explicit Node(std::weak_ptr<Scene> Scene = std::weak_ptr<kitsune::scenegraph::Scene>());

        bool isLocalActive() const { return Active; }

        bool isActive() const;

        void setActive(bool State) { Active = State; }

        void addComponent(std::size_t typehash, std::unique_ptr<Component> &&Component);

        void addComponent(std::size_t typehash, Component *Component);

        bool hasComponent(std::size_t typehash);

        Component *getComponent(std::size_t typehash);

        void removeComponent(Component *Component, bool Delete = true);

        template<class T, class... ArgTypes>
        T *createComponent(ArgTypes... args) {
            auto ptr = new T(args...);

            addComponent < T > (ptr);

            ptr->initialize();

            return ptr;
        }

        template<class T>
        void addComponent(std::unique_ptr<T> &&Component) {
            addComponent(T::componentNameHash, std::move(Component));
        }

        template<class T>
        void addComponent(T *Component) {
            addComponent(T::componentNameHash, std::forward<T *>(Component));
        }

        template<class T>
        bool hasComponent() {
            return hasComponent(T::componentNameHash);
        }

        template<class T>
        T *getComponent() {
            return (T *) getComponent(T::componentNameHash);
        }

        std::shared_ptr<Node> addChildNode();

        std::shared_ptr<Node> getParentNode();

        std::shared_ptr<Scene> getScene();

        glm::mat4 getWorldTransform();

        void setWorldTransform(const glm::mat4 &transform);

        glm::mat4 getLocalTransform() const;

        void setLocalTransform(const glm::mat4 &transform);

        glm::mat4 getTransformToOrigin();

        glm::vec3 getLocalOffset() const;

        void setLocalOffset(const glm::vec3 &offset);

        glm::quat getLocalRotation() const;

        void setLocalRotation(const glm::quat &rotation);

        glm::vec3 getLocalScale() const;

        void setLocalScale(const glm::vec3 &scale);

        void resetTransform();

        const std::string &getName() const { return Name; }

        void setName(const std::string &name) { Name = name; }

        void setName(std::string &&name) { Name = std::move(name); }

        const std::string &getTag() const { return Tag; }

        void setTag(const std::string &tag);

        void setTag(std::string &&tag);

        invalidatedByParentCallback::auto_remover_type
        addInvalidatedByParentEvent(invalidatedByParentCallback::function_type &&function);

        invalidatedByParentCallback::auto_remover_type
        addInvalidatedByParentEvent(const invalidatedByParentCallback::function_type &function);

        componentChangedCallback::auto_remover_type
        addComponentAddedEvent(componentChangedCallback::function_type &&function);

        componentChangedCallback::auto_remover_type
        addComponentAddedEvent(const componentChangedCallback::function_type &function);

        componentChangedCallback::auto_remover_type
        addComponentRemovedEvent(componentChangedCallback::function_type &&function);

        componentChangedCallback::auto_remover_type
        addComponentRemovedEvent(const componentChangedCallback::function_type &function);

        void render();

    protected:
        void invalidate();

    private:
        glm::quat LocalRotation;
        glm::vec3 LocalOffset;
        glm::vec3 LocalScale;
        glm::mat4 LocalTransform;

        glm::mat4 WorldTransform;
        bool RecalculateWorld;

        std::string Name;
        std::string Tag;

        std::weak_ptr<Node> ParentNode;
        std::weak_ptr<Scene> ParentScene;

        std::set<std::shared_ptr<Node>> ChildNodes;

        std::multimap<std::size_t, std::unique_ptr<Component>> Components;

        bool Active;

        invalidatedByParentCallback invalidatedByParentEvent;
        componentChangedCallback componentAddedEvent;
        componentChangedCallback componentRemovedEvent;
    };

}
