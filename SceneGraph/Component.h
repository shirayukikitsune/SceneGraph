#pragma once

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
		Component(std::shared_ptr<Node> Node);
		virtual ~Component();

		bool isLocalActive() const { return Active; }
		bool isActive() const;

	private:
		std::weak_ptr<Node> Node;
		
		std::weak_ptr<Scene> Scene;

		bool Active;
	};

}
}
