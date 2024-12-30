#include "runtime/function/render/include/surface.h"
#include "runtime/resource/config_manager/config_manager.h"


#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>


using namespace VE;

inline void windowContentScaleUpdate(float scale)
{
// #if defined(__MACH__)
//     float font_scale               = fmaxf(1.0f, scale);
//     ImGui::GetIO().FontGlobalScale = 1.0f / font_scale;
// #endif
// // TODO: Reload fonts if DPI scale is larger than previous font loading DPI scale    
}

inline void windowContentScaleCallback(GLFWwindow* window, float x_scale, float y_scale)
{
    windowContentScaleUpdate(fmaxf(x_scale, y_scale));
}


float SurfaceUI::getContentScale() const
{
    float x_scale, y_scale;
    glfwGetWindowContentScale(m_io->m_window, &x_scale, &y_scale);
    return fmaxf(1.0f, fmaxf(x_scale, y_scale));
}

float SurfaceUI::getIndentScale() const
{
#if defined(__MACH__)
    return 1.0f;
#else // Not tested on Linux
    return getContentScale();
#endif
}

int SurfaceUI::initialize(SurfaceRHI* rhi, VirtualRenderer* vrenderer, std::shared_ptr<SurfaceIO> vio)
{
    m_io = vio;
    m_rhi = rhi;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingAlwaysTabBar = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    float content_scale = getContentScale();
    windowContentScaleUpdate(content_scale);
    glfwSetWindowContentScaleCallback(vio->m_window, windowContentScaleCallback);

    
    // implement init
    ImGui_ImplGlfw_InitForVulkan(rhi->m_vulkan_manager->m_vulkan_context._window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = rhi->m_vulkan_manager->m_vulkan_context._instance;
    init_info.PhysicalDevice = rhi->m_vulkan_manager->m_vulkan_context._physical_device;
    init_info.Device = rhi->m_vulkan_manager->m_vulkan_context._device;
    init_info.QueueFamily = rhi->m_vulkan_manager->m_vulkan_context._queue_indices.graphicsFamily.value();
    init_info.Queue = rhi->m_vulkan_manager->m_vulkan_context._graphics_queue;
    init_info.DescriptorPool = rhi->m_vulkan_manager->m_descriptor_pool;
    // init_info.RenderPass = rhi->m_vulkan_manager->m_triangle._framebuffer.render_pass;// renderPass; // Modify: if not dynamic render
    init_info.RenderPass = rhi->m_vulkan_manager->getLightingPass(); // Modify: if not dynamic render
    init_info.Subpass = 1;
    // ?? may be different from the real swapchain image count // see: Imgui_ImplVulkanH_GetMinImageCountFromPresentMode
    init_info.MinImageCount = rhi->m_vulkan_manager->m_max_frames_in_flight;
    init_info.ImageCount = rhi->m_vulkan_manager->m_max_frames_in_flight;
    // init_info.UseDynamicRendering = true; // !!enable dynamic rendering
    ImGui_ImplVulkan_Init(&init_info);

    // fonts upload
    fontsUpload(rhi);










    return 0;
}

void SurfaceUI::fontsUpload(SurfaceRHI* rhi)
{
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool                 = rhi->m_vulkan_manager->m_vulkan_context._command_pool;
    allocInfo.commandBufferCount          = 1;

    VkCommandBuffer commandBuffer = {};
    if (vkAllocateCommandBuffers(rhi->m_vulkan_manager->m_vulkan_context._device, &allocInfo, &commandBuffer) !=
        VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags                    = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Could not create one-time command buffer!");
    }

    ImGui_ImplVulkan_CreateFontsTexture();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }

    VkSubmitInfo submitInfo {};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &commandBuffer;

    vkQueueSubmit(rhi->m_vulkan_manager->m_vulkan_context._graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(rhi->m_vulkan_manager->m_vulkan_context._graphics_queue);

    vkFreeCommandBuffers(rhi->m_vulkan_manager->m_vulkan_context._device,
                         rhi->m_vulkan_manager->m_vulkan_context._command_pool,
                         1,
                         &commandBuffer);

    // ImGui_ImplVulkan_DestroyFontUploadObjects();
    // ImGui_ImplVulkan_DestroyFontsTexture();
}




void SurfaceUI::tick_pre(UIState* uistate)
{
    // if (m_rhi->m_vulkan_manager->m_frame_swapchain_image_acquired[m_rhi->m_vulkan_manager->m_current_frame_index])
    {
        m_tmp_uistate = uistate;
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // onTick(uistate);

        // while(1)
        // {
            
        // }
    }
}

void SurfaceUI::tick_post(UIState* uistate) { draw_frame(); }

void SurfaceUI::draw_frame()
{
    // if (m_rhi->m_vulkan_manager->m_frame_swapchain_image_acquired[m_rhi->m_vulkan_manager->m_current_frame_index])
    if (1)
    {
        VkCommandBuffer current_command_buffer = m_rhi->m_vulkan_manager->getCurrentCommandBuffer();

        if (m_rhi->m_vulkan_manager->m_enable_debug_utils_label)
        {
            VkDebugUtilsLabelEXT label_info = {
                VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT, NULL, "ImGUI", {1.0f, 1.0f, 1.0f, 1.0f}};
            m_rhi->m_vulkan_manager->m_vulkan_context._vkCmdBeginDebugUtilsLabelEXT(current_command_buffer,
                                                                                    &label_info);
        }

        // Create a simple window
        ImGui::Begin("Hello, ImGui!");
        ImGui::Text("This is a simple window!");
        ImGui::End();


        ImGui::Render();

        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), current_command_buffer);

        if (m_rhi->m_vulkan_manager->m_enable_debug_utils_label)
        {
            m_rhi->m_vulkan_manager->m_vulkan_context._vkCmdEndDebugUtilsLabelEXT(current_command_buffer);
        }
    }




    // if (true)
    // {
    //     VkCommandBuffer current_command_buffer = m_rhi->m_vulkan_manager->getCurrentCommandBuffer();


    //     // ///////////////////
    //     // // Create a simple window
    //     // ImGui::Begin("Hello, ImGui!");
    //     // ImGui::Text("This is a simple window!");
    //     // ImGui::End();
        
    //     // ///////////////////

    //     // ImGui::Render();

    //     // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), current_command_buffer);

    // }
}