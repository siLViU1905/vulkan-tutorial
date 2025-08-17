cmake_minimum_required(VERSION 3.31)
project(vulkan)

set(CMAKE_CXX_STANDARD 20)

#modify the version as you want
set(VULKAN_SDK "C:/VulkanSDK/1.4.321.1")
set(Vulkan_ROOT "${VULKAN_SDK}")

set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (MSVC)
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib")
elseif (MINGW OR CMAKE_COMPILER_IS_GNUCXX)
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a")
endif ()

find_package(Vulkan REQUIRED)


add_executable(vulkan
        main.cpp
        app/VulkanApp.cpp
        app/Vertex.cpp
        app/Mesh.cpp

        #imgui
        include/imgui/imgui.cpp
        include/imgui/imgui_demo.cpp
        include/imgui/imgui_draw.cpp
        include/imgui/imgui_widgets.cpp
        include/imgui/imgui_tables.cpp
        include/imgui/imgui_impl_glfw.cpp
        include/imgui/imgui_impl_vulkan.cpp
        app/LightBuffer.cpp
)

target_link_libraries(vulkan
        Vulkan::Vulkan
        ${GLFW_LIBRARIES}
)
