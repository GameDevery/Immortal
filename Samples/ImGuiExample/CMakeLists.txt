cmake_minimum_required(VERSION 3.16)

project("ImGuiExample" LANGUAGES CXX)

set(SRC_FILES
    Main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Immortal/ImGui/imgui_impl_immortal.cpp)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
)

source_group("\\" FILES ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Immortal/
)

target_link_libraries(${PROJECT_NAME}
    ImmortalGraphics
    imgui
)

target_link_runtime(${PROJECT_NAME} ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Examples")
