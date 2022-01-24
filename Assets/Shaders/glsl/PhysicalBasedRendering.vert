#version 450

struct ModelInfo
{
    vec3  color;
    float roughness;
	float metallic;
	int   objectID;
};

struct FSInput
{
    vec3 WorldPos;
    vec3 Normal;
    vec3 Tagent;
    vec3 Bitagent;
    vec2 TexCoord;
};

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inTagent;
layout (location = 3) in vec3 inBitagent;
layout (location = 4) in vec2 inTexCoord;

layout (location = 0) out FSInput fsInput;
layout (location = 5) out flat ModelInfo outModel;

layout (binding = 0) uniform Transform
{
	mat4 viewProjection;
    mat4 skyProjection;
    mat4 sceneRotation;
} ubo;

layout (push_constant) uniform Model
{
    mat4  transform;
    vec3  color;
    float roughness;
	float metallic;
    int   objectID;
} model;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
    outModel.color     = model.color;
    outModel.roughness = model.roughness;
    outModel.metallic  = model.metallic;
    outModel.objectID  = model.objectID;

	gl_Position =  ubo.viewProjection * model.transform * vec4(inPos, 1.0);

#if VULKAN
	gl_Position.y = -gl_Position.y;
#endif

    fsInput.WorldPos = vec3(gl_Position);
    fsInput.Normal   = mat3(model.transform) * inNormal;
    fsInput.Tagent   = inTagent;
    fsInput.Bitagent = inBitagent;
    fsInput.TexCoord = inTexCoord;
}