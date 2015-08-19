#pragma once

#include <memory>

namespace kitsune {
namespace scenegraph {

	class Node;

	class Scene
	{
	public:
		Scene();
		~Scene();

		/**
		 * @brief Prepares the scene, creating all associated resources
		 */
		void initialize();

		/**
		 * @brief Handles the changes of the scene, propagating to all connected nodes
		 */
		void update(float DeltaTime);

	protected:
		/**
		 * @brief Event called right when the scene is asked to initialize.
		 *
		 * @remarks The root node is not yet initialized
		 */
		virtual void onPreInit() {};

		/**
		 * @brief Event called when the scene is fully initialized
		 */
		virtual void onInit() {};

	private:
		/**
		 * @brief The root node of the scene
		 *
		 * This pointer is guaranteed to be valid (i.e., RootNode != nullptr) while the instance of this scene remains valid
		 */
		std::unique_ptr<Node> RootNode;
	};

}
}
