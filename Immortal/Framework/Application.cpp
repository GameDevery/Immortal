#include "Application.h"

#include "Log.h"
#include "Async.h"
#include "Render/Graphics.h"
#include "Script/ScriptEngine.h"
#include "Graphics/AsyncCompute.h"

namespace Immortal
{

Application *Application::This = nullptr;

Application::Application(BackendAPI graphicsBackendAPI, const std::string &title, uint32_t width, uint32_t height) :
    eventSink{ this },
    name{ title }
{
	!!This ? throw Exception(SError::InvalidSingleton) : This = this;

    eventSink.Listen(&Application::OnWindowClosed, Event::Type::WindowClose);
    eventSink.Listen(&Application::OnWindowResize, Event::Type::WindowResize);
    eventSink.Listen(&Application::OnWindowMove,   Event::Type::WindowMove);

    Async::Init();

	window = Window::CreateInstance(title, width, height, graphicsBackendAPI == BackendAPI::OpenGL ? WindowType::GLFW : WindowType::None);
    window->SetIcon("Assets/Icon/Terminal.png");
    window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

    instance = Instance::CreateInstance(graphicsBackendAPI, window->GetType());
    device = instance->CreateDevice(0);
    queue = device->CreateQueue(QueueType::Graphics);

	Graphics::SetDevice(device);
	Graphics::Execute<SetQueueTask>(queue);
	Graphics::Execute<AsyncTask>(AsyncTaskType::BeginRecording);
    Graphics::ConstructGlobalVariables();

    swapchain = device->CreateSwapchain(queue, window, Format::BGRA8, bufferCount, SwapchainMode::VerticalSync);

    commandBuffers.resize(bufferCount);
    for (size_t i = 0; i < bufferCount; i++)
    {
		commandBuffers[i] = device->CreateCommandBuffer();
    }

    gpuEvent = device->CreateGPUEvent("ApplicationGPUEvent");

    gui = new GuiLayer{ device, queue, window, swapchain };
	gui->OnAttach();
}

Application::~Application()
{
	timer.Stop();

    for (auto &layer : layerStack)
    {
		layer->OnDetach();
    }

	layerStack.clear();

    gui->OnDetach();
	gui.Reset();

    Graphics::Release();

    commandBuffers.clear();
	swapchain.Reset();
	gpuEvent.Reset();
    queue.Reset();
	window.Reset();
	device.Reset();
	instance.Reset();
	Async::Release();
}

Layer *Application::PushLayer(Layer *layer)
{
    layerStack.PushLayer(layer);
    layer->OnAttach();

    return layer;
}

Layer *Application::PushOverlay(Layer *overlay)
{
    layerStack.PushOverlay(overlay);
    overlay->OnAttach();

    return overlay;
}

void Application::OnRender()
{
    Time::DeltaTime = timer.tick<Timer::Seconds>();

	Graphics::Execute<AsyncTask>(AsyncTaskType::BeginRecording);
    for (Layer *layer : layerStack)
    {
        layer->OnUpdate();
    }
	Graphics::Execute<AsyncTask>(AsyncTaskType::EndRecording);
	Graphics::Execute<AsyncTask>(AsyncTaskType::Submiting);

    if (!runtime.minimized)
    {
        gui->Begin();
        gui->Render();
        gui->End();

        swapchain->PrepareNextFrame();
        gpuEvent->Wait(syncValues[syncPoint], 0xffffffffff);
	    Graphics::SetRenderIndex(syncValues[syncPoint]);

        CommandBuffer *commandBuffer = commandBuffers[syncPoint];

        const float clearColor[4] = { 0, 0, 0, 0 };
	    commandBuffer->Begin();
        RenderTarget *renderTarget = swapchain->GetCurrentRenderTarget();
	    commandBuffer->BeginRenderTarget(renderTarget, clearColor);
        gui->SubmitRenderDrawCommands(commandBuffer);
	    commandBuffer->EndRenderTarget();
	    commandBuffer->End();

        GPUEvent *submitGPUEvent[] = { gpuEvent };
	    queue->Submit(&commandBuffer, 1, submitGPUEvent, 1, swapchain);
		queue->Present(swapchain, nullptr, 0);

	    syncValues[syncPoint] = gpuEvent->GetSyncPoint();

	    SLROTATE(syncPoint, bufferCount);
    }

    window->ProcessEvents();
}

void Application::Run()
{
	Graphics::Execute<AsyncTask>(AsyncTaskType::EndRecording);
	Graphics::Execute<AsyncTask>(AsyncTaskType::Submiting);
	Graphics::WaitIdle();

	window->Show();
    while (runtime.running)
    {
        OnRender();
    }
}

void Application::Close()
{
    runtime.running = false;
}

void Application::OnEvent(Event &e)
{
    eventSink.Dispatch(e);
    if (gui)
    {
		gui->OnEvent(e);
    }

    for (auto it = layerStack.end(); it != layerStack.begin(); )
    {
        (*--it)->OnEvent(e);
        if (e.Handled)
        {
            break;
        }
    }
}

bool Application::OnWindowClosed(WindowCloseEvent &e)
{
    /* Wait all threads to finish before closing */

    runtime.running = false;
    return !runtime.running;
}

bool Application::OnWindowResize(WindowResizeEvent &e)
{
    auto width  = e.Width();
    auto height = e.Height();

	runtime.minimized = e.Width() == 0 || e.Height() == 0;

    if (!runtime.minimized)
    {
		queue->WaitIdle(0xffffffff);
		swapchain->Resize(width, height);
    }

    OnRender();

    return runtime.minimized;
}

bool Application::OnWindowMove(WindowMoveEvent &e)
{
    OnRender();
    return true;
}

}
