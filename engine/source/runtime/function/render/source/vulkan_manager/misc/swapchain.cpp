#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

// bool VE::VVulkanManager::initializeSwapchainFramebuffers()
// {
//     m_swapchain_framebuffer.resize(m_vulkan_context._swapchain_imageviews.size());

//     // create frame buffer for every imageview
//     for (size_t i = 0; i < m_vulkan_context._swapchain_imageviews.size(); i++)
//     {
//         // first image view is used in post process
//         // VkImageView frambuffer_attachments_for_image_view[3] = 
//         VkImageView attachments[] = {m_vulkan_context._swapchain_imageviews[i]};

//         VkFramebufferCreateInfo framebufferInfo = {};
//         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//         framebufferInfo.renderPass =  
//     }
// }