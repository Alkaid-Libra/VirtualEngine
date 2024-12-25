set(TARGET_NAME VirtualRuntime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(json_include ${THIRD_PARTY_DIR}/json11)
# set(json_lib_dir ${THIRD_PARTY_DIR}/lib/json11)
add_library(json11 ${json_include}/json11.cpp)
set_target_properties(json11 PROPERTIES FOLDER ${third_party_folder}/json11)

file(GLOB_RECURSE HEADER_FILES "*.h")
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${HEADER_FILES} ${SOURCE_FILES})

add_library(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})
# add_executable(${TARGET_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17)
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Engine")

# Link dependencies
target_link_libraries(${TARGET_NAME} PUBLIC ${vulkan_lib})
target_link_libraries(${TARGET_NAME} PUBLIC glm)
target_link_libraries(${TARGET_NAME} PUBLIC glfw)
target_link_libraries(${TARGET_NAME} PUBLIC imgui)
target_link_libraries(${TARGET_NAME} PRIVATE tinyobjloader stb)
target_link_libraries(${TARGET_NAME} PUBLIC spdlog::spdlog)
target_link_libraries(${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:json11>)

# Include
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${vulkan_include}>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${THIRD_PARTY_DIR}/vulkanmemoryallocator/include>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${json_include}>)

target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source>)
target_include_directories(${TARGET_NAME} PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source/runtime>)


# compile shader to spv
# add_dependencies(${TARGET_NAME} ${SHADER_COMPILE_TARGET})

# precompile
# set global vari used by precompile
set(VIRTUAL_RUNTIME_HEADS "${HEADER_FILES}" PARENT_SCOPE)


# target_include_directories(${TARGET_NAME} 
#     PUBLIC $<BUILD_INTERFACE:${ENGINE_ROOT_DIR}/source/runtime/function/render/include>
#     $<INSTALL_INTERFACE:include/${TARGET_NAME}-${PROJECT_VERSION}>)

