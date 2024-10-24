#include "EditorCamera.h"
#include "Framework/Timer.h"
#include "Input.h"
#include "Math/Math.h"

namespace Immortal
{

EditorCamera::EditorCamera(const Matrix4 &projection) :
    Camera{ projection }
{
    distance = Vector3{ -0.8f, 0.8f, 0.8f }.Distance(focalPoint);

    yaw = 0.0f;
    pitch = 0.0f;

    UpdateView();
}

void EditorCamera::Focus(const Vector3 & focusPoint)
{
    focalPoint = focusPoint;
    if (distance > minFocusDistance)
    {
        MouseZoom((distance - minFocusDistance) / ZoomSpeed());
        UpdateView();
    }
}

void EditorCamera::OnUpdate()
{
    if (Input::IsKeyPressed(KeyCode::LeftAlt))
    {
        auto pos = Input::GetMousePosition();
        Vector2 mouse = { pos.x, pos.y };
        Vector2 delta = (mouse - initialPosition) * 0.003f;
        initialPosition = mouse;

        if (Input::IsMouseButtonPressed(MouseCode::Middle))
        {
            MousePan(delta);
        }
        else if (Input::IsMouseButtonPressed(MouseCode::Left))
        {
            MouseRotate(delta);
        }
        else if (Input::IsMouseButtonPressed(MouseCode::Right))
        {
            MouseZoom(delta.y);
        }
    }

    UpdateView();
}

void EditorCamera::OnEvent(Event & e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&EditorCamera::OnMouseScroll, this, std::placeholders::_1));
}

Vector3 EditorCamera::UpDirection()
{
    return Vector::Rotate(Orientation(), Vector3(0.0f, 1.0f, 0.0f));
}

Vector3 EditorCamera::RightDirection()
{
    return Vector::Rotate(Orientation(), Vector3(1.0f, 0.0f, 0.0f));
}

Vector3 EditorCamera::ForwardDirection()
{
    return Vector::Rotate(Orientation(), Vector3(0.0f, 0.0f, -1.0f));
}

Quaternion EditorCamera::Orientation() const
{
    return Quaternion{ Vector3{ -pitch, -yaw, 0.0f } };
}

void EditorCamera::UpdateView()
{
    position = CalculatePosition();
    Quaternion orientation = Orientation();
    rotation = Vector::EulerAngles(orientation) * (float)(180.0f / Math::PI);
    view     = Vector::Translate(position) * Vector::ToMatrix4(orientation);
    view     = Vector::Inverse(view);
}

bool EditorCamera::OnMouseScroll(MouseScrolledEvent & e)
{
    float delta = e.GetOffsetY() * Time::DeltaTime;
    MouseZoom(delta);
    UpdateView();
    return false;
}

void EditorCamera::MousePan(const Vector::Vector2 & delta)
{
    Vector::Vector2 speed = PanSpeed();
    focalPoint += -RightDirection() * delta.x * speed.x * distance;
    focalPoint += UpDirection() * delta.y * speed.y * distance;
}

void EditorCamera::MouseRotate(const Vector::Vector2 & delta)
{
    float speed = 115.0f * Time::DeltaTime;
    float yawSign = UpDirection().y < 0 ? -1.0f : 1.0f;
    yaw += yawSign * delta.x * speed;
    pitch += delta.y * speed;
}

void EditorCamera::MouseZoom(float delta)
{
    distance -= delta * ZoomSpeed();
    if (distance < 1.0f)
    {
        focalPoint += ForwardDirection();
        distance = 1.0f;
    }
}

Vector3 EditorCamera::CalculatePosition()
{
    return focalPoint - ForwardDirection() * distance;
}

template <class T>
inline float SpeedFactor(T &x)
{
    return 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
}

Vector2 EditorCamera::PanSpeed() const
{
    float deltaTime = Time::DeltaTime;
    float x = std::min(viewportSize.x, 240.0f) * deltaTime;
    float xFactor = SpeedFactor(x);
    xFactor = Math::Lerp(0.0f, 1.5f, xFactor);

    float y = std::min(viewportSize.y, 240.0f) *deltaTime;
    float yFactor = SpeedFactor(y);
    yFactor = Math::Lerp(0.0f, 1.5f, yFactor);

    return { xFactor, yFactor };
}

float EditorCamera::ZoomSpeed() const
{
    float speed = distance * 50.0f * Time::DeltaTime;
    speed = std::max(speed, 0.0f);
    return std::min(speed * speed, 100.0f);
}

void EditorCamera::SetViewportSize(Vector2 size)
{
    viewportSize = size;
    SetProjection(Vector::PerspectiveFOV(FOV, viewportSize.x, viewportSize.y, (float)0.1, 1000));
}
}
