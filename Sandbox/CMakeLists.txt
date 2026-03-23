cmake_minimum_required(VERSION 3.16)

project(Sandbox LANGUAGES CXX)

add_executable(Sandbox Sandbox.cpp)

target_compile_features(Sandbox PUBLIC cxx_std_17)
target_include_directories(Sandbox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

hyperion_configure_target(Sandbox)

# Enable assertions in Debug builds
target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:HYPERION_ENABLE_ASSERTS>
)

target_link_libraries(Sandbox PRIVATE Hyperion)

set_target_properties(Sandbox PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if (MINGW)
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)

    set(MINGW_RUNTIME_DLLS
            "libwinpthread-1.dll"
            "libstdc++-6.dll"
            "libgcc_s_seh-1.dll"
    )

    foreach (DLL_NAME ${MINGW_RUNTIME_DLLS})
        if (EXISTS "${MINGW_BIN_DIR}/${DLL_NAME}")
            add_custom_command(TARGET Sandbox POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_BIN_DIR}/${DLL_NAME}"
                    $<TARGET_FILE_DIR:Sandbox>
                    COMMENT "Copying ${DLL_NAME} to output directory"
            )
        endif ()
    endforeach ()
endif ()
