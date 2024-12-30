#include "runtime/function/render/include/vulkan_manager/vulkan_point_light_pass.h"

#include "runtime/function/render/include/vulkan_manager/vulkan_common.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_util.h"

// 
#include <stdexcept>




namespace VE
{
    void VPointLightPass::initialize()
    {
        setupAttachments();
        setupRenderPass();
        setupFramebuffer();
        setupDescriptorSetLayout();
    }

    void VPointLightPass::postInitialize()
    {
        setupPipelines();
        setupDescriptorSet();
    }


    void VPointLightPass::setupAttachments()
    {
        // color and depth
        _framebuffer.attachments.resize(2);

        // color
        _framebuffer.attachments[0].format = VK_FORMAT_R32_SFLOAT;
        VVulkanUtil::createImage(m_p_vulkan_context->_physical_device,
                                 m_p_vulkan_context->_device,
                                 m_point_light_shadow_map_dimension,
                                 m_point_light_shadow_map_dimension,
                                 _framebuffer.attachments[0].format,
                                 VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                 _framebuffer.attachments[0].image,
                                 _framebuffer.attachments[0].mem,
                                 0,
                                 2 * m_max_point_light_count,
                                 1);
        _framebuffer.attachments[0].view = VVulkanUtil::createImageView(m_p_vulkan_context->_device,
                                                                        _framebuffer.attachments[0].image,
                                                                        _framebuffer.attachments[0].format,
                                                                        VK_IMAGE_ASPECT_COLOR_BIT,
                                                                        VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                                                                        2 * m_max_point_light_count,
                                                                        1);

        // depth
        _framebuffer.attachments[1].format = m_p_vulkan_context->_depth_image_format;
        VVulkanUtil::createImage(m_p_vulkan_context->_physical_device,
                                 m_p_vulkan_context->_device,
                                 m_point_light_shadow_map_dimension,
                                 m_point_light_shadow_map_dimension,
                                 _framebuffer.attachments[1].format,
                                 VK_IMAGE_TILING_OPTIMAL,
                                 VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
                                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                 _framebuffer.attachments[1].image,
                                 _framebuffer.attachments[1].mem,
                                 0,
                                 2 * m_max_point_light_count,
                                 1);
        _framebuffer.attachments[1].view = VVulkanUtil::createImageView(m_p_vulkan_context->_device,
                                                                        _framebuffer.attachments[1].image,
                                                                        _framebuffer.attachments[1].format,
                                                                        VK_IMAGE_ASPECT_DEPTH_BIT,
                                                                        VK_IMAGE_VIEW_TYPE_2D_ARRAY,
                                                                        2 * m_max_point_light_count,
                                                                        1);
    }

    void VPointLightPass::setupRenderPass()
    {
        VkAttachmentDescription attachments[2] = {};
        VkAttachmentDescription& point_light_shadow_color_attachment_description = attachments[0];
        point_light_shadow_color_attachment_description.format = _framebuffer.attachments[0].format;
        point_light_shadow_color_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
        point_light_shadow_color_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        point_light_shadow_color_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        point_light_shadow_color_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        point_light_shadow_color_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        point_light_shadow_color_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        point_light_shadow_color_attachment_description.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        VkAttachmentDescription& point_light_shadow_depth_attachment_description = attachments[1];
        point_light_shadow_depth_attachment_description.format = _framebuffer.attachments[1].format;
        point_light_shadow_depth_attachment_description.samples = VK_SAMPLE_COUNT_1_BIT;
        point_light_shadow_depth_attachment_description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        point_light_shadow_depth_attachment_description.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        point_light_shadow_depth_attachment_description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        point_light_shadow_depth_attachment_description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        point_light_shadow_depth_attachment_description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        point_light_shadow_depth_attachment_description.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpasses[1] = {};

        VkAttachmentReference shadow_pass_color_attachment_reference = {};
        shadow_pass_color_attachment_reference.attachment = &point_light_shadow_color_attachment_description - attachments;
        shadow_pass_color_attachment_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentReference shadow_pass_depth_attachment_reference = {};
        shadow_pass_depth_attachment_reference.attachment = &point_light_shadow_depth_attachment_description - attachments;
        shadow_pass_depth_attachment_reference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkSubpassDescription& shadow_pass = subpasses[0];
        shadow_pass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        shadow_pass.colorAttachmentCount = 1;
        shadow_pass.pColorAttachments = &shadow_pass_color_attachment_reference;
        shadow_pass.pDepthStencilAttachment = &shadow_pass_depth_attachment_reference;

        VkSubpassDependency dependencies[1] = {};

        VkSubpassDependency& lighting_pass_dependency = dependencies[0];
        lighting_pass_dependency.srcSubpass = 0;
        lighting_pass_dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
        lighting_pass_dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        lighting_pass_dependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        lighting_pass_dependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // store_op_store
        lighting_pass_dependency.dstAccessMask = 0;
        lighting_pass_dependency.dependencyFlags = 0; // not by begin

        VkRenderPassCreateInfo renderpass_create_info{};
        renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
        renderpass_create_info.pAttachments = attachments;
        renderpass_create_info.subpassCount = sizeof(subpasses) / sizeof(subpasses[0]);
        renderpass_create_info.pSubpasses = subpasses;
        renderpass_create_info.dependencyCount = sizeof(dependencies) / sizeof(dependencies[0]);
        renderpass_create_info.pDependencies = dependencies;

        if (vkCreateRenderPass(m_p_vulkan_context->_device, &renderpass_create_info, nullptr, &_framebuffer.render_pass) != VK_SUCCESS)
        {
            throw std::runtime_error("create point light shadow render pass");
        }
    }

    void VPointLightPass::setupFramebuffer()
    {
        VkImageView attachments[2] = {_framebuffer.attachments[0].view, _framebuffer.attachments[1].view};

        VkFramebufferCreateInfo framebuffer_create_info{};
        framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_create_info.flags = 0U;
        framebuffer_create_info.renderPass = _framebuffer.render_pass;
        framebuffer_create_info.attachmentCount = sizeof(attachments) / sizeof(attachments[0]);
        framebuffer_create_info.pAttachments = attachments;
        framebuffer_create_info.width = m_point_light_shadow_map_dimension;
        framebuffer_create_info.height = m_point_light_shadow_map_dimension;
        framebuffer_create_info.layers = 2 * m_max_point_light_count;

        if (vkCreateFramebuffer(m_p_vulkan_context->_device, &framebuffer_create_info, nullptr, &_framebuffer.framebuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("create point light shadow framebuffer");
        }
    }

    void VPointLightPass::setupDescriptorSetLayout()
    {
        _descriptor_infos.resize(1);

        VkDescriptorSetLayoutBinding mesh_point_light_shadow_global_layout_bindings[3];

        VkDescriptorSetLayoutBinding& mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding =
            mesh_point_light_shadow_global_layout_bindings[0];
        mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.binding = 0;
        mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.descriptorType =
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_point_light_shadow_global_layout_perframe_storage_buffer_binding.stageFlags =
            VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        
        VkDescriptorSetLayoutBinding& mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding =
            mesh_point_light_shadow_global_layout_bindings[1];
        mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.binding = 1;
        mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.descriptorType =
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.descriptorCount = 1;
        mesh_point_light_shadow_global_layout_perdrawcall_storage_buffer_binding.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutBinding& mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding =
            mesh_point_light_shadow_global_layout_bindings[2];
        mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.binding = 2;
        mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.descriptorType =
            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.descriptorCount = 1;
        mesh_point_light_shadow_global_layout_per_drawcall_vertex_blending_storage_buffer_binding.stageFlags =
            VK_SHADER_STAGE_VERTEX_BIT;

        VkDescriptorSetLayoutCreateInfo mesh_point_light_shadow_global_layout_create_info;
        mesh_point_light_shadow_global_layout_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        mesh_point_light_shadow_global_layout_create_info.pNext = nullptr;
        mesh_point_light_shadow_global_layout_create_info.flags = 0;
        mesh_point_light_shadow_global_layout_create_info.bindingCount = 
            sizeof(mesh_point_light_shadow_global_layout_bindings) / sizeof(mesh_point_light_shadow_global_layout_bindings[0]);
        mesh_point_light_shadow_global_layout_create_info.pBindings = mesh_point_light_shadow_global_layout_bindings;

        if (vkCreateDescriptorSetLayout(m_p_vulkan_context->_device, &mesh_point_light_shadow_global_layout_create_info,
                                        nullptr, &_descriptor_infos[0].layout) != VK_SUCCESS)
        {
            throw std::runtime_error("create mesh point light shadow global layout");
        }
    }

    
    void VPointLightPass::setupPipelines()
    {

    }
    void VPointLightPass::setupDescriptorSet()
    {

    }

} // namespace VE
