#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

// 
#include <stdexcept>


bool VE::VVulkanManager::initializeSwapchainFramebuffers()
{
    m_swapchain_framebuffer.resize(m_vulkan_context._swapchain_imageviews.size());

    // create frame buffer for every imageview
    for (size_t i = 0; i < m_vulkan_context._swapchain_imageviews.size(); i++)
    {
        // first image view is used in post process
        VkImageView frambuffer_attachments_for_image_view[3] = {m_postprocess_pass._framebuffer.attachments[0].view,
                                                                m_vulkan_context._depth_image_view,
                                                                m_vulkan_context._swapchain_imageviews[i]}; 

        VkFramebufferCreateInfo framebuffer_create_info = {};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.flags = 0U;
        framebuffer_create_info.renderPass = m_mesh_lighting_pass._framebuffer.render_pass;
        framebuffer_create_info.attachmentCount =
            sizeof(frambuffer_attachments_for_image_view) / sizeof(frambuffer_attachments_for_image_view[0]);
        framebuffer_create_info.pAttachments = frambuffer_attachments_for_image_view;
        framebuffer_create_info.width = m_vulkan_context._swapchain_extent.width;
        framebuffer_create_info.height = m_vulkan_context._swapchain_extent.height;
        framebuffer_create_info.layers = 1;

        if (vkCreateFramebuffer(m_vulkan_context._device, &framebuffer_create_info, nullptr, &m_swapchain_framebuffer[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("create normal framebuffer");
        }
    }

    return true;
}