#pragma once

#include "ImmortalCore.h"
#include "Render/RenderContext.h"
#include "Framework/Device.h"

struct GLFWwindow;
namespace Immortal
{
namespace OpenGL
{

class RenderContext : public SuperRenderContext
{
public:
    using Super = SuperRenderContext;

public:
    RenderContext(SuperRenderContext::Description &desc);

    Device *GetDevice() override
    {
        return nullptr;
    }

    void INIT();

    GLFWwindow *Handle()
    {
        return handle;
    }

private:
    GLFWwindow *handle;
};

}
}