#include "SceneSerializer.h"

#include "FileSystem/FileSystem.h"
#include "Scene/Object.h"
#include "Helper/json.h"

namespace Immortal
{

namespace ns
{

void to_json(JSON::SuperJSON &j, const Vector3 &v)
{
    j = JSON::SuperJSON{
        { "x", v.x },
        { "y", v.y },
        { "z", v.z },
    };
}

void to_json(JSON::SuperJSON &j, const Vector4 &v)
{
    j = JSON::SuperJSON{
        { "x", v.x },
        { "y", v.y },
        { "z", v.z },
        { "w", v.z }
    };
}

void to_json(JSON::SuperJSON &j, const char *v)
{
    j = v ? "" : v;
}

void from_json(const JSON::SuperJSON &j, Vector3 &v)
{
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
}

void from_json(const JSON::SuperJSON &j, Vector4 &v)
{
    j.at("x").get_to(v.x);
    j.at("y").get_to(v.y);
    j.at("z").get_to(v.z);
    j.at("w").get_to(v.w);
}

const JSON::SuperJSON TryFind(const JSON::SuperJSON &j, const std::string &key)
{
    if (j.find(key) != j.end())
    {
        return j[key];
    }

    return JSON::SuperJSON{};
}

}

SceneSerializer::SceneSerializer()
{

}

void SceneSerializer::Serialize(Scene *scene, const std::string &path)
{
    using namespace ::nlohmann;

    Stream stream{ path, Stream::Mode::Write };
    if (!stream.Writable())
    {
        LOG::INFO("Failed to serialize!");
        return;
    }

    json data;
    data["version"] = "0.0.1";
    auto &objects = data["Objects"];
    scene->Registry().each([&](Object::Primitive primitive) {
        Object object{ primitive, scene };
        if (!object)
        {
            return;
        }

        json objectData;
        const auto &c = object.GetComponent<TagComponent>();
        objectData["Name"] = c.Tag;

        if (object.HasComponent<TransformComponent>())
        {
            auto &transform = objectData["Transform"];
            const auto &t   = object.GetComponent<TransformComponent>();
            ns::to_json(transform["Position"], t.Position);
            ns::to_json(transform["Rotation"], t.Rotation);
            ns::to_json(transform["Scale"], t.Scale);
        }
        if (object.HasComponent<LightComponent>())
        {
            auto &light   = objectData["Light"];
            const auto &l = object.GetComponent<LightComponent>();
            ns::to_json(light["Radiance"], l.Radiance);
            light["Enabled"] = l.Enabled;
        }
        if (object.HasComponent<SpriteRendererComponent>())
        {
            auto &sprite     = objectData["SpriteRenderer"];
            const auto &s    = object.GetComponent<SpriteRendererComponent>();
            ns::to_json(sprite["Color"], s.Color);
            //sprite["Source"]          = s.Sprite->Source();
            sprite["TilingFactor"] = s.TilingFactor;
        }
        if (object.HasComponent<MeshComponent>())
        {
            auto &meshObject = objectData["Mesh"];
            const auto &mesh = object.GetComponent<MeshComponent>().Mesh;
            meshObject["Source"] = mesh->Source();

            if (object.HasComponent<MaterialComponent>())
            {
                auto &materialObject = objectData["Material"];
                const auto &materialComponent = object.GetComponent<MaterialComponent>();
                for (auto &material : materialComponent.References)
                {
                    json j;
                    ns::to_json(j["Albedo"], material.AlbedoColor);
                    j["Metalness"] = material.Metallic;
                    j["Roughness"] = material.Roughness;

                    auto &textures = j["Textures"];
                    //textures["Albedo"   ] = material.Textures.Albedo->Source();
                    //textures["Normal"   ] = material.Textures.Normal->Source();
                    //textures["Metalness"] = material.Textures.Metallic->Source();
                    //textures["Roughness"] = material.Textures.Roughness->Source();
                    materialObject.emplace_back(std::move(j));
                }
            }
            else
            {
                LOG::ERR("Material Component Missing");
            }
        }
        if (object.HasComponent<ScriptComponent>())
        {
            auto &script = object.GetComponent<ScriptComponent>();
            auto &scriptObject = objectData["Script"];
            scriptObject["Source"] = script.path;
        }
        if (object.HasComponent<VideoPlayerComponent>())
        {
            auto &videoPlayer = object.GetComponent<VideoPlayerComponent>();
            auto &videoPlayerObject = objectData["VideoPlayer"];
            videoPlayerObject["Source"] = videoPlayer.GetSource();
        }
        if (object.HasComponent<CameraComponent>())
        {
            auto &camera = object.GetComponent<CameraComponent>();
            auto &cameraObject = objectData["Camera"];
            cameraObject["Primary"] = camera.Primary;
            cameraObject["ProjectionType"] = camera.Camera.GetType();
        }
        objects.emplace_back(objectData);
        });

    auto str = data.dump();
    stream.Write(str);
}

bool SceneSerializer::Deserialize(Scene *scene, const std::string &filepath)
{
    auto json = JSON{}.Parse(filepath);

    for (const auto &data : json["Objects"])
    {
        Object object = scene->CreateObject(data["Name"]);

        const auto &transform = ns::TryFind(data, "Transform");
        if (!transform.is_null())
        {
            auto &t = object.GetComponent<TransformComponent>();
            ns::from_json(transform["Position"], t.Position);
            ns::from_json(transform["Rotation"], t.Rotation);
            ns::from_json(transform["Scale"], t.Scale);
        }

        const auto &light = ns::TryFind(data, "Light");
        if (!light.is_null())
        {
            auto &l = object.AddComponent<LightComponent>();
            l.Enabled = light["Enabled"];
            ns::from_json(light["Radiance"], l.Radiance);
        }

        const auto &sprite = ns::TryFind(data, "SpriteRenderer");
        if (!sprite.is_null())
        {
            auto &s = object.AddComponent<SpriteRendererComponent>();
            ns::from_json(sprite["Color"], s.Color);
            s.TilingFactor = sprite["TilingFactor"];
			s.Sprite = Graphics::CreateTexture(sprite["Source"]);
			s.Result = Graphics::CreateTexture(Format::RGBA8, s.Sprite->GetWidth(), s.Sprite->GetHeight());

            object.AddComponent<ColorMixingComponent>();
        }

        const auto &meshObject = ns::TryFind(data, "Mesh");
        if (!meshObject.is_null())
        {
            auto &meshComponent = object.AddComponent<MeshComponent>();
            meshComponent.Mesh.reset(new Mesh{ meshObject["Source"] });
        }

        const auto &materialObject = ns::TryFind(data, "Material");
        if (!materialObject.is_null())
        {
            auto &meshComponent = object.GetComponent<MeshComponent>();
            auto &material = object.AddComponent<MaterialComponent>();
            material.References.resize(meshComponent.Mesh->Size());

            auto LoadTexture = [&](Ref<Texture> &texture, const std::string &path) {
                if (!path.empty())
                {
					texture = Graphics::CreateTexture(path);
                }
            };
            for (size_t i = 0; i < materialObject.size(); i++)
            {
                auto &ref = material.References[i];
                const auto &m = materialObject[i];

                ns::from_json(m["Albedo"], ref.AlbedoColor);

                ref.Metallic  = m["Metalness"];
                ref.Roughness = m["Roughness"];
                
                auto &textures = m["Textures"];
                LoadTexture(ref.Textures.Albedo,    textures["Albedo"]);
                LoadTexture(ref.Textures.Normal,    textures["Normal"]);
                LoadTexture(ref.Textures.Metallic,  textures["Metalness"]);
                LoadTexture(ref.Textures.Roughness, textures["Roughness"]);
            }
        }

        const auto &scriptObject = ns::TryFind(data, "Script");
        if (!scriptObject.is_null())
        {
            auto &script = object.AddComponent<ScriptComponent>(scriptObject["Source"]);
            script.Init((int)object, scene);
        }

        const auto &cameraObject = ns::TryFind(data, "Camera");
        if (!cameraObject.is_null())
        {
            auto &camera = object.AddComponent<CameraComponent>();
            camera.Primary = cameraObject["Primary"];
            camera.Camera.SetProjectionType(cameraObject["ProjectionType"]);
        }

        const auto &videoPlayerObject = ns::TryFind(data, "VideoPlayer");
        if (!videoPlayerObject.is_null())
        {

        }
    }

    return false;
}

}
