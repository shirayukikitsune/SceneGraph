#pragma once

#include <memory>

namespace kitsune {
namespace scenegraph {

	class Node
	{
	public:
		Node();
		~Node();

		bool isLocalActive() const { return Active; }
		bool isActive() const;

	protected:

	private:
		std::weak_ptr<Node> ParentNode;

		bool Active;
	};

}
}

