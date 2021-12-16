#pragma once

#include "ImmortalCore.h"
#include "Render/Renderer.h"

#include "RenderContext.h"
#include <glad/glad.h>

#include "Texture.h"
#include "Buffer.h"
#include "Shader.h"
#include "Pipeline.h"
#include "RenderTarget.h"

namespace Immortal
{
namespace OpenGL
{

class Renderer : public SuperRenderer
{
public:
    using Super = SuperRenderer;

public:
    Renderer(SuperRenderContext *context);

    virtual void Setup() override;

    virtual void OnResize(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

    virtual void SetClearColor(const Vector::Vector4 &color) override;

    virtual void Clear() override;

    virtual void EnableDepthTest() override;

    virtual void DisableDepthTest() override;

    virtual void SwapBuffers() override;

    virtual const char *GraphicsRenderer() override
    {
        return context->GraphicsRenderer();
    }

    virtual std::shared_ptr<SuperTexture> CreateTexture(const std::string &filepath) override
    {
        return std::make_shared<Texture>(filepath);
    }

    virtual std::shared_ptr<SuperTexture> CreateTexture(uint32_t width, uint32_t height, const void *data, const Texture::Description &description) override
    {
        return std::make_shared<Texture>(width, height, data, description, 0);
    }

    virtual std::shared_ptr<SuperBuffer> CreateBuffer(const size_t size, int binding) override
    {
        return std::make_shared<Buffer>(size, binding);
    }

    virtual std::shared_ptr<SuperBuffer> CreateBuffer(const size_t size, const void *data, Buffer::Type type) override
    {
        return std::make_shared<Buffer>(size, data, type);
    }

    virtual std::shared_ptr<SuperBuffer> CreateBuffer(const size_t size, Buffer::Type type) override
    {
        return std::make_shared<Buffer>(size, type);
    }

    virtual std::shared_ptr<SuperPipeline> CreatePipeline(std::shared_ptr<SuperShader> &shader)
    {
        return std::make_shared<Pipeline>(shader);
    }

    virtual std::shared_ptr<SuperRenderTarget>CreateRenderTarget(const RenderTarget::Description &description) override
    {
        return std::make_shared<RenderTarget>(description);
    }

    virtual std::shared_ptr<Shader::Super> CreateShader(const std::string &filepath, Shader::Type type) override
    {
        return std::make_shared<Shader>(filepath, type);
    }

    virtual void Draw(const std::shared_ptr<Pipeline::Super> &pipeline) override;

    virtual void Begin(std::shared_ptr<RenderTarget::Super> &renderTarget) override;

    virtual void End() override;

private:
    RenderContext *context{ nullptr };
};

}
}