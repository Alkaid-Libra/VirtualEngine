#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"

// semaphore: signal an image is ready for rendering // ready for presentation
// (m_vulkan_context._swapchain_images --> semaphores, fences)
bool VE::VVulkanManager::createSyncPrimitives()
{
    VkSemaphoreCreateInfo semaphore_create_info{};
    semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fence_create_info{};
    fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // the fence is initialized as signaled
}