cmake_minimum_required(VERSION 3.16)

project(Hyperion LANGUAGES C CXX)

file(GLOB_RECURSE HYPERION_SOURCES CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE HYPERION_HEADERS CONFIGURE_DEPENDS src/*.h)

set(GLAD_DIR "${CMAKE_SOURCE_DIR}/vendor/GLAD/src")
if (EXISTS "${GLAD_DIR}/glad.c")
    set(GLAD_SOURCES "${GLAD_DIR}/glad.c"
    )
else ()
    file(GLOB_RECURSE GLAD_SOURCES CONFIGURE_DEPENDS "${GLAD_DIR}/*.c")
endif ()

if (NOT GLAD_SOURCES)
    message(FATAL_ERROR "No GLAD sources found in ${GLAD_DIR}. Make sure vendor/GLAD/src/glad.c exists or update the path in CMakeLists.txt")
endif ()

add_library(glad STATIC ${GLAD_SOURCES})
add_library(glad::glad ALIAS glad)
target_include_directories(glad SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/vendor/GLAD/include)
if (CMAKE_C_COMPILER)
    set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
elseif (CMAKE_CXX_COMPILER)
    set_target_properties(glad PROPERTIES LINKER_LANGUAGE CXX)
endif ()

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/vendor/imgui")
set(IMGUI_SYSTEM_BACKENDS_DIR "/usr/share/doc/libimgui-dev/examples/backends")

if (EXISTS "${IMGUI_DIR}/imgui.cpp")
    set(IMGUI_SOURCES
            "${IMGUI_DIR}/imgui.cpp"
            "${IMGUI_DIR}/imgui_demo.cpp"
            "${IMGUI_DIR}/imgui_draw.cpp"
            "${IMGUI_DIR}/imgui_tables.cpp"
            "${IMGUI_DIR}/imgui_widgets.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
            "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
    )

    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui SYSTEM PUBLIC "${IMGUI_DIR}" "${IMGUI_DIR}/backends")
    target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad)
elseif (EXISTS "${IMGUI_SYSTEM_BACKENDS_DIR}/imgui_impl_glfw.cpp")
    # System-installed imgui (e.g., libimgui-dev on Debian/Ubuntu).
    # The static library already includes the core imgui objects; only the
    # platform/renderer backends need to be compiled separately.
    add_library(imgui STATIC
            "${IMGUI_SYSTEM_BACKENDS_DIR}/imgui_impl_glfw.cpp"
            "${IMGUI_SYSTEM_BACKENDS_DIR}/imgui_impl_opengl3.cpp"
    )
    target_include_directories(imgui SYSTEM PUBLIC
            /usr/include
            /usr/include/imgui
            /usr/include/imgui/backends
    )
    target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad /usr/lib/x86_64-linux-gnu/libimgui.a stb)
    target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
else()
    message(FATAL_ERROR "imgui not found. Please initialise the vendor/imgui git submodule or install libimgui-dev.")
endif()

add_library(Hyperion SHARED
        ${HYPERION_SOURCES}
        ${HYPERION_HEADERS}
)

target_compile_features(Hyperion PUBLIC cxx_std_17)
target_include_directories(Hyperion PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (WIN32)
    target_compile_definitions(Hyperion PUBLIC HYPERION_PLATFORM_WINDOWS PRIVATE HYPERION_BUILD_DLL)
elseif (UNIX AND NOT APPLE)
    target_compile_definitions(Hyperion PUBLIC HYPERION_PLATFORM_LINUX PRIVATE HYPERION_BUILD_DLL)
elseif (APPLE)
    target_compile_definitions(Hyperion PUBLIC HYPERION_PLATFORM_MACOS PRIVATE HYPERION_BUILD_DLL)
endif ()

# Enable assertions in Debug builds
target_compile_definitions(Hyperion PUBLIC
    $<$<CONFIG:Debug>:HYPERION_ENABLE_ASSERTS>
)

hyperion_configure_target(Hyperion)

if (HYPERION_ENABLE_PCH)
    target_precompile_headers(Hyperion PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Hyperion/hyperionpch.h)
endif ()

target_link_libraries(Hyperion
        PUBLIC
        spdlog::spdlog
        glfw
        glad
        OpenGL::GL
        imgui
        glm::glm
)
