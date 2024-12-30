#pragma once

#include "runtime/function/render/include/vulkan_manager/vulkan_global_resource.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"

#include <vector>
#include <vulkan/vulkan.h>

#include <glm/glm.hpp>

namespace VE
{
    static const uint32_t m_point_light_shadow_map_dimension = 2048;
    // static const uint32_t m_


    static uint32_t const m_max_point_light_count = 32;
    static uint32_t const m_mesh_per_drawcall_max_instance_count = 64;
    static uint32_t const m_mesh_vertex_blending_max_joint_count = 1024;




    struct VulkanScenePointLight
    {
        Vector3 position;
        float radius;
        Vector3 intensity;
        float _padding_intensity;
    };
    struct VulkanSceneDirectionalLight
    {
        Vector3 direction;
        float _padding_direction;
        Vector3 color;
        float _padding_color;
    };

    struct MeshPerframeStorageBufferObject
    {
        glm::mat4 proj_view_matrix;
        glm::mat3 camera_position;
        float _padding_camera_position;
        Vector3 ambient_light;
        float _padding_ambient_light;
        uint32_t point_light_num;
        uint32_t _padding_point_light_num_1;
        uint32_t _padding_point_light_num_2;
        uint32_t _padding_point_light_num_3;
        VulkanScenePointLight scene_point_lights[m_max_point_light_count];
        VulkanSceneDirectionalLight scene_directionnal_light;
        glm::mat4 directional_light_proj_view;
    };

    struct VulkanMeshInstance
    {
        float enable_vertex_blending;
        float _padding_enable_vertex_blending_1;
        float _padding_enable_vertex_blending_2;
        float _padding_enable_vertex_blending_3;
        glm::mat4 model_matrix;
    };
    struct MeshPerdrawcallStorageBufferObject
    {
        VulkanMeshInstance mesh_instances[m_mesh_per_drawcall_max_instance_count];
    };
    struct MeshPerdrawcallVertexBlendingStorageBufferObject
    {
        glm::mat4 joint_matrices[m_mesh_vertex_blending_max_joint_count * m_mesh_per_drawcall_max_instance_count];
    };

    struct AxisStorageBufferObject
    {
        glm::mat4 model_matrix = glm::mat4(1.0);
        uint32_t selected_axis = 3;
    };
} // namespace VE
