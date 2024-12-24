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
        VVulkanContext* v_context;
        VkDescriptorPool descriptor_pool;
        VGlobalRenderResource* v_global_render_resource;
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

        // virtual void initialize();
        // virtual void draw();
        // virtual void postInitialize();

        // virtual VkRenderPass getRenderPass();
        // virtual std::vector<VkImageView> getFramebufferImageViews();
        // virtual std::vector<VkDescriptorSetLayout> getDescriptorSetLayouts();

    protected:
        static VVulkanContext* m_v_vulkan_context;
        static VkDescriptorPool m_descriptor_pool;
        static VGlobalRenderResource* m_v_global_render_resource;
    };
} // namespace VE
