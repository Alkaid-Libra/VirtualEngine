#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

int VE::VVulkanManager::initialize(GLFWwindow* window, class Scene& scene, VirtualRenderer* virtual_renderer)
{
    m_vulkan_context.initialize(window);
}
