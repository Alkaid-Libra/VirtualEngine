#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

int VE::VVulkanManager::initialize(GLFWwindow* window, class Scene& scene, VirtualRenderer* virtual_renderer)
{
    m_vulkan_context.initialize(window);

    m_global_render_resource.initialize(m_vulkan_context, m_max_frames_in_flight);

    VRenderPassBase::m_render_config._enable_debug_utils_label = m_enable_debug_utils_label;
    VRenderPassBase::m_render_config._enable_validation_layers = m_enable_validation_layers;
    VRenderPassBase::m_render_config._enable_point_light_shadow = m_enable_point_light_shadow;

    VRenderPassBase::m_command_info._viewport = m_viewport;
    VRenderPassBase::m_command_info._scissor = m_scissor;

    // global textures for IBL


    if (initializeCommandPool() && initializeCommandBuffers() && initializeDescriptorPool() && createSyncPrimitives() &&
        initializeRenderPass() && initializeSwapchainFramebuffers())
        return 1;
    else
        return 0;
}
