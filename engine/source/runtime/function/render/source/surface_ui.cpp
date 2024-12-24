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

    
    // // implement init
    // ImGui_ImplGlfw_InitForVulkan(rhi->m_vulkan_manager->m_vulkan_context._window, true);
    // ImGui_ImplVulkan_InitInfo init_info = {};
    // init_info.Instance = rhi->m_vulkan_manager->m_vulkan_context._instance;
    // init_info.PhysicalDevice = rhi->m_vulkan_manager->m_vulkan_context._physical_device;
    // init_info.Device = rhi->m_vulkan_manager->m_vulkan_context._device;
    // init_info.QueueFamily = rhi->m_vulkan_manager->m_vulkan_context._queue_indices.graphicsFamily.value();
    // init_info.Queue = rhi->m_vulkan_manager->m_vulkan_context._graphics_queue;
    // init_info.DescriptorPool = rhi->m_vulkan_manager->m_descriptor_pool;
    // // init_info.RenderPass = renderPass; // Modify: if not dynamic render
    // init_info.RenderPass = rhi->m_vulkan_manager->getLightingPass(); // Modify: dynamic render
    // init_info.Subpass = 1;
    // // ?? may be different from the real swapchain image count // see: Imgui_ImplVulkanH_GetMinImageCountFromPresentMode
    // init_info.MinImageCount = rhi->m_vulkan_manager->m_max_frames_in_flight;
    // init_info.ImageCount = rhi->m_vulkan_manager->m_max_frames_in_flight;
    // // init_info.UseDynamicRendering = true; // !!enable dynamic rendering
    // ImGui_ImplVulkan_Init(&init_info);












    return 0;
}

    void createImGuiRenderPass() {
        VkAttachmentDescription attachment{};
        attachment.format = VK_FORMAT_B8G8R8A8_SRGB;
        attachment.samples = VK_SAMPLE_COUNT_1_BIT;
        attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachment{};
        colorAttachment.attachment = 0;
        colorAttachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachment;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &attachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        // if (vkCreateRenderPass(_device, &renderPassInfo, nullptr, &imguiRenderPass) != VK_SUCCESS) {
        //     throw std::runtime_error("Failed to create ImGui render pass!");
        // }
    }



void SurfaceUI::tick_pre(UIState* uistate)
{
    // if (m_rhi->m_vulkan_manager->m_frame_swapchain_image_acquired[m_rhi->m_vulkan_manager->m_current_frame_index])
    {
        m_tmp_uistate = uistate;
        // ImGui_ImplVulkan_NewFrame();
        // ImGui_ImplGlfw_NewFrame();
        // ImGui::NewFrame();
        // onTick(uistate);

        while(1)
        {
            
        }
    }
}

void SurfaceUI::tick_post(UIState* uistate) { draw_frame(); }

void SurfaceUI::draw_frame()
{
    if (true)
    {
        VkCommandBuffer current_command_buffer = m_rhi->m_vulkan_manager->getCurrentCommandBuffer();


        // ///////////////////
        // // Create a simple window
        // ImGui::Begin("Hello, ImGui!");
        // ImGui::Text("This is a simple window!");
        // ImGui::End();
        
        // ///////////////////

        // ImGui::Render();

        // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), current_command_buffer);

    }
}