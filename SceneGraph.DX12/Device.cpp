#include "Device.h"
#include "Util.h"

#include <wrl.h>

using kitsune::scenegraph::dx12::Device;
using Microsoft::WRL::ComPtr;

Device::Device()
{
    Valid = false;
}

Device::~Device()
{
}

bool Device::createAdapter()
{
#ifdef _DEBUG
    {
        ComPtr<ID3D12Debug> DebugInterface;
        HRESULT Result = D3D12GetDebugInterface(IID_PPV_ARGS(&DebugInterface));
        if (SUCCEEDED(Result)) {
            DebugInterface->EnableDebugLayer();
        }
    }
#endif

    ComPtr<IDXGIFactory4> Factory;
    ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)));

    auto Adapters = findAvailableAdapters(Factory.Get());
    if (Adapters.empty()) { return false; }

    // Create the device on the first available adapter
    for (auto &Adapter = Adapters.begin() + 1; Adapter != Adapters.end(); ++Adapter) {
        HRESULT Result = D3D12CreateDevice(*Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3DDevice));
        if (FAILED(Result)) {
            D3DDevice.Reset();
        }
        else {
            Adapters.erase(Adapter);
        }
    }

    if (!D3DDevice) { return false; }

    // Release unused adapters
    for (auto &Unused = Adapters.begin(); Unused != Adapters.end(); ++Unused) {
        (*Unused)->Release();
    }

    // Make scopes so the pointers will be freed
    {
        auto QueueDesc = makeCommandQueueDescriptor();
        ThrowIfFailed(D3DDevice->CreateCommandQueue(QueueDesc.get(), IID_PPV_ARGS(&CommandQueue)));
    }

    {
        auto RTVDesc = makeRTVDescriptor();
        ThrowIfFailed(D3DDevice->CreateDescriptorHeap(RTVDesc.get(), IID_PPV_ARGS(&RTVHeap)));
    }

    {
        auto DSVDesc = makeDSVDescriptor();
        ThrowIfFailed(D3DDevice->CreateDescriptorHeap(DSVDesc.get(), IID_PPV_ARGS(&DSVHeap)));
    }

    if (!createCommandAllocators()) { return false; }

    createFences();

    return true;
}

std::unique_ptr<D3D12_COMMAND_QUEUE_DESC> Device::makeCommandQueueDescriptor()
{
    std::unique_ptr<D3D12_COMMAND_QUEUE_DESC> QueueDesc(new D3D12_COMMAND_QUEUE_DESC);
    QueueDesc->Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    QueueDesc->Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    QueueDesc->NodeMask = 0;
    QueueDesc->Priority = 0;

    return QueueDesc;
}

std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> Device::makeRTVDescriptor()
{
    std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> RTVDesc(new D3D12_DESCRIPTOR_HEAP_DESC);
    RTVDesc->Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    RTVDesc->Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    RTVDesc->NodeMask = 0;
    RTVDesc->NumDescriptors = BufferCount;

    return RTVDesc;
}

std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> Device::makeDSVDescriptor()
{
    std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> DSVDesc(new D3D12_DESCRIPTOR_HEAP_DESC);
    DSVDesc->Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    DSVDesc->Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DSVDesc->NodeMask = 0;
    DSVDesc->NumDescriptors = 1;

    return DSVDesc;
}

bool Device::createCommandAllocators()
{
    for (auto i = 0; i < BufferCount; ++i) {
        if (!D3DDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocators[i]))) {
            return false;
        }
    }

    return true;
}

std::vector<IDXGIAdapter1*> Device::findAvailableAdapters(IDXGIFactory4 * Factory)
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

    return Output;
}

void Device::createFences()
{
    ThrowIfFailed(D3DDevice->CreateFence(FenceValues[CurrentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
    FenceValues[CurrentFrame]++;

    FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (FenceEvent == nullptr)
    {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}
