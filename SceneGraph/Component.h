#pragma once

#include "Callback.h"
#include "Hash.h"
#include "Node.h"

#include <memory>

#define KIT_SG_COMPONENT_NAME(t) #t
#define KIT_SG_COMPONENT(t) public: constexpr static std::size_t componentNameHash = KIT_SG_COMPONENT_NAME(t)_hash;

namespace kitsune {
namespace scenegraph {

	class Scene;

	class Component
	{
		KIT_SG_COMPONENT(kitsune::scenegraph::Component);

	public:
		typedef auto_callback<void(std::weak_ptr<Node>)> onNodeSetCallback;

		Component(std::shared_ptr<Node> Node);
		virtual ~Component();

		bool isLocalActive() const { return Active; }
		bool isActive() const;
		void setActive(bool State) { Active = State; }

		virtual void initialize();

		void setNode(std::weak_ptr<Node> Node);
		std::shared_ptr<Node> getNode();

		std::shared_ptr<Scene> getScene();

		onNodeSetCallback::auto_remover_type addOnNodeSetEvent(onNodeSetCallback::function_type && function);
		onNodeSetCallback::auto_remover_type addOnNodeSetEvent(const onNodeSetCallback::function_type & function);

	private:
		std::weak_ptr<Node> Node;
		
		std::weak_ptr<Scene> Scene;

		bool Active;

		onNodeSetCallback onNodeSetEvent;

	protected:
		virtual void onNodeSet() {}
	};

}
}
