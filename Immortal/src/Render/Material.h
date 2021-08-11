#pragma once

#include "ImmortalCore.h"
#include "Framework/Object.h"

#include "Texture.h"
#include "Shader.h"

namespace Immortal {

	class IMMORTAL_API Material : public Object
	{
	public:
		enum class Flag
		{
			None      = BIT(0),
			DepthTest = BIT(1),
			Blend     = BIT(2),
			TwoSided  = BIT(3)
		};

		struct MaterialUniforms
		{
			Vector::Vector3  AlbedoColor;
			float            Metalness;
			float            Roughness;
		};

	public:
		virtual ~Material() { }

		virtual void Set(const std::string &name, float  value)    = 0;
		virtual void Set(const std::string &name, INT32  value)      = 0;
		virtual void Set(const std::string &name, UINT32 value) = 0;
		virtual void Set(const std::string &name, bool   value)     = 0;
		virtual void Set(const std::string &name, const Vector::Vector2& value) = 0;
		virtual void Set(const std::string &name, const Vector::Vector3& value) = 0;
		virtual void Set(const std::string &name, const Vector::Vector4& value) = 0;
		virtual void Set(const std::string &name, const Vector::Matrix3& value) = 0;
		virtual void Set(const std::string &name, const Vector::Matrix4& value) = 0;

		virtual void Set(const std::string& name, const Ref<Texture2D>& texture)   = 0;
		virtual void Set(const std::string& name, const Ref<TextureCube>& texture) = 0;

		virtual Ref<Texture2D> texture(const std::string &name) = 0;
		virtual Ref<TextureCube> textureCube(const std::string &name) = 0;

		virtual UINT32 Flags() const = 0;

		virtual Ref<Shader> shader() = 0;
		virtual const std::string &Name() const = 0;
	};


}