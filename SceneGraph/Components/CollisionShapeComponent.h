#pragma once

#include "../Base/Component.h"

namespace kitsune {
namespace scenegraph {

	class CollisionShapeComponent :
		public Component
	{
		KIT_SG_COMPONENT(kitsune::scenegraph::CollisionShapeComponent);

	public:
		CollisionShapeComponent();
		virtual ~CollisionShapeComponent();
	};

}
}

