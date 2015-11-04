#include "Bootstrap.h"

#include <wrl.h>

using kitsune::scenegraph::dx12::Bootstrap;
using Microsoft::WRL::ComPtr;

void Bootstrap::createAdapter()
{
#ifdef _DEBUG
	{
		ComPtr<ID3D12Debug> DebugInterface;

		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugInterface)))) {
			DebugInterface->EnableDebugLayer();
		}
	}
#endif

	ComPtr<IDXGIFactory4> Factory;
	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)))) { return; }

	
}

std::vector<IDXGIAdapter1*> && Bootstrap::findAvailableDevices(IDXGIFactory4 * Factory)
{
	std::vector<IDXGIAdapter1*> Output;
	IDXGIAdapter1 *Adapter = nullptr;

	for (UINT i = 0; Factory->EnumAdapters1(i, &Adapter) != DXGI_ERROR_NOT_FOUND; ++i) {
		DXGI_ADAPTER_DESC1 Desc;
		Adapter->GetDesc1(&Desc);

		// Skip software adapter
		if (Desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			continue;

		if (SUCCEEDED(D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) {
			Output.push_back(Adapter);
		}
	}

	return std::move(Output);
}
