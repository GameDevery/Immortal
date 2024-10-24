#pragma once

#include "Common.h"

#include <queue>

namespace Immortal
{
namespace Vulkan
{

class Device;
class DescriptorSetLayout;

class DescriptorPool
{
public:
    static constexpr uint32_t MaxSetsPerPool = 1000;

    using Primitive = VkDescriptorPool;

public:
    DescriptorPool(Device *device, const DescriptorSetLayout &layout, uint32_t poolSize = MaxSetsPerPool);

    DescriptorPool(Device *device, const std::vector<VkDescriptorPoolSize> &poolSize);

    ~DescriptorPool();

    VkResult Allocate(const VkDescriptorSetLayout *pDescriptorSetLayout, VkDescriptorSet *pDescriptorSet, uint32_t count = 1);

    void Free(VkDescriptorSet *pDescriptorSet, uint32_t size = 1);

    void Reset();

    void Destroy();

    Primitive Handle() const
    {
        return handles.back();
    }

    operator Primitive() const
    {
        return Handle();
    }

private:
    VkDescriptorPool Create();

    VkResult AllocateInternal(const VkDescriptorSetLayout *pDescriptorSetLayout, VkDescriptorSet *pDescriptorSet, uint32_t count);

protected:
    Device *device{ nullptr };

    std::vector<VkDescriptorPool> handles;

    std::vector<VkDescriptorPoolSize> poolSize;

    std::queue<VkDescriptorSet> cache;

    uint32_t allocatedCount = 0;
};

}
}
