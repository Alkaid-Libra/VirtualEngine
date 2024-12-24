#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"
#include "runtime/function/render/include/render.h"

// TODO: remove this as the vmaAllocator
uint32_t VE::VVulkanManager::m_max_vertex_blending_mesh_count = 256;
uint32_t VE::VVulkanManager::m_max_material_count             = 256;

#ifndef NDEBUG
bool VE::VVulkanManager::m_enable_validation_layers = true;
bool VE::VVulkanManager::m_enable_debug_utils_label = true;
#else
bool VE::VVulkanManager::m_enable_validation_layers = true;
bool VE::VVulkanManager::m_enable_debug_utils_label = true;
#endif



// initialize vulkan from io->window
VE::VVulkanManager::VVulkanManager(GLFWwindow* window, class Scene& scene, VirtualRenderer* virtual_renderer)
{
    initialize(window, scene, virtual_renderer);
}