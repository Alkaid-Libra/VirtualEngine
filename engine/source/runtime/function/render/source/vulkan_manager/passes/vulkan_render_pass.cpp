#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"


VE::VVulkanContext* VE::VRenderPassBase::m_p_vulkan_context = nullptr;
VkDescriptorPool VE::VRenderPassBase::m_descriptor_pool = VK_NULL_HANDLE;
VE::VGlobalRenderResource* VE::VRenderPassBase::m_p_global_render_resource = nullptr;

VE::VRenderConfig VE::VRenderPassBase::m_render_config;
VE::VRenderCommandInfo VE::VRenderPassBase::m_command_info;

namespace VE
{
    void VRenderPassBase::setContext(const VRenderPassHelperInfo& helper_info)
    {
        m_p_vulkan_context = helper_info.p_context;
        m_descriptor_pool = helper_info.descriptor_pool;
        m_p_global_render_resource = helper_info.p_global_render_resource;
    }

    void VRenderPassBase::initialize()
    {
        assert(m_p_vulkan_context);
        assert(m_descriptor_pool != VK_NULL_HANDLE);
        assert(m_p_global_render_resource);
    }

    VkRenderPass VRenderPassBase::getRenderPass() { return _framebuffer.render_pass; }

    std::vector<VkImageView> VRenderPassBase::getFramebufferImageViews()
    {
        std::vector<VkImageView> image_views;
        for (auto& attach : _framebuffer.attachments)
        {
            image_views.push_back(attach.view);
        }
        return image_views;
    }

    std::vector<VkDescriptorSetLayout> VRenderPassBase::getDescriptorSetLayouts()
    {
        std::vector<VkDescriptorSetLayout> layouts;
        for(auto& desc : _descriptor_infos)
        {
            layouts.push_back(desc.layout);
        }
        return layouts;
    }
} // namespace VE
