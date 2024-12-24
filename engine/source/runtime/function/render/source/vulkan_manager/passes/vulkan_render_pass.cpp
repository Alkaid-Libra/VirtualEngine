#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"


VE::VVulkanContext* VE::VRenderPassBase::m_v_vulkan_context = nullptr;
VkDescriptorPool VE::VRenderPassBase::m_descriptor_pool = VK_NULL_HANDLE;
VE::VGlobalRenderResource* VE::VRenderPassBase::m_v_global_render_resource = nullptr;

namespace VE
{
    void VRenderPassBase::setContext(const VRenderPassHelperInfo& helper_info)
    {
        m_v_vulkan_context = helper_info.v_context;
        m_descriptor_pool = helper_info.descriptor_pool;
        m_v_global_render_resource = helper_info.v_global_render_resource;
    }
} // namespace VE
