#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <vector>
#include <Windows.h>
#include <wrl.h>

namespace kitsune {
namespace scenegraph {
namespace dx12 {
	
	class Application;

	class Bootstrap
	{
	public:
		virtual void setWindowClass(WNDCLASSEX &WndClass) {}
		virtual void onInitializing(Application * Application) = 0;
		virtual void onInitialized() = 0;
		virtual void onTerminating() = 0;
		virtual void onUpdate() = 0;
		virtual void onRender() = 0;
		virtual void createAdapter();

	protected:
		ComPtr<ID3D12Device> Device;

		std::vector<IDXGIAdapter1*> && findAvailableDevices(IDXGIFactory4 *Factory);
	};

	extern kitsune::scenegraph::dx12::Bootstrap * AppBootstrap;

}
}
}
