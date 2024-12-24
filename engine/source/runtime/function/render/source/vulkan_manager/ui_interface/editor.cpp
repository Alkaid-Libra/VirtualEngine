#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

namespace VE
{
    


    VkRenderPass VVulkanManager::getLightingPass() { return m_mesh_lighting_pass._framebuffer.render_pass; }
} // namespace VE
