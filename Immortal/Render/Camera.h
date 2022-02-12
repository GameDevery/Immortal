#pragma once

#include "Core.h"
#include "Math/Vector.h"

namespace Immortal
{

class Camera
{
public:
    enum class ProjectionType
    { 
        Perspective  = 0,
        Orthographic = 1
    };

public:
    Camera() = default;

    Camera(const Matrix4 &prj)
        : projection{ prj }
    {

    }

    virtual ~Camera() = default;

    const Matrix4 Projection() const
    { 
        return projection;
    }

    const Matrix4 &View() const
    { 
        return view;
    }

    virtual Matrix4 ViewProjection() const
    { 
        return projection * view;
    }

    void SetProjection(const Matrix4 &prj)
    { 
        projection = prj;
    }

    float Exposure() const
    { 
        return exposure;
    }

    float &Exposure()
    {
        return exposure;
    }

protected:
    Matrix4 view{ 1.0f };

    Matrix4 projection{ 1.0f };

    float exposure{ 0.8f };
};

}
