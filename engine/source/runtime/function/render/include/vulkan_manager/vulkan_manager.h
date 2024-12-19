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


    public:
        


        // rendering config
        static bool m_enable_validation_layers;
        static bool m_enable_debug_utils_label;
        static bool m_enable_point_light_shadow;
    };
} // namespace VE
