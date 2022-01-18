#include "Pipeline.h"
#include "Device.h"
#include "RenderTarget.h"
#include "Texture.h"
#include "RenderContext.h"

namespace Immortal
{
namespace Vulkan
{

GraphicsPipeline::GraphicsPipeline(Device *device, std::shared_ptr<Shader::Super> &shader) :
    device{ device },
    Super{ shader }
{
    configuration = std::make_unique<Configuration>();
    CleanUpObject(configuration.get());
}

GraphicsPipeline::~GraphicsPipeline()
{
    if (device)
    {
        device->Destroy(handle);
    }
}

void GraphicsPipeline::Set(std::shared_ptr<Buffer::Super> &buffer)
{
    Super::Set(buffer);
}

void GraphicsPipeline::Set(const InputElementDescription &description)
{
    Super::Set(description);
    auto size                       = desc.layout.Size();
    auto &inputAttributeDescription = configuration->inputAttributeDescriptions;

    inputAttributeDescription.resize(size);
    for (int i = 0; i < size; i++)
    {
        inputAttributeDescription[i].binding  = 0;
        inputAttributeDescription[i].location = i;
        inputAttributeDescription[i].format   = desc.layout[i].BaseType<VkFormat>();
        inputAttributeDescription[i].offset   = desc.layout[i].Offset();
    }

    configuration->vertexInputBidings.emplace_back(VkVertexInputBindingDescription{
        0,
        desc.layout.Stride(),
        VK_VERTEX_INPUT_RATE_VERTEX
        });

    SetupVertex();
    SetupLayout();
}

void GraphicsPipeline::Create(const std::shared_ptr<RenderTarget::Super> &superTarget)
{
    Reconstruct(superTarget);

    descriptorPool.reset(new DescriptorPool{ device, Limit::PoolSize  });

    auto shader = std::dynamic_pointer_cast<Shader>(desc.shader);
    descriptorSetUpdater = shader->GetAddress<DescriptorSetUpdater>();

    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

void GraphicsPipeline::Reconstruct(const std::shared_ptr<SuperRenderTarget> &superTarget)
{
    auto target = std::dynamic_pointer_cast<RenderTarget>(superTarget);

    auto state = &configuration->state;
    auto attachment = &configuration->attament;

    state->rasterization.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    state->rasterization.polygonMode             = VK_POLYGON_MODE_FILL;
    state->rasterization.cullMode                = VK_CULL_MODE_NONE;
    state->rasterization.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    state->rasterization.depthClampEnable        = VK_FALSE;
    state->rasterization.rasterizerDiscardEnable = VK_FALSE;
    state->rasterization.depthBiasEnable         = VK_FALSE;
    state->rasterization.lineWidth               = 1.0f;

    std::vector<VkPipelineColorBlendAttachmentState> colorBlends;
    colorBlends.resize(target->ColorAttachmentCount());
    for (auto &colorBlend : colorBlends)
    {
        colorBlend.colorWriteMask = 0xf;
        colorBlend.blendEnable    = VK_FALSE;
    }
    state->colorBlend.sType               = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    state->colorBlend.attachmentCount     = U32(colorBlends.size());
    state->colorBlend.pAttachments        = colorBlends.data();
    state->colorBlend.blendConstants[0]   = 1.0f;
    state->colorBlend.blendConstants[1]   = 1.0f;
    state->colorBlend.blendConstants[2]   = 1.0f;
    state->colorBlend.blendConstants[3]   = 1.0f;

    state->depthStencil.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    state->depthStencil.depthTestEnable       = VK_TRUE;
    state->depthStencil.depthWriteEnable      = VK_TRUE;
    state->depthStencil.depthCompareOp        = VK_COMPARE_OP_LESS_OR_EQUAL;
    state->depthStencil.depthBoundsTestEnable = VK_FALSE;
    state->depthStencil.back.failOp           = VK_STENCIL_OP_KEEP;
    state->depthStencil.back.passOp           = VK_STENCIL_OP_KEEP;
    state->depthStencil.back.compareOp        = VK_COMPARE_OP_ALWAYS;
    state->depthStencil.stencilTestEnable     = VK_FALSE;
    state->depthStencil.front                 = state->depthStencil.back;

    state->viewport.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    state->viewport.viewportCount = 1;
    state->viewport.scissorCount  = 1;
    state->viewport.flags         = 0;

    state->multiSample.sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    state->multiSample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    state->multiSample.flags                = 0;

    std::array<VkDynamicState, 2> dynamic{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };
    state->dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    state->dynamic.dynamicStateCount = dynamic.size();
    state->dynamic.pDynamicStates = dynamic.data();

    auto shader = std::dynamic_pointer_cast<Shader>(desc.shader);
    descriptorSetLayout = shader->Get<VkDescriptorSetLayout>();

    VkGraphicsPipelineCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.renderPass = target->GetRenderPass();
    createInfo.flags = 0;
    createInfo.layout = shader->Get<PipelineLayout&>();
    createInfo.pInputAssemblyState = &state->inputAssembly;
    createInfo.pVertexInputState = &state->vertexInput;
    createInfo.pRasterizationState = &state->rasterization;
    createInfo.pDepthStencilState = &state->depthStencil;
    createInfo.pViewportState = &state->viewport;
    createInfo.pMultisampleState = &state->multiSample;
    createInfo.pDynamicState = &state->dynamic;
    createInfo.pColorBlendState = &state->colorBlend;

    auto &stages = shader->Stages();
    createInfo.pStages = stages.data();
    createInfo.stageCount = stages.size();

    Check(device->CreatePipelines(cache, 1, &createInfo, nullptr, &handle));
}

bool GraphicsPipeline::Ready()
{
    bool ready = descriptorSetUpdater->Ready();
    if (ready)
    {
        Check(descriptorPool->Allocate(&descriptorSetLayout, &descriptorSet));
        descriptorSetUpdater->Set(descriptorSet);
    }
    return ready;
}

void GraphicsPipeline::Bind(Texture::Super *superTexture, uint32_t slot)
{
    auto texture = dynamic_cast<Texture *>(superTexture);
    auto descriptor = rcast<const Descriptor *>(&texture->DescriptorInfo());
    
    for (auto &writeDesc : descriptorSetUpdater->WriteDescriptorSets)
    {
        if (writeDesc.dstBinding == slot &&
            Equals(writeDesc.pImageInfo, descriptor))
        {
            return;
        }
    }

    Bind(descriptor, slot);
}

void GraphicsPipeline::Bind(const std::string &name, const Buffer::Super *uniform)
{
    auto descriptor = rcast<const BufferDescriptor *>(uniform->Descriptor());
    descriptorSetUpdater->Set(name, descriptor);
    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

void GraphicsPipeline::Bind(const Descriptor *descriptors, uint32_t slot)
{
    for (auto &writeDescriptor : descriptorSetUpdater->WriteDescriptorSets)
    {
        if (writeDescriptor.descriptorType <= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE &&
            writeDescriptor.dstBinding == slot)
        {
            writeDescriptor.pImageInfo = rcast<const VkDescriptorImageInfo *>(descriptors);
            break;
        }
    }
    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

ComputePipeline::ComputePipeline(Device *device, Shader::Super *superShader) :
    device{ device },
    descriptorPool{ new DescriptorPool{ device, Limit::PoolSize} }
{
    auto shader = dynamic_cast<Shader *>(superShader);
    auto &stage = shader->Stages();

    layout = shader->Get<PipelineLayout&>();
    descriptorSetLayout = shader->Get<VkDescriptorSetLayout>();
    descriptorSetUpdater = shader->GetAddress<DescriptorSetUpdater>();

    VkComputePipelineCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.layout = layout;
    createInfo.stage = stage[0];

    device->CreatePipelines(cache, 1, &createInfo, nullptr, &handle);

    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

ComputePipeline::~ComputePipeline()
{
    if (device)
    {
        device->Destroy(handle);
    }
}

void ComputePipeline::Bind(const std::string &name, const Buffer::Super *uniform)
{
    auto descriptor = rcast<const BufferDescriptor *>(uniform->Descriptor());
    descriptorSetUpdater->Set(name, descriptor);
    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

void ComputePipeline::Bind(Texture::Super *superTexture, uint32_t slot)
{
    auto texture = dynamic_cast<Texture *>(superTexture);

    for (auto &writeDescriptor : descriptorSetUpdater->WriteDescriptorSets)
    {
        if (writeDescriptor.dstBinding == slot)
        {
            VkDescriptorImageInfo *descriptorInfo = rcast<VkDescriptorImageInfo *>(&texture->DescriptorInfo());
            if (descriptorInfo != writeDescriptor.pImageInfo)
            {
                isChanged++;
                writeDescriptor.pImageInfo = descriptorInfo;
            }
            if (writeDescriptor.descriptorType == VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
            {
                VkImageSubresourceRange subresourceRange{};
                subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;
                subresourceRange.baseMipLevel = 0;
                subresourceRange.levelCount   = texture->MipLevels();
                subresourceRange.layerCount   = 1;

                barriers.emplace_back(
                    texture->Get<VkImage>(),
                    subresourceRange,
                    texture->Layout,
                    VK_IMAGE_LAYOUT_GENERAL,
                    VK_ACCESS_SHADER_READ_BIT,
                    VK_ACCESS_SHADER_WRITE_BIT
                );
                descriptorInfo->imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            }
            break;
        }
    }

    device->Compute([&](CommandBuffer *cmdbuf) -> void {
        cmdbuf->PipelineBarrier(
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            0,
            0, nullptr,
            0, nullptr,
            U32(barriers.size()), barriers.data()
        );
        });

    if (isChanged == descriptorSetUpdater->WriteDescriptorSets.size() && Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

void ComputePipeline::Bind(const Descriptor *descriptors, uint32_t slot)
{
    for (auto &writeDescriptor : descriptorSetUpdater->WriteDescriptorSets)
    {
        if (writeDescriptor.dstBinding == slot)
        {
            if (writeDescriptor.descriptorType <= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
            {
                writeDescriptor.pImageInfo = rcast<const VkDescriptorImageInfo *>(descriptors);
            }
            else
            {
                writeDescriptor.pBufferInfo = rcast<const VkDescriptorBufferInfo *>(descriptors);
            }
            break;
        }
    }

    if (Ready())
    {
        descriptorSetUpdater->Update(*device);
    }
}

void ComputePipeline::Dispatch(CommandBuffer *cmdbuf, uint32_t nGroupX, uint32_t nGroupY, uint32_t nGroupZ)
{
    isChanged = 0;

    vkCmdBindDescriptorSets(
        *cmdbuf,
        BindPoint,
        layout,
        0, 1,
        &descriptorSet,
        0, nullptr
    );
    vkCmdBindPipeline(*cmdbuf, BindPoint, handle);

    cmdbuf->Dispatch(
        nGroupX,
        nGroupY,
        nGroupZ
    );

    RenderContext::That->Submit([&](CommandBuffer *drawCmdbuf) {
        VkImageLayout layouts[] = {
            VK_IMAGE_LAYOUT_GENERAL
        };

        for (size_t i = 0; i < SL_ARRAY_LENGTH(layouts); i++)
        {
            for (auto &b : barriers)
            {
                b.Swap();
                b.To(layouts[i]);
            }

            drawCmdbuf->PipelineBarrier(
                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                0,
                0, nullptr,
                0, nullptr,
                U32(barriers.size()), barriers.data()
            );
        }
        barriers.clear();
        });
}

void ComputePipeline::Dispatch(uint32_t nGroupX, uint32_t nGroupY, uint32_t nGroupZ)
{
    device->Compute([&](CommandBuffer *cmdbuf) {
        Dispatch(cmdbuf, nGroupX, nGroupY, nGroupZ);
        });
}

void ComputePipeline::PushConstant(uint32_t size, const void *data, uint32_t offset)
{
    device->Compute([&](CommandBuffer *cmdbuf) {
        cmdbuf->PushConstants(
            layout,
            Shader::Stage::Compute,
            offset,
            size,
            data
        );
        });
}

bool ComputePipeline::Ready()
{
    bool ready = descriptorSetUpdater->Ready();
    if (ready)
    {
        Check(descriptorPool->Allocate(&descriptorSetLayout, &descriptorSet));
        descriptorSetUpdater->Set(descriptorSet);
    }
    return ready;
}

void ComputePipeline::ResetResource()
{
    // descriptorPool->Reset();
}

}
}
