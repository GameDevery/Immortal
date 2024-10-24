#include "Instance.h"

namespace Immortal
{
namespace D3D
{

Instance::Instance() :
    handle{},
    dxgiLibrary{},
    CreateDXGIFactory2{}
{
	dxgiLibrary = LoadLibraryA("dxgi.dll");
	if (!dxgiLibrary)
	{
		LOG::ERR("Failed to load dxgi.dll");
	}

	uint32_t dxgiFactoryFlags = 0;

#if _DEBUG
	dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

	CreateDXGIFactory2 = (PFN_CreateDXGIFactory2)GetProcAddress(dxgiLibrary, "CreateDXGIFactory2");
	Check(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&handle)), "Failed to create DXGI Factory");
}

Instance::~Instance()
{
	physicalDevices.clear();
	handle.Reset();
	if (dxgiLibrary)
	{
		FreeLibrary(dxgiLibrary);
		dxgiLibrary = nullptr;
	}

#ifdef _DEBUG
	HMODULE dxgidebug = LoadLibraryA("dxgidebug.dll");
	if (dxgidebug)
	{
		ComPtr<IDXGIDebug> dxgiDebug;
		auto __DXGIGetDebugInterface = (decltype(&DXGIGetDebugInterface)) ::GetProcAddress(dxgidebug, "DXGIGetDebugInterface");
		Check(__DXGIGetDebugInterface(IID_PPV_ARGS(&dxgiDebug)));
		Check(dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL));

		FreeLibrary(dxgidebug);
	}
#endif
}

void Instance::EnumerateAdapter(uint32_t deviceId, IDXGIAdapter1 **ppAdapter)
{
	Check(handle->EnumAdapters1(deviceId, ppAdapter));
}

}
}
