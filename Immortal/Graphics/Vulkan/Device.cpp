#include "Device.h"

#include "Common.h"
#include "PhysicalDevice.h"
#include "DescriptorPool.h"
#include "Shared/Async.h"

#include "Buffer.h"
#include "DescriptorSet.h"
#include "GPUEvent.h"
#include "Pipeline.h"
#include "Texture.h"
#include "Surface.h"
#include "Swapchain.h"
#include "Sampler.h"

#include "Shared/Async.h"

namespace Immortal
{
namespace Vulkan
{

Device::Device() :
    handle{},
    physicalDevice{},
    allocationCallbacks{},
    memoryAllocator{}
{

}

Device::Device(PhysicalDevice *physicalDevice, std::unordered_map<const char*, bool> requestedExtensions) :
    physicalDevice{ physicalDevice }
{
    VmaVulkanFunctions     vmaVulkanFunc{};
    VmaAllocatorCreateInfo allocatorInfo{};

    uint32_t propsCount = uint32_t(physicalDevice->QueueFamilyProperties.size());
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos(propsCount, { VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO });
    std::vector<std::vector<float>>      queueProps(propsCount);

    for (uint32_t index = 0; index < propsCount; index++)
    {
        const VkQueueFamilyProperties &prop = physicalDevice->QueueFamilyProperties[index];
        queueProps[index].resize(prop.queueCount, 0.5f);
        if (physicalDevice->HighPriorityGraphicsQueue && GetQueueFailyIndex(VK_QUEUE_GRAPHICS_BIT) == index)
        {
            queueProps[index][0] = 1.0f;
        }
        queueCreateInfos[index].pNext            = nullptr;
        queueCreateInfos[index].queueFamilyIndex = index;
        queueCreateInfos[index].queueCount       = prop.queueCount;
        queueCreateInfos[index].pQueuePriorities = queueProps[index].data();
    }
    std::vector<VkExtensionProperties> availableExtensions;
    Check(physicalDevice->EnumerateDeviceExtensionProperties(availableExtensions));

    for (auto &extension : availableExtensions)
    {
        deviceExtensions.insert(extension.extensionName);
    }

#if _DEBUG
    if (!deviceExtensions.empty())
    {
        LOG::DEBUG("Device supports the following extensions: ");
        for (auto &extension : availableExtensions)
        {
            LOG::DEBUG("  \t{}", extension.extensionName);
        }
    }
#endif

    if (IsExtensionSupported("VK_KHR_get_memory_requirements2") && IsExtensionSupported("VK_KHR_dedicated_allocation"))
    {
        enabledExtensions.emplace_back("VK_KHR_get_memory_requirements2");
        enabledExtensions.emplace_back("VK_KHR_dedicated_allocation");

        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;
        vmaVulkanFunc.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
        vmaVulkanFunc.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;

        LOG::DEBUG("Dedicated Allocation enabled");
    }

    for (auto &extension : requestedExtensions)
    {
        auto &extensionName = extension.first;
        bool isOptional = extension.second;
        if (IsExtensionSupported(extensionName))
        {
            enabledExtensions.emplace_back(extensionName);
            LOG::DEBUG("Enabled Device Extension: {}", extensionName);
        }
        else
        {
            if (isOptional)
            {
                LOG::WARN("Some features may be disabled for optional device extension not available. - `{}`", extensionName);
            }
            else
            {
                LOG::ERR("Stop running for required device extension not available - `{}`", extensionName);
                Check(VK_ERROR_EXTENSION_NOT_PRESENT);
            }
        }
    }

    if (IsEnabled("VK_KHR_performance_query") && IsEnabled("VK_EXT_host_query_reset"))
    {
        physicalDevice->RequestExtensionFeatures<VkPhysicalDevicePerformanceQueryFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PERFORMANCE_QUERY_FEATURES_KHR);
        physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceHostQueryResetFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_HOST_QUERY_RESET_FEATURES);
        LOG::DEBUG("Performance query enabled");
    }
    if (IsEnabled(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME))
    {
        physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceTimelineSemaphoreFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_TIMELINE_SEMAPHORE_FEATURES_KHR);
    }
    if (IsEnabled(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
    {
        physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceBufferDeviceAddressFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR);
    }
    if (IsEnabled(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME))
    {
        physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceAccelerationStructureFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR);
    }
    if (IsEnabled(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME))
    {
        physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceRayQueryFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR);
	}
	if (IsEnabled(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME))
    {
		physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceDynamicRenderingFeaturesKHR>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR);
	}
    if (IsEnabled(VK_EXT_SHADER_MODULE_IDENTIFIER_EXTENSION_NAME))
    {
		physicalDevice->RequestExtensionFeatures<VkPhysicalDeviceShaderModuleIdentifierFeaturesEXT>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_MODULE_IDENTIFIER_FEATURES_EXT);
		physicalDevice->RequestExtensionFeatures<VkPhysicalDevicePipelineCreationCacheControlFeatures>(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PIPELINE_CREATION_CACHE_CONTROL_FEATURES);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext                   = physicalDevice->LastRequestedExtensionFeature;
    createInfo.queueCreateInfoCount    = uint32_t(queueCreateInfos.size());
    createInfo.pQueueCreateInfos       = queueCreateInfos.data();
    createInfo.enabledExtensionCount   = uint32_t(enabledExtensions.size());
    createInfo.ppEnabledExtensionNames = enabledExtensions.data();
    createInfo.pEnabledFeatures        = &physicalDevice->RequestedFeatures;

    Check(physicalDevice->CreateDevice(&createInfo, &handle));

    queues.resize(propsCount);
    for (uint32_t queueFamilyIndex = 0U; queueFamilyIndex < propsCount; queueFamilyIndex++)
    {
        const auto &queueFamilyProps = physicalDevice->QueueFamilyProperties[queueFamilyIndex];
        VkBool32 presentSupported = VK_NULL_HANDLE ? physicalDevice->IsPresentSupported(VK_NULL_HANDLE, queueFamilyIndex) : false;

        uint32_t queueCount = queueFamilyProps.queueCount;
		queues[queueFamilyIndex].reserve(queueCount);
		for (uint32_t queueIndex = 0; queueIndex < queueCount; queueIndex++)
        {
            queues[queueFamilyIndex].emplace_back(this, queueFamilyIndex, queueFamilyProps, presentSupported, queueIndex);
        }
    }

    vmaVulkanFunc.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
    vmaVulkanFunc.vkGetDeviceProcAddr   = vkGetDeviceProcAddr;

    allocatorInfo.device               = handle;
    allocatorInfo.instance             = *physicalDevice->Get<Instance>();
    allocatorInfo.physicalDevice       = *physicalDevice;
    allocatorInfo.pVulkanFunctions     = &vmaVulkanFunc;
    allocatorInfo.pAllocationCallbacks = VK_NULL_HANDLE;

    if (IsEnabled(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME))
    {
        allocatorInfo.flags |= VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
    }

    Check(vmaCreateAllocator(&allocatorInfo, &memoryAllocator));

    semaphorePool = new SemaphorePool(this);

    descriptorPool = new DescriptorPool{ this, Limit::PoolSize };

    EnableGlobal();
}

uint32_t Device::GetQueueFailyIndex(VkQueueFlagBits requestFlags)
{
    const auto &queueFamilyProperties = physicalDevice->QueueFamilyProperties;

    VkQueueFlags mask = 0;

    if (requestFlags & VK_QUEUE_COMPUTE_BIT)
    {
        mask |= VK_QUEUE_GRAPHICS_BIT;
    }

    if (requestFlags & VK_QUEUE_TRANSFER_BIT)
    {
        mask |= VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
    }

    for (size_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        VkQueueFlags queueFlags = queueFamilyProperties[i].queueFlags;
        if (queueFlags & requestFlags && !(queueFlags & mask))
        {
            return uint32_t(i);
            break;
        }
    }

    return 0;
}

Device::~Device()
{
    Wait();

    commandPools.clear();
    descriptorPool->Destroy();
    semaphorePool.Reset();

    for (auto &queue : destroyCoroutine.queues)
    {
        while (!queue.empty())
        {
            std::function<void()> func = queue.front();
            queue.pop();
            func();
        }
    }

    descriptorPool.Reset();

    if (memoryAllocator != VK_NULL_HANDLE)
    {
        VmaTotalStatistics stats;
        vmaCalculateStatistics(memoryAllocator, &stats);
        LOG::INFO("Total device memory leaked: {} Bytes({} MB).",
            stats.total.statistics.allocationBytes,
            stats.total.statistics.allocationBytes / (1024.0f * 1024.0f)
        );
        vmaDestroyAllocator(memoryAllocator);
    }

    if (handle)
    {
        DestroyDevice(nullptr);
    }
}

Anonymous Device::GetBackendHandle() const
{
	return (void *)handle;
}

BackendAPI Device::GetBackendAPI()
{
	return BackendAPI::Vulkan;
}

SuperQueue *Device::CreateQueue(QueueType type, QueuePriority priority)
{
	VkQueueFlags flags = VkQueueFlags(type);

	for (uint32_t familyIndex = 0; familyIndex < queues.size(); familyIndex++)
	{
        for (auto &queue : queues[familyIndex])
        {
			VkQueueFlags queueFlags = queue.Properties().queueFlags;
			uint32_t queueCount     = queue.Properties().queueCount;
			if (((queueFlags & flags) == flags) && !queue.IsOccupied())
			{
				queue.Occupy();
				return new Queue{ &queue };
			}
        }
	}

    queues[0][0].Occupy();
    return new Queue{ &queues[0][0] };
}

SuperCommandBuffer *Device::CreateCommandBuffer(QueueType type)
{
	CommandPool *commandPool = nullptr;

    {
		auto threadId = Thread::Id();
		std::unique_lock lock{mutex};
		auto it = commandPools.find(threadId);
		if (it == commandPools.end())
		{
			commandPool = new CommandPool{ this, threadId };
			commandPools[threadId] = commandPool;
		}
        else
        {
			commandPool = it->second;
        }
    }

    return new CommandBuffer{ commandPool };
}

SuperSwapchain *Device::CreateSwapchain(SuperQueue *_queue, Window *window, Format format, uint32_t bufferCount, SwapchainMode mode)
{
    VkExtent2D extent = {
        .width  = window->GetWidth(),
        .height = window->GetHeight()
    };

    Surface surface{};
    Instance *instance = physicalDevice->Get<Instance>();
    Check(instance->CreateSurface(window, &surface, nullptr));

    Queue *queue = InterpretAs<Queue>(_queue);
    if (!physicalDevice->IsPresentSupported(surface, queue->GetFamilyIndex()))
    {
		LOG::ERR("Physical Device doesn't support WSI!");
    }

    return new Swapchain{ this, std::move(surface), format, extent, bufferCount, mode };
}

SuperSampler *Device::CreateSampler(Filter filter, AddressMode addressMode, CompareOperation compareOperation, float minLod, float maxLod)
{
    return new Sampler{ this, filter, addressMode, compareOperation, minLod, maxLod };
}

SuperShader *Device::CreateShader(const std::string &name, ShaderStage stage, const std::string &source, const std::string &entryPoint)
{
    return new Shader{ this, name, stage, source, entryPoint };
}

SuperGraphicsPipeline *Device::CreateGraphicsPipeline()
{
    return new GraphicsPipeline{ this };
}

SuperComputePipeline *Device::CreateComputePipeline(SuperShader *shader)
{
	return new ComputePipeline{ this, InterpretAs<Shader>(shader) };
}

SuperTexture *Device::CreateTexture(Format format, uint32_t width, uint32_t height, uint16_t mipLevels, uint16_t arrayLayers, TextureType type)
{
    return new Texture{ this, format, width, height, mipLevels, arrayLayers, type };
}

SuperBuffer *Device::CreateBuffer(size_t size, BufferType type)
{
    return new Buffer{ this, type, size };
}

SuperDescriptorSet *Device::CreateDescriptorSet(SuperPipeline *pipeline)
{
	return new DescriptorSet{ this, InterpretAs<Pipeline>(pipeline) };
}

SuperGPUEvent *Device::CreateGPUEvent(const std::string &name)
{
    return new GPUEvent{ this };
}

SuperRenderTarget *Device::CreateRenderTarget(uint32_t width, uint32_t height, const Format *pColorAttachmentFormats, uint32_t colorAttachmentCount, Format depthAttachmentFormat)
{
	return new RenderTarget{ this, width, height, pColorAttachmentFormats, colorAttachmentCount, depthAttachmentFormat };
}

uint32_t Device::GetMemoryType(uint32_t bits, VkMemoryPropertyFlags properties, VkBool32 *memoryTypeFound)
{
    for (uint32_t i = 0; i < physicalDevice->MemoryProperties.memoryTypeCount; i++)
    {
        if (bits & 1)
        {
            if ((physicalDevice->MemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                if (memoryTypeFound)
                {
                    *memoryTypeFound = true;
                }
                return i;
            }
        }
        bits >>= 1;
    }

    if (memoryTypeFound)
    {
        *memoryTypeFound = false;
        return 0;
    }
    else
    {
        SLASSERT(false && "Could not find a matching memory type");
        return -1;
    }
}

void Device::DestroyObjects()
{
    auto &queue = destroyCoroutine.queues[destroyCoroutine.freeing];
    auto &mutex = destroyCoroutine.mutex;

    SLROTATE(destroyCoroutine.working, destroyCoroutine.queues.size());
    SLROTATE(destroyCoroutine.freeing, destroyCoroutine.queues.size());

    while (!queue.empty())
    {
        std::function<void()> func;
        func = queue.front();
        queue.pop();
        func();
    }
}

VkResult Device::AllocateDescriptorSet(const VkDescriptorSetLayout *pDescriptorSetLayout, VkDescriptorSet *pDescriptorSets)
{
    return descriptorPool->Allocate(pDescriptorSetLayout, pDescriptorSets);
}

void Device::FreeDescriptorSet(VkDescriptorSet *pDescriptorSets, uint32_t size)
{
    descriptorPool->Free(pDescriptorSets, size);
}

}
}
