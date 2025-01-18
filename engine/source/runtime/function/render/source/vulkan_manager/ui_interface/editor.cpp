#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

namespace VE
{
    
    void VE::VVulkanManager::updateUIRenderSceneViewport(VkViewport render_scene_viewport)
    {
        m_viewport = render_scene_viewport;

        m_scissor.offset = {0, 0};
        m_scissor.extent = m_vulkan_context._swapchain_extent;
    }


    VkRenderPass VVulkanManager::getLightingPass() { return m_mesh_lighting_pass._framebuffer.render_pass; }
} // namespace VE
