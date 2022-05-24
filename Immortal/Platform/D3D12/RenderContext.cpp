#include "RenderContext.h"

#include "Framework/Utils.h"
#include "Descriptor.h"
#include "GuiLayer.h"

namespace Immortal
{
namespace D3D12
{

Device *RenderContext::UnlimitedDevice = nullptr;

DescriptorAllocator RenderContext::shaderVisibleDescriptorAllocator{
    DescriptorPool::Type::ShaderResourceView,
    DescriptorPool::Flag::ShaderVisible
};

DescriptorAllocator RenderContext::descriptorAllocators[U32(DescriptorPool::Type::Quantity)] = {
    DescriptorPool::Type::ShaderResourceView,
    DescriptorPool::Type::Sampler,
    DescriptorPool::Type::RenderTargetView,
    DescriptorPool::Type::DepthStencilView
};

RenderContext::RenderContext(const Description &descrition) :
    desc{ descrition }
{
    Setup();
}

RenderContext::RenderContext(const void *handle)
{
    Setup();
}

RenderContext::~RenderContext()
{

}

void RenderContext::Setup()
{
    desc.FrameCount = Swapchain::SWAP_CHAIN_BUFFER_COUNT;

    uint32_t dxgiFactoryFlags = 0;

#if SLDEBUG
    ComPtr<ID3D12Debug> debugController;

    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();

        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        LOG::INFO("Enable Debug Layer: {0}", rcast<void*>(debugController.Get()));
    }
#endif
    Check(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory)), "Failed to create DXGI Factory");
    device = std::make_unique<Device>(dxgiFactory);

    UnlimitedDevice = device.get();

    for (size_t i = 0; i < SL_ARRAY_LENGTH(descriptorAllocators); i++)
    {
        descriptorAllocators[i].Init(device.get());
    }
    shaderVisibleDescriptorAllocator.Init(device.get());

    auto adapterDesc = device->GetAdapterDesc();
    Super::UpdateMeta(
        Utils::ws2s(adapterDesc.Description).c_str(),
        nullptr,
        nullptr
        );

    auto hWnd = rcast<HWND>(desc.WindowHandle->Primitive());

    {
        Queue::Description queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

        queue.reset(new Queue{ device.get(), queueDesc});
    }

    {
        Swapchain::Description swapchainDesc{};
        CleanUpObject(&swapchainDesc);
        swapchainDesc.BufferCount       = desc.FrameCount;
        swapchainDesc.Width             = desc.Width;
        swapchainDesc.Height            = desc.Height;
        swapchainDesc.Format            = desc.format;
        swapchainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapchainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapchainDesc.SampleDesc.Count  = 1;
        swapchainDesc.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        // swapchainDesc.Flags          = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

        swapchain = std::make_unique<Swapchain>(device.get(), queue->Handle(), hWnd, swapchainDesc);
        // swapchain->SetMaximumFrameLatency(desc.FrameCount);
        // swapchainWritableObject = swapchain->FrameLatencyWaitableObject();
    }

    Check(dxgiFactory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));

    {
        CheckDisplayHDRSupport();
        color.enableST2084 = color.hdrSupport;

        EnsureSwapChainColorSpace(color.bitDepth, color.enableST2084);
        /* SetHDRMetaData(
            HDRMetaDataPool[color.hdrMetaDataPoolIdx][0],
            HDRMetaDataPool[color.hdrMetaDataPoolIdx][1],
            HDRMetaDataPool[color.hdrMetaDataPoolIdx][2],
            HDRMetaDataPool[color.hdrMetaDataPoolIdx][3]
        ); */
    }

    {
        frameIndex = swapchain->AcquireCurrentBackBufferIndex();

        for (int i = 0; i < desc.FrameCount; i++)
        {
            commandAllocator[i] = queue->RequestCommandAllocator();
        }

        commandList = std::make_unique<CommandList>(
            device.get(),
            CommandList::Type::Direct,
            commandAllocator[frameIndex]
            );

        commandList->Close();

        queue->Execute(commandList->AddressOf<ID3D12CommandList>());

        fence.reset(new Fence{ device.get(), fenceValues[frameIndex] });

        fenceValues[frameIndex]++;
        WaitForGPU();
    }

#ifdef SLDEBUG
    device->Set("RenderContext::Device");
#endif
}

inline int ComputeIntersectionArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2)
{
    // (ax1, ay1) = left-top coordinates of A; (ax2, ay2) = right-bottom coordinates of A
    // (bx1, by1) = left-top coordinates of B; (bx2, by2) = right-bottom coordinates of B
    return std::max(0, std::min(ax2, bx2) - std::max(ax1, bx1)) * std::max(0, std::min(ay2, by2) - std::max(ay1, by1));
}

void RenderContext::EnsureSwapChainColorSpace(Swapchain::BitDepth bitDepth, bool enableST2084)
{
    DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

    switch (bitDepth)
    {
    case Swapchain::BitDepth::_8:
        color.rootConstants[DisplayCurve] = sRGB;
        break;

    case Swapchain::BitDepth::_10:
        colorSpace = enableST2084 ? DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 : DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
        color.rootConstants[DisplayCurve] = enableST2084 ? ST2084 : sRGB;
        break;

    case Swapchain::BitDepth::_16:
        colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
        color.rootConstants[DisplayCurve] = None;
        break;

    default:
        break;
    }

    if (color.currentColorSpace != colorSpace)
    {
        UINT colorSpaceSupport = 0;
        if (swapchain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport) &&
            ((colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) == DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
        {
            swapchain->Set(colorSpace);
            color.currentColorSpace = colorSpace;
        }
    }
}

void RenderContext::CheckDisplayHDRSupport()
{
    if (dxgiFactory->IsCurrent() == false)
    {
        Check(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory)));
    }

    ComPtr<IDXGIAdapter1> dxgiAdapter;
    Check(dxgiFactory->EnumAdapters1(0, &dxgiAdapter));

    UINT i = 0;
    ComPtr<IDXGIOutput> currentOutput;
    ComPtr<IDXGIOutput> bestOutput;
    float bestIntersectArea = -1;

    while (dxgiAdapter->EnumOutputs(i, &currentOutput) != DXGI_ERROR_NOT_FOUND)
    {
        int ax1 = windowBounds.left;
        int ay1 = windowBounds.top;
        int ax2 = windowBounds.right;
        int ay2 = windowBounds.bottom;

        DXGI_OUTPUT_DESC desc{};
        Check(currentOutput->GetDesc(&desc));
        RECT rect = desc.DesktopCoordinates;
        int bx1 = rect.left;
        int by1 = rect.top;
        int bx2 = rect.right;
        int by2 = rect.bottom;

        int intersectArea = ComputeIntersectionArea(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
        if (intersectArea > bestIntersectArea)
        {
            bestOutput = currentOutput;
            bestIntersectArea = ncast<float>(intersectArea);
        }
        i++;
    }

    ComPtr<IDXGIOutput6> output6;
    Check(bestOutput.As(&output6));

    DXGI_OUTPUT_DESC1 desc1;
    Check(output6->GetDesc1(&desc1));

    color.hdrSupport = (desc1.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
}

void RenderContext::SetHDRMetaData(float maxOutputNits, float minOutputNits, float maxCLL, float maxFALL)
{
    if (!swapchain)
    {
        return;
    }

    if (!color.hdrSupport)
    {
        swapchain->Set(DXGI_HDR_METADATA_TYPE_NONE, 0, nullptr);
        return;
    }

    static const DisplayChromaticities displayChromaticityList[] = {
        { 0.64000f, 0.33000f, 0.30000f, 0.60000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut Rec709
        { 0.70800f, 0.29200f, 0.17000f, 0.79700f, 0.13100f, 0.04600f, 0.31270f, 0.32900f }, // Display Gamut Rec2020
    };

    int selectedChroma{ 0 };
    if (color.bitDepth == Swapchain::BitDepth::_16 && color.currentColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709)
    {
        selectedChroma = 0;
    }
    else if (color.bitDepth == Swapchain::BitDepth::_10 && color.currentColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
    {
        selectedChroma = 1;
    }
    else
    {
        swapchain->Set(DXGI_HDR_METADATA_TYPE_NONE, 0, nullptr);
    }

    const DisplayChromaticities &chroma = displayChromaticityList[selectedChroma];

    DXGI_HDR_METADATA_HDR10 metaData{};
    metaData.RedPrimary[0]             = ncast<UINT16>(chroma.RedX   * 50000.0f);
    metaData.RedPrimary[0]             = ncast<UINT16>(chroma.RedX   * 50000.0f);
    metaData.RedPrimary[1]             = ncast<UINT16>(chroma.RedY   * 50000.0f);
    metaData.GreenPrimary[0]           = ncast<UINT16>(chroma.GreenX * 50000.0f);
    metaData.GreenPrimary[1]           = ncast<UINT16>(chroma.GreenY * 50000.0f);
    metaData.BluePrimary[0]            = ncast<UINT16>(chroma.BlueX  * 50000.0f);
    metaData.BluePrimary[1]            = ncast<UINT16>(chroma.BlueY  * 50000.0f);
    metaData.WhitePoint[0]             = ncast<UINT16>(chroma.WhiteX * 50000.0f);
    metaData.WhitePoint[1]             = ncast<UINT16>(chroma.WhiteY * 50000.0f);

    metaData.MaxMasteringLuminance     = ncast<UINT>(maxOutputNits * 10000.0f);
    metaData.MinMasteringLuminance     = ncast<UINT>(minOutputNits * 10000.0f);

    metaData.MaxContentLightLevel      = ncast<UINT16>(maxCLL);
    metaData.MaxFrameAverageLightLevel = ncast<UINT16>(maxFALL);

    swapchain->Set(DXGI_HDR_METADATA_TYPE_HDR10, sizeof(metaData), &metaData);
}

void RenderContext::WaitForGPU()
{
    Check(queue->Signal(*fence, fenceValues[frameIndex]));
    Check(fence->SetCompletion(fenceValues[frameIndex]));
    fence->Wait();
    fenceValues[frameIndex]++;
}

UINT RenderContext::WaitForPreviousFrame()
{
    const uint64_t currentFenceValue = fenceValues[frameIndex];
    Check(queue->Signal(*fence, currentFenceValue));

    frameIndex = swapchain->AcquireCurrentBackBufferIndex();
    auto completedValue = fence->GetCompletion();
    if (completedValue < fenceValues[frameIndex])
    {
        Check(fence->SetCompletion(fenceValues[frameIndex]));
        fence->Wait();
    }

    fenceValues[frameIndex] = currentFenceValue + 1;

    return frameIndex;
}

void RenderContext::UpdateSwapchain(UINT width, UINT height)
{
    if (!swapchain)
    {
        return;
    }

    DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
    swapchain->GetDesc(&swapchainDesc);

    if (desc.Width != width || desc.Height != height)
    {
        WaitForGPU();
        swapchain->ClearRenderTarget();

        swapchain->ResizeBuffers(
            width,
            height,
            DXGI_FORMAT_UNKNOWN,
            swapchainDesc.Flags,
            desc.FrameCount
        );

        EnsureSwapChainColorSpace(color.bitDepth, color.enableST2084);
        swapchain->CreateRenderTarget();
    }
}

void RenderContext::WaitForNextFrameResources()
{
    auto frameIndex = swapchain->AcquireCurrentBackBufferIndex();

    HANDLE waitableObjects[] = {
        swapchainWritableObject,
        NULL
    };
    DWORD numWaitableObjects = 1;

    uint64_t fenceValue = fenceValues[frameIndex];
    if (fenceValue != 0)
    {
        fenceValues[frameIndex] = 0;
        fence->SetCompletion(fenceValue);
        waitableObjects[1] = fence->GetEvent();;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
}

void RenderContext::CopyDescriptorHeapToShaderVisible()
{
    auto srcDescriptorAllocator = descriptorAllocators[U32(DescriptorPool::Type::ShaderResourceView)];

    device->CopyDescriptors(
        srcDescriptorAllocator.CountOfDescriptor(),
        shaderVisibleDescriptorAllocator.FreeStartOfHeap(),
        srcDescriptorAllocator.StartOfHeap(),
        D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );
}

GuiLayer::Super *RenderContext::CreateGuiLayer()
{
    return new GuiLayer{ this };
}

}
}
