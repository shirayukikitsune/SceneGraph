#pragma once

#include <vector>
#include <Windows.h>

#include "Device.h"

namespace kitsune {
namespace scenegraph {
namespace dx12 {

	class Bootstrap
	{
	public:
		virtual void setWindowClass(WNDCLASSEX &WndClass) {}
		virtual void onInitializing() = 0;
		virtual void onInitialized() = 0;
		virtual void onTerminating() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;

        virtual Device* createDevice() { return new Device; }
	};

	extern kitsune::scenegraph::dx12::Bootstrap * AppBootstrap;

}
}
}
