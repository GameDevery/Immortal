#include "Instance.h"

#define IMMORTAL_ENABLE_D3D11
#define IMMORTAL_ENABLE_D3D12
#define IMMORTAL_ENABLE_VULKAN
#define IMMORTAL_ENABLE_OPENGL
#ifdef __APPLE__
#define IMMORTAL_ENABLE_METAL
#endif

#ifdef _WIN32
#ifdef IMMORTAL_ENABLE_D3D12
#include "D3D12/Instance.h"
#endif
#ifdef IMMORTAL_ENABLE_D3D11
#include "D3D11/Instance.h"
#endif
#endif

#ifdef IMMORTAL_ENABLE_VULKAN
#include "Vulkan/Instance.h"
#endif
#ifdef IMMORTAL_ENABLE_OPENGL
#include "OpenGL/Instance.h"
#endif

#ifdef IMMORTAL_ENABLE_METAL
#include "Metal/Instance.h"
#endif

namespace Immortal
{

Instance *Instance::CreateInstance(BackendAPI apiType, Window::Type windowType)
{
	switch (apiType)
    {
#ifdef _WIN32
#ifdef IMMORTAL_ENABLE_D3D11
		case BackendAPI::D3D11:
			return new D3D11::Instance;
#endif
#ifdef IMMORTAL_ENABLE_D3D12
		case BackendAPI::D3D12:
			return new D3D12::Instance;
#endif
#endif
#ifdef IMMORTAL_ENABLE_VULKAN
		case BackendAPI::Vulkan:
        {
            std::unordered_map<const char *, bool> instanceExtensions{};

			switch (windowType)
            {
            case Window::Type::Win32:
                instanceExtensions.insert({ "VK_KHR_win32_surface", false });
                break;

            case Window::Type::Wayland:
                instanceExtensions.insert({ "VK_KHR_wayland_surface", false });
                break;

            case Window::Type::XCB:
                instanceExtensions.insert({ "VK_KHR_xcb_surface", false });
                break;

            case Window::Type::X11:
                instanceExtensions.insert({ "VK_KHR_xlib_surface", false });
                break;

            case Window::Type::Headless:
				instanceExtensions.insert({ "VK_EXT_headless_surface", false });
                break;

#if defined(__APPLE__)
            case Window::Type::Cocoa:
                instanceExtensions.insert({ "VK_EXT_metal_surface", false });
                instanceExtensions.insert({ "VK_KHR_portability_enumeration", false });
                break;
#endif
            default:
                break;
            }

			static std::vector<const char *> validationLayers = {
#	ifdef _DEBUG
			    "VK_LAYER_KHRONOS_validation",
			    "VK_LAYER_KHRONOS_synchronization2",
			    "VK_LAYER_LIGHTWSI_Light",
#	endif
			};

			return new Vulkan::Instance{ "ImmortalGraphics", instanceExtensions, validationLayers };
        }
#endif
#ifdef IMMORTAL_ENABLE_OPENGL
		case BackendAPI::OpenGL:
			return new OpenGL::Instance;
#endif
#ifdef IMMORTAL_ENABLE_METAL
        case BackendAPI::Metal:
            return new Metal::Instance;
#endif
        default:
            throw std::runtime_error{ "Not support api" };
            return nullptr;
    }
}

}
