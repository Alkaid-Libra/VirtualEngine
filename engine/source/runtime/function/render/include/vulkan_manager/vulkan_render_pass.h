#pragma once

#include "runtime/function/render/include/vulkan_manager/vulkan_global_resource.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"
// #include 

#include <vector>
#include <vulkan/vulkan.h>

namespace VE
{
    struct VRenderPassHelperInfo
    {
        VVulkanContext* p_context;
        VkDescriptorPool descriptor_pool;
        VGlobalRenderResource* p_global_render_resource;
    };

    struct VRenderConfig
    {
        bool _enable_validation_layers;
        bool _enable_debug_utils_label;
        bool _enable_point_light_shadow;
    };

    struct VRenderCommandInfo
    {
        uint32_t _current_frame_index;
        uint32_t* _p_current_frame_index;
        VkCommandBuffer _current_command_buffer;
        VkCommandBuffer* _p_current_command_buffer;
        VkCommandPool _current_command_pool;
        VkCommandPool* _p_command_pools;
        VkFence _current_fence;
        VkViewport _viewport;
        VkRect2D _scissor;
        int _max_frames_in_flight;
        VkFence* _is_frame_in_flight_fences;
    };

    class VRenderPassBase
    {
    public:
        struct FrameBufferAttachment
        {
            VkImage image;
            VkDeviceMemory mem;
            VkImageView view;
            VkFormat format;
        };

        struct Framebuffer
        {
            int width;
            int height;
            VkFramebuffer framebuffer;
            VkRenderPass render_pass;

            std::vector<FrameBufferAttachment> attachments;
        };

        struct Descriptor
        {
            VkDescriptorSetLayout layout;
            VkDescriptorSet descriptor_set;
        };

        struct RenderPipeline
        {
            VkPipelineLayout layout;
            VkPipeline pipeline;
        };

        std::vector<Descriptor> _descriptor_infos;
        std::vector<RenderPipeline> _render_pipelines;
        Framebuffer _framebuffer;

        static void setContext(const VRenderPassHelperInfo& helper_info);

        virtual void initialize();
        // virtual void draw();
        // virtual void postInitialize();

        virtual VkRenderPass getRenderPass();
        virtual std::vector<VkImageView> getFramebufferImageViews();
        virtual std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

        static VRenderConfig m_render_config;
        static VRenderCommandInfo m_command_info;

    protected:
        static VVulkanContext* m_p_vulkan_context;
        static VkDescriptorPool m_descriptor_pool;
        static VGlobalRenderResource* m_p_global_render_resource;
    };
} // namespace VE
