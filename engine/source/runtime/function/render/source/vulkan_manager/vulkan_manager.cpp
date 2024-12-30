#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"
#include "runtime/function/render/include/render.h"

// TODO: remove this as the vmaAllocator
uint32_t VE::VVulkanManager::m_max_vertex_blending_mesh_count = 256;
uint32_t VE::VVulkanManager::m_max_material_count             = 256;

#ifndef NDEBUG
bool VE::VVulkanManager::m_enable_validation_layers = true;
bool VE::VVulkanManager::m_enable_debug_utils_label = true;
#else
bool VE::VVulkanManager::m_enable_validation_layers = true;
bool VE::VVulkanManager::m_enable_debug_utils_label = true;
#endif

#if defined(__GNUC__)
bool VE::VVulkanManager::m_enable_point_light_shadow = false;
#else
bool VE::VVulkanManager::m_enable_point_light_shadow = true;
#endif



// initialize vulkan from io->window
VE::VVulkanManager::VVulkanManager(GLFWwindow* window, class Scene& scene, VirtualRenderer* virtual_renderer)
{
    initialize(window, scene, virtual_renderer);
}

void VE::VVulkanManager::beginFrame()
{

    // reset storage buffer offset

    // wait for fences (stop CPU until GPU finish render operation in last frame)
    VkResult res_wait_for_fences = m_vulkan_context._vkWaitForFences(
        m_vulkan_context._device, 1, &m_is_frame_in_flight_fences[m_current_frame_index], VK_TRUE, UINT64_MAX);
    assert(VK_SUCCESS == res_wait_for_fences);
    // wait for get next image from swapchain
    VkResult acquire_image_result =
        vkAcquireNextImageKHR(m_vulkan_context._device,
                              m_vulkan_context._swapchain,
                              UINT64_MAX,
                              m_image_available_for_render_semaphores[m_current_frame_index],
                              VK_NULL_HANDLE,
                              &m_current_swapchain_image_index);  

    // reset command pool
    VkResult res_reset_command_pool =
        m_vulkan_context._vkResetCommandPool(m_vulkan_context._device, m_command_pools[m_current_frame_index], 0);

    // begin command buffer
    VkCommandBufferBeginInfo command_buffer_begin_info{};
    command_buffer_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    command_buffer_begin_info.flags = 0;
    command_buffer_begin_info.pInheritanceInfo = nullptr;

    VkResult res_begin_command_buffer = 
        m_vulkan_context._vkBeginCommandBuffer(m_command_buffers[m_current_frame_index], &command_buffer_begin_info);
    assert(VK_SUCCESS == res_begin_command_buffer);



    // Lighting begin pass
    {
        VkRenderPassBeginInfo renderpass_begin_info{};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderPass = m_mesh_lighting_pass._framebuffer.render_pass;
        renderpass_begin_info.framebuffer = m_swapchain_framebuffer[m_current_swapchain_image_index];
        renderpass_begin_info.renderArea.offset = {0, 0};
        renderpass_begin_info.renderArea.extent = m_vulkan_context._swapchain_extent;

        VkClearValue clear_values[3];
        clear_values[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_values[1].depthStencil = {1.0f, 0};
        clear_values[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        renderpass_begin_info.clearValueCount = sizeof(clear_values) / sizeof(clear_values[0]);
        renderpass_begin_info.pClearValues = clear_values;

        m_vulkan_context._vkCmdBeginRenderPass(
            m_command_buffers[m_current_frame_index], &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
    }

    m_vulkan_context._vkCmdNextSubpass(m_command_buffers[m_current_frame_index], VK_SUBPASS_CONTENTS_INLINE);


}
void VE::VVulkanManager::endFrame()
{
    // if (m_frame_swapchain_image_acquired[m_current_frame_index])
    // {
        // Light end pass
        {
            m_vulkan_context._vkCmdEndRenderPass(m_command_buffers[m_current_frame_index]);
        }

        // end command buffer
        VkResult res_end_command_buffer =
            m_vulkan_context._vkEndCommandBuffer(m_command_buffers[m_current_frame_index]);
        assert(VK_SUCCESS == res_end_command_buffer);




        // submit commands
        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        VkSubmitInfo submit_info = {};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &m_image_available_for_render_semaphores[m_current_frame_index];
        submit_info.pWaitDstStageMask = wait_stages;

        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &m_command_buffers[m_current_frame_index];
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &m_image_finished_for_presentation_semaphores[m_current_frame_index];

        VkResult res_reset_fences = m_vulkan_context._vkResetFences(
            m_vulkan_context._device, 1, &m_is_frame_in_flight_fences[m_current_frame_index]);
        assert(VK_SUCCESS == res_reset_fences);

        // submit queue, when finish render, GPU signal the fence of the frame
        VkResult res_queue_submit = vkQueueSubmit(
            m_vulkan_context._graphics_queue, 1, &submit_info, m_is_frame_in_flight_fences[m_current_frame_index]);
        assert(VK_SUCCESS == res_queue_submit);

        // present
        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &m_image_finished_for_presentation_semaphores[m_current_frame_index];
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &m_vulkan_context._swapchain;
        present_info.pImageIndices = &m_current_swapchain_image_index;

        VkResult present_result = vkQueuePresentKHR(m_vulkan_context._present_queue, &present_info);

    // }

    // m_current_frame_index = (m_current_frame_index + 1) % m_max_frames_in_flight;

}




// ///////////////////
// void VE::VVulkanManager::beginFrame()
// {
//     // reset storage buffer offset
//     m_global_render_resource._storage_buffer._global_upload_ringbuffers_end[m_current_frame_index] =
//         m_global_render_resource._storage_buffer._global_upload_ringbuffers_begin[m_current_frame_index];

//     VkResult res_wait_for_fences = m_vulkan_context._vkWaitForFences(
//         m_vulkan_context._device, 1, &m_is_frame_in_flight_fences[m_current_frame_index], VK_TRUE, UINT64_MAX);
//     assert(VK_SUCCESS == res_wait_for_fences);

//     VkResult res_reset_command_pool =
//         m_vulkan_context._vkResetCommandPool(m_vulkan_context._device, m_command_pools[m_current_frame_index], 0);
//     assert(VK_SUCCESS == res_reset_command_pool);

//     VkResult acquire_image_result =
//         vkAcquireNextImageKHR(m_vulkan_context._device,
//                               m_vulkan_context._swapchain,
//                               UINT64_MAX,
//                               m_image_available_for_render_semaphores[m_current_frame_index],
//                               VK_NULL_HANDLE,
//                               &m_current_swapchain_image_index);

//     if (acquire_image_result == VK_ERROR_OUT_OF_DATE_KHR)
//     {
//         recreateSwapChain();
//         m_frame_swapchain_image_acquired[m_current_frame_index] = false;
//         return;
//     }
//     else if (acquire_image_result == VK_SUBOPTIMAL_KHR)
//     {
//         recreateSwapChain();
//         m_frame_swapchain_image_acquired[m_current_frame_index] = false;
//         return;
//     }
//     else
//     {
//         m_frame_swapchain_image_acquired[m_current_frame_index] = true;
//         assert(VK_SUBOPTIMAL_KHR == acquire_image_result || VK_SUCCESS == acquire_image_result);
//     }

//     VkCommandBufferBeginInfo command_buffer_begin_info {};
//     command_buffer_begin_info.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//     command_buffer_begin_info.flags            = 0;
//     command_buffer_begin_info.pInheritanceInfo = nullptr;

//     VkResult res_begin_command_buffer =
//         m_vulkan_context._vkBeginCommandBuffer(m_command_buffers[m_current_frame_index], &command_buffer_begin_info);
//     assert(VK_SUCCESS == res_begin_command_buffer);

//     VRenderPassBase::m_command_info._current_frame_index       = m_current_frame_index;
//     VRenderPassBase::m_command_info._p_current_frame_index     = &m_current_frame_index;
//     VRenderPassBase::m_command_info._current_command_buffer    = m_command_buffers[m_current_frame_index];
//     VRenderPassBase::m_command_info._p_current_command_buffer  = m_command_buffers;
//     VRenderPassBase::m_command_info._current_command_pool      = m_command_pools[m_current_frame_index];
//     VRenderPassBase::m_command_info._p_command_pools           = m_command_pools;
//     VRenderPassBase::m_command_info._current_fence             = m_is_frame_in_flight_fences[m_current_frame_index];
//     VRenderPassBase::m_command_info._viewport                  = m_viewport;
//     VRenderPassBase::m_command_info._scissor                   = m_scissor;
//     VRenderPassBase::m_command_info._max_frames_in_flight      = m_max_frames_in_flight;
//     VRenderPassBase::m_command_info._is_frame_in_flight_fences = m_is_frame_in_flight_fences;

//     // VRenderPassBase::m_visiable_nodes.p_directional_light_visible_mesh_nodes = &m_directional_light_visible_mesh_nodes;
//     // VRenderPassBase::m_visiable_nodes.p_point_lights_visible_mesh_nodes      = &m_point_lights_visible_mesh_nodes;
//     // VRenderPassBase::m_visiable_nodes.p_main_camera_visible_mesh_nodes       = &m_main_camera_visible_mesh_nodes;
//     // VRenderPassBase::m_visiable_nodes.p_axis_node                            = &m_axis_node;
//     // VRenderPassBase::m_visiable_nodes.p_main_camera_visible_particlebillboard_nodes =
//     //     &m_main_camera_visible_particlebillboard_nodes;

//     // m_directional_light_pass.draw();

//     // m_point_light_pass.draw();

//     // // Lighting begin pass
//     // {
//     //     VkRenderPassBeginInfo renderpass_begin_info {};
//     //     renderpass_begin_info.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//     //     renderpass_begin_info.renderPass        = m_mesh_lighting_pass._framebuffer.render_pass;
//     //     renderpass_begin_info.framebuffer       = m_swapchain_framebuffers[m_current_swapchain_image_index];
//     //     renderpass_begin_info.renderArea.offset = {0, 0};
//     //     renderpass_begin_info.renderArea.extent = m_vulkan_context._swapchain_extent;

//     //     VkClearValue clear_values[3];
//     //     clear_values[0].color                 = {{0.0f, 0.0f, 0.0f, 1.0f}};
//     //     clear_values[1].depthStencil          = {1.0f, 0};
//     //     clear_values[2].color                 = {{0.0f, 0.0f, 0.0f, 1.0f}};
//     //     renderpass_begin_info.clearValueCount = (sizeof(clear_values) / sizeof(clear_values[0]));
//     //     renderpass_begin_info.pClearValues    = clear_values;

//     //     m_vulkan_context._vkCmdBeginRenderPass(
//     //         m_command_buffers[m_current_frame_index], &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
//     // }

//     // m_mesh_lighting_pass.draw();

//     // m_vulkan_context._vkCmdNextSubpass(m_command_buffers[m_current_frame_index], VK_SUBPASS_CONTENTS_INLINE);

//     // m_postprocess_pass.draw();
// }

// void Pilot::PVulkanManager::endFrame()
// {
//     if (m_frame_swapchain_image_acquired[m_current_frame_index])
//     {
//         // Lighting end pass
//         {
//             m_vulkan_context._vkCmdEndRenderPass(m_command_buffers[m_current_frame_index]);
//         }

//         // end command buffer
//         VkResult res_end_command_buffer =
//             m_vulkan_context._vkEndCommandBuffer(m_command_buffers[m_current_frame_index]);
//         assert(VK_SUCCESS == res_end_command_buffer);

//         // submit commands
//         VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
//         VkSubmitInfo         submit_info   = {};
//         submit_info.sType                  = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//         submit_info.waitSemaphoreCount     = 1;
//         submit_info.pWaitSemaphores        = &m_image_available_for_render_semaphores[m_current_frame_index];
//         submit_info.pWaitDstStageMask      = wait_stages;
//         submit_info.commandBufferCount     = 1;
//         submit_info.pCommandBuffers        = &m_command_buffers[m_current_frame_index];
//         submit_info.signalSemaphoreCount   = 1;
//         submit_info.pSignalSemaphores      = &m_image_finished_for_presentation_semaphores[m_current_frame_index];

//         VkResult res_reset_fences = m_vulkan_context._vkResetFences(
//             m_vulkan_context._device, 1, &m_is_frame_in_flight_fences[m_current_frame_index]);
//         assert(VK_SUCCESS == res_reset_fences);

//         VkResult res_queue_submit = vkQueueSubmit(
//             m_vulkan_context._graphics_queue, 1, &submit_info, m_is_frame_in_flight_fences[m_current_frame_index]);
//         assert(VK_SUCCESS == res_queue_submit);

//         // present
//         VkPresentInfoKHR present_info   = {};
//         present_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//         present_info.waitSemaphoreCount = 1;
//         present_info.pWaitSemaphores    = &m_image_finished_for_presentation_semaphores[m_current_frame_index];
//         present_info.swapchainCount     = 1;
//         present_info.pSwapchains        = &m_vulkan_context._swapchain;
//         present_info.pImageIndices      = &m_current_swapchain_image_index;

//         VkResult present_result = vkQueuePresentKHR(m_vulkan_context._present_queue, &present_info);
//         // check if swapchain out of date
//         if (present_result == VK_ERROR_OUT_OF_DATE_KHR)
//         {
//             recreateSwapChain();
//         }
//         else if (present_result == VK_SUBOPTIMAL_KHR)
//         {
//             recreateSwapChain();
//         }
//         else
//         {
//             assert(VK_SUBOPTIMAL_KHR == present_result || VK_SUCCESS == present_result);
//         }
//     }

//     m_current_frame_index = (m_current_frame_index + 1) % m_max_frames_in_flight;
// }