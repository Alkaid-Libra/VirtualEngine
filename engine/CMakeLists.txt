set(ENGINE_ROOT_DIR "${VIRTUALENGINE_ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/3rdparty")
set(ENGINE_ASSET_DIR "/asset")
set(ENGINE_SCHEMA_DIR "/schema")

set(vulkan_include ${THIRD_PARTY_DIR}/VulkanSDK/include)

if(UNIX)
    if(APPLE)

    else()
        set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Linux/libvulkan.so.1)
        set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Linux/glslangValidator)
        add_compile_definitions("VIRTUALENGINE_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Linux")
    endif()
else()
    message(FATAL_ERROR "Unknown Platform")
endif()

set(SHADER_COMPILE_TARGET VirtualEngineShaderCompile)
add_subdirectory(shader)

add_subdirectory(3rdparty)

add_subdirectory(source/runtime)
add_subdirectory(source/editor)
# add_subdirectory(source/test)

set(CODEGEN_TARGET "VirtualEnginePreCompile")
# include(source/precompile/precompile.cmake)
# set_target_properties("${CODEGEN_TARGET}" PROPERTIES FOLDER "Engine")

# add_dependencies(VirtualRuntime "${CODEGEN_TARGET}")