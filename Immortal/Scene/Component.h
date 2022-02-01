#pragma once

#include "Core.h"

#include "Render/Render.h"
#include "Render/Mesh.h"
#include "Render/Texture.h"
#include "SceneCamera.h"

namespace IMMORTAL_API Immortal
{

struct Component
{
    enum class Type
    {
        None,
        ColorMixing,
        ID,
        Filter,
        Tag,
        Transform,
        Mesh,
        Meta,
        Light,
        Script,
        Scene,
        SpriteRenderer,
        Camera
    };

    Component(Type type) :
        type{ type }
    {
        
    }

    Type type{ Type::None };
};

struct IDComponent : public Component
{
    IDComponent() :
        Component{ Type::ID }
    {
        
    }

    IDComponent(uint64_t id) :
        Component{ Type::ID },
        uid(id)
    {
    
    }

    uint64_t uid{ 0 };
};

struct TagComponent : public Component
{
    TagComponent() :
        Component{ Type::Tag }
    {
        
    }

    TagComponent(const std::string &tag) :
        Component{ Type::Tag },
        Tag{ tag }
    {

    }

    std::string Tag;
};

struct TransformComponent : public Component
{
    TransformComponent() :
        Component{ Type::Transform }
    {
        
    }

    void Set(Vector3 position, Vector3 rotation, Vector3 scale)
    { 
        Position = position;
        Rotation = rotation;
        Scale    = scale;
    }

    Matrix4 Transform() const
    {
        return Vector::Translate(Position) * Vector::Rotate(Rotation) * Vector::Scale(Scale);
    }

    operator Matrix4() const
    {
        return Transform();
    }

    static constexpr Vector3 Up{ 0.0f, 1.0f, 0.0f };

    static constexpr Vector3 Right{ 1.0f, 0.0f, 0.0f };

    static constexpr Vector3 Forward{ 0.0f, 0.0f, -1.0f };

    Vector3 Position{ 0.0f, 0.0f, 0.0f };

    Vector3 Rotation{ 0.0f, 0.0f, 0.0f };

    Vector3 Scale{ 1.0f, 1.0f, 1.0f };
};

struct MeshComponent : public Component
{
    MeshComponent() :
        Component{ Type::Mesh }
    {
    
    }

    MeshComponent(std::shared_ptr<Immortal::Mesh> mesh) :
        Component{ Type::Mesh },
        Mesh{ mesh }
    {
    
    }

    operator std::shared_ptr<Immortal::Mesh>()
    {
        return Mesh;
    }

    std::shared_ptr<Immortal::Mesh> Mesh;
};

struct MaterialComponent : public Component
{
    MaterialComponent() :
        Component{ Type::Mesh },
        AlbedoColor{ 0.995f, 0.995f, 0.995f },
        Metallic{ 1.0f },
        Roughness{ 1.0f }
    {
        Textures.Albedo    = Render::Preset()->WhiteTexture;
        Textures.Normal    = Textures.Albedo;
        Textures.Metallic  = Textures.Albedo;
        Textures.Roughness = Textures.Albedo;
        Textures.AO        = Textures.Albedo;
    }

    struct {
        std::shared_ptr<Texture> Albedo;
        std::shared_ptr<Texture> Normal;
        std::shared_ptr<Texture> Metallic;
        std::shared_ptr<Texture> Roughness;
        std::shared_ptr<Texture> AO;
    } Textures;

    Vector3 AlbedoColor;
    float   Metallic;
    float   Roughness;
};

struct LightComponent : public Component
{
    LightComponent() :
        Component{ Type::Light }
    {
        
    }
};

struct SceneComponent : public Component
{
    SceneComponent() :
        Component{ Type::Scene }
    {
    
    }
};

struct SpriteRendererComponent : public Component
{
    static inline Texture::Description Desc = {
        Format::RGBA8,
        Wrap::Repeat,
        Filter::Linear
    };

    SpriteRendererComponent() :
        Component{ Type::SpriteRenderer }
    {
        Texture = Render::Preset()->WhiteTexture;
    }

    SpriteRendererComponent(std::shared_ptr<Texture> texture) :
        Component{ Type::SpriteRenderer },
        Texture{ texture }
    {

    }

    SpriteRendererComponent(std::shared_ptr<Texture> texture, const Vector4 color) :
        Component{ Type::SpriteRenderer },
        Texture{ texture },
        Color{ color }
    {

    }

    SpriteRendererComponent(const SpriteRendererComponent &other) = default;

    std::shared_ptr<Immortal::Texture> Texture = Render::Preset()->WhiteTexture;

    std::shared_ptr<Immortal::Texture> Final   = nullptr;

    Vector4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

    float TilingFactor = 1.0f;
};

struct CameraComponent : public Component
{
    CameraComponent() :
        Component{ Type::Camera }
    {
    
    }

    CameraComponent(const CameraComponent &other) = default;

    operator SceneCamera&()
    { 
        return Camera;
    }

    operator const SceneCamera&() const
    { 
        return Camera;
    }

    SceneCamera Camera;

    bool Primary = false;
};

struct DirectionalLightComponent : public Component
{
    DirectionalLightComponent() :
        Component{ Type::Light }
    {
    
    }

    Vector3 Radiance{ 1.0f, 1.0f, 1.0f };

    float Intensity   = 1.0f;
    bool  CastShadows = true;
    bool  SoftShadows = true;
    float LightSize   = 0.5f; // For PCSS
};

struct ScriptComponent : public Component
{
    ScriptComponent() :
        Component{ Type::Script }
    {
    
    }

    ScriptComponent(const ScriptComponent & other) = default;

    ScriptComponent(const std::string &name) :
        Component{ Type::Script },
        Name{ name }
    {

    }

    std::string Name;
};

struct MetaComponent : public Component
{
    MetaComponent() :
        Component{ Type::Meta }
    {

    }

    void *Meta = nullptr;
};

struct ColorMixingComponent : public Component
{
    ColorMixingComponent() :
        Component{ Type::ColorMixing }
    {

    }

    bool Modified    = false;
    bool Initialized = false;

    Vector4 RGBA = Vector4{ 0, 0, 0, 1.0f };
    Vector4 HSL;
    
    struct {
        float ColorTemperature;
        float Hue;
    } WhiteBalance{ 0, 0 };

    struct {
        float White;
        float Black;
    } Gradation{ 0, 0 };

    float Exposure    = 0;
    float Contrast    = 0;
    float Hightlights = 0;
    float Shadow      = 0;
    float Vividness   = 0;
};

enum class FilterType
{
    None,
    GaussianBlur,
    AverageBlur,
    DCT
};

struct FilterComponent : public Component
{
    FilterComponent() :
        Component{ Type::Filter }
    {

    }

    std::vector<FilterType> Filter;
};

}
