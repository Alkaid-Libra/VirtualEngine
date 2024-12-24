#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN 1
#endif

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS 1
#endif

#ifndef GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
#endif

#include <GLFW/glfw3.h>

#include "vulkan_context.h"
#include "vulkan_passes.h"
// #include "vulkan_gl"

namespace VE
{
    class VVulkanManager
    {
    public:
        VVulkanManager(GLFWwindow* window, class Scene& scene, class VirtualRenderer* virtual_renderer);
        // clear module resource
        void clear();
        // frame start
        void beginFrame();
        // frame end
        void endFrame();
        // per frame synchronization
        void
        syncScene(class Scene& scene, class VirtualRenderer* virtual_renderer, struct SceneReleaseHandles& release_handles);
        // get visiable objects
        void
        culling(class Scene& scene, class VirtualRenderer* virtual_renderer, struct SceneReleaseHandles& release_handles);

    private:
        // initialization
        int initialize(GLFWwindow* window, class Scene& scene, class VirtualRenderer* virtual_renderer);
        // initialize render passes
        bool initializeRenderPass();
        // initialize command pool
        bool initializeCommandPool();
        // initialize swapchain frame buffers
        bool initializeSwapchainFramebuffers();
        // description pool for uniform buffer and image sampler
        bool initializeDescriptorPool();
        // semaphore : signal an image is ready for rendering / presentation
        bool createSyncPrimitives();
        // allocate command buffer : for drawing commands
        bool initializeCommandBuffers();
        // swapchain clear or recreate
        void clearSwapChain();
        // recreate swapchain
        bool recreateSwapChain();

    public:
        // vulkan cotext include device create, default command buffer, etc
        VVulkanContext m_vulkan_context;


    private:
        // global rendering resource, include IBL data, global storage buffer
        VGlobalRenderResource m_global_render_resource;
        // include lighting, shadow, post process, mouse picking pass
        VLightingPass m_mesh_lighting_pass;
        VDirectionalLightPass m_directional_light_pass;
        VPointLightPass m_point_light_pass;
        VPostprocessPass m_postprocess_pass;
        VPickPass m_mouse_pick_pass;


    public:
        

        VkCommandBuffer getCurrentCommandBuffer();
        VkRenderPass getLightingPass();

        // rendering config
        static bool m_enable_validation_layers;
        static bool m_enable_debug_utils_label;
        static bool m_enable_point_light_shadow;

        static uint32_t const m_max_frames_in_flight = 3;
        uint32_t m_current_frame_index = 0;
        bool m_frame_swapchain_image_acquired[m_max_frames_in_flight];

        // global descriptor pool
        VkDescriptorPool m_descriptor_pool;


        static uint32_t m_max_vertex_blending_mesh_count;
        static uint32_t m_max_material_count;

        VkCommandPool m_command_pools[m_max_frames_in_flight];
        VkCommandBuffer m_command_buffers[m_max_frames_in_flight];
        VkSemaphore m_image_available_for_render_semaphores[m_max_frames_in_flight];
        VkSemaphore m_image_finished_for_presentation_semaphores[m_max_frames_in_flight];
        VkFence m_is_frame_in_flight_fences[m_max_frames_in_flight];




    private:
        
        uint32_t m_current_swapchain_image_index = 0;
        // std::vector<VkFramebuffer> m_swapchain_framebuffer;

        // 
        public:
        std::vector<VkFramebuffer> m_swapchain_framebuffer;
    };
} // namespace VE
