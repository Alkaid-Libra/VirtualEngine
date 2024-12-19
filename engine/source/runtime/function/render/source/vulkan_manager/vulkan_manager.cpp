#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"
#include "runtime/function/render/include/render.h"


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