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

if (EXISTS "${IMGUI_DIR}/imgui.cpp")
    # Use the locally checked-out vendor/imgui submodule.
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
else()
    # Fall back to a system-installed imgui.
    # Locate the core header and static library portably.
    find_path(IMGUI_INCLUDE_DIR imgui.h PATH_SUFFIXES imgui)
    find_path(IMGUI_BACKENDS_SRC_DIR imgui_impl_glfw.cpp
              PATHS
                  /usr/share/doc/libimgui-dev/examples/backends  # Debian/Ubuntu
              PATH_SUFFIXES
                  imgui/backends
                  backends
    )
    find_library(IMGUI_LIBRARY NAMES imgui)
    find_library(STB_LIBRARY NAMES stb)

    if (IMGUI_INCLUDE_DIR AND IMGUI_BACKENDS_SRC_DIR AND IMGUI_LIBRARY)
        # The static library already includes the core imgui objects; only the
        # platform/renderer backends need to be compiled separately here.
        add_library(imgui STATIC
                "${IMGUI_BACKENDS_SRC_DIR}/imgui_impl_glfw.cpp"
                "${IMGUI_BACKENDS_SRC_DIR}/imgui_impl_opengl3.cpp"
        )
        target_include_directories(imgui SYSTEM PUBLIC
                "${IMGUI_INCLUDE_DIR}"
                "${IMGUI_INCLUDE_DIR}/backends"
                "${IMGUI_BACKENDS_SRC_DIR}"
        )
        set(_IMGUI_LINK_LIBS glfw OpenGL::GL glad "${IMGUI_LIBRARY}")
        if (STB_LIBRARY)
            list(APPEND _IMGUI_LINK_LIBS "${STB_LIBRARY}")
        endif()
        target_link_libraries(imgui PUBLIC ${_IMGUI_LINK_LIBS})
        target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    else()
        message(FATAL_ERROR
            "imgui not found. Please either:\n"
            "  1. Initialise the vendor/imgui git submodule, or\n"
            "  2. Install imgui development files (e.g. libimgui-dev on Debian/Ubuntu).")
    endif()
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
