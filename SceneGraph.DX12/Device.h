#pragma once

#include <array>
#include <memory>
#include <vector>

#include <d3d12.h>
#include <dxgi1_4.h>
#include <Windows.h>
#include <wrl.h>

namespace kitsune {
namespace scenegraph {
namespace dx12 {

class Device
{
public:
    Device();
    ~Device();

    bool createAdapter();

    bool isValid() const { return Valid; }

    ID3D12Device* getDevice() const { return D3DDevice.Get(); }

protected:
    std::vector<Microsoft::WRL::ComPtr<ID3D12CommandAllocator>> CommandAllocators;

    virtual std::unique_ptr<D3D12_COMMAND_QUEUE_DESC> makeCommandQueueDescriptor();
    virtual std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> makeRTVDescriptor();
    virtual std::unique_ptr<D3D12_DESCRIPTOR_HEAP_DESC> makeDSVDescriptor();
    virtual bool createCommandAllocators();

private:
    static constexpr int BufferCount = 3;

    int CurrentFrame;
    bool Valid;

    Microsoft::WRL::ComPtr<ID3D12Device> D3DDevice;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DSVHeap;
    Microsoft::WRL::ComPtr<ID3D12Fence> Fence;

    std::array<uint64_t, BufferCount> FenceValues;
    HANDLE FenceEvent;

    std::vector<IDXGIAdapter1*> findAvailableAdapters(IDXGIFactory4 *Factory);
    void createFences();
};

}
}
}