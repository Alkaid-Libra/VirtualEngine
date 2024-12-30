#pragma once

#include <GLFW/glfw3.h>

// 
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <vulkan/vulkan.h>
#include "runtime/function/render/include/surface.h"

namespace VE
{







    
    class SurfaceIO
    {


    public:

        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        const char* getTitle() const { return m_title; }



        GLFWwindow* m_window{nullptr};

        const char* m_title;
        uint32_t m_width;
        uint32_t m_height;

        SurfaceIO(const char* title, uint32_t width, uint32_t height)
        {
            m_width = width;
            m_height = height;
            m_title = title;
        }




        int initialize()
        {
            if (!glfwInit())
            {
                return -1;
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            m_window = glfwCreateWindow(getWidth(), getHeight(), getTitle(), NULL, NULL);
            // m_window = glfwCreateWindow(1080, 960, getTitle(), NULL, NULL);
            if (!m_window)
            {
                glfwTerminate();
                return -1;
            }

            // Setup input callbacks


            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            return 0;
        }
        bool tick_NotQuit()
        {
            while (!glfwWindowShouldClose(m_window))
            {
                glfwPollEvents();
                return true;

                // // 
                // // Start the Dear ImGui frame
                // ImGui_ImplVulkan_NewFrame();
                // ImGui_ImplGlfw_NewFrame();
                // ImGui::NewFrame();

                // ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                // ImGui::Text("This is some useful text.");
                // ImGui::End();







                // VkCommandBuffer current_command_buffer;// = m_rhi->m_vulkan_manager->getCurrentCommandBuffer();

                // // if (m_rhi->m_vulkan_manager->m_enable_debug_untils_label)
                // // {
                //     VkDebugUtilsLabelEXT label_info = {
                //         VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT, NULL, "ImGUI", {1.0f, 1.0f, 1.0f, 1.0f}};
                //     m_rhi->m_vulkan_manager->_vkCmdBeginDebugUtilsLabelEXT(current_command_buffer,
                //                                                                             &label_info);
                // // }
                // // Rendering
                // ImGui::Render();
                // // VkCommandBuffer commandBuffer = beginFrame();

                // ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), current_command_buffer);

                // // endFrame(commandBuffer);

            }
            return false;
        }
        int clear()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            return 0;
        }
    };
} // namespace VE
