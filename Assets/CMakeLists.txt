cmake_minimum_required(VERSION 3.16)

project("Shaders")

set(GLSL_FILES
    AnimatedBasic3D.frag
    AnimatedBasic3D.vert
    Basic.frag
    Basic.vert
    Basic3D.frag
    Basic3D.vert
    ColorMixing.comp
    color_space_nv122rgba8.comp
    color_space_yuv2rgba8.comp
    Equirect2Cube.comp
    GaussianBlur.comp
    Outline.frag
    Outline.vert
    PhysicalBasedRendering.frag
    PhysicalBasedRendering.vert
    Render2D.frag
    Render2D.vert
    SimpleBlur.comp
    Skybox.frag
    Skybox.vert
    Texture.frag
    Texture.vert)

set(HLSL_FILES
    Basic.hlsl
    Basic3D.hlsl
    ColorMixing.hlsl
    color_space_nv122rgba.hlsl
    color_space_yuvp2rgba.hlsl
    Render2D.hlsl
    Texture.hlsl)

list(TRANSFORM GLSL_FILES PREPEND "Shaders/glsl/")
list(TRANSFORM HLSL_FILES PREPEND "Shaders/hlsl/")

source_group("glsl\\" FILES ${GLSL_FILES})
source_group("hlsl\\" FILES ${HLSL_FILES})

add_custom_target(${PROJECT_NAME}
    SOURCES
        ${GLSL_FILES}
        ${HLSL_FILES}
    )
