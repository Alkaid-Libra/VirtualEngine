// #include "runtime/function/render/include/vulkan_manager/vulkan_passes.h"
// #include "runtime/function/render/include/vulkan_manager/vulkan_util.h"


// #include <vector>
// #include <fstream>

// namespace VE
// {       
//     void VTrianglePass::setupRenderPass()
//     {
//         VkAttachmentDescription colorAttachment = {};
//         colorAttachment.format = m_v_vulkan_context->_swapchain_image_format;
//         colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
//         colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
//         colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
//         colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
//         colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
//         colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//         colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

//         VkAttachmentReference colorAttachmentRef = {};
//         colorAttachmentRef.attachment = 0;
//         colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

//         VkSubpassDescription subpass = {};
//         subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
//         subpass.colorAttachmentCount = 1;
//         subpass.pColorAttachments = &colorAttachmentRef;

//         VkRenderPassCreateInfo renderPassInfo = {};
//         renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
//         renderPassInfo.attachmentCount = 1;
//         renderPassInfo.pAttachments = &colorAttachment;
//         renderPassInfo.subpassCount = 1;
//         renderPassInfo.pSubpasses = &subpass;

//         if (vkCreateRenderPass(m_v_vulkan_context->_device, &renderPassInfo, nullptr, &_framebuffer.render_pass) != VK_SUCCESS)
//         {
//             throw std::runtime_error("failed to create render pass");
//         }
//     }

//     void VTrianglePass::setupPipelines()
//     {
//         auto vertShaderCode = readFile("../../../../shader/generated/spv/vert.spv");
//         auto fragShaderCode = readFile("../../../../shader/generated/spv/frag.spv");
//         // auto vertShaderCode = readFile("../../../../../../../../shader/generated/spv/vert.spv");
//         // auto fragShaderCode = readFile("frag.spv");

//         VkShaderModule vertShaderModule = VVulkanUtil::createShaderModule(m_v_vulkan_context->_device, vertShaderCode);
//         VkShaderModule fragShaderModule = VVulkanUtil::createShaderModule(m_v_vulkan_context->_device, fragShaderCode);
    
//         VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
//         vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
//         vertShaderStageInfo.module = vertShaderModule;
//         vertShaderStageInfo.pName = "main";

//         VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
//         fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
//         fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
//         fragShaderStageInfo.module = fragShaderModule;
//         fragShaderStageInfo.pName = "main";

//         VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

//         VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
//         vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//         vertexInputInfo.vertexBindingDescriptionCount = 0;
//         vertexInputInfo.pVertexBindingDescriptions = nullptr;
//         vertexInputInfo.vertexAttributeDescriptionCount = 0;
//         vertexInputInfo.pVertexAttributeDescriptions = nullptr;

//         VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
//         inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//         inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
//         inputAssembly.primitiveRestartEnable = VK_FALSE;

//         VkViewport viewport = {};
//         viewport.x = 0.0f;
//         viewport.y = 0.0f;
//         viewport.width = 1280.0f;
//         viewport.height = 960.0f;
//         viewport.minDepth = 0.0f;
//         viewport.maxDepth = 1.0f;

//         VkRect2D scissor = {};
//         scissor.offset = { 0, 0 };
//         scissor.extent = {1280, 960};

//         VkPipelineViewportStateCreateInfo viewportState = {};
//         viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//         viewportState.viewportCount = 1;
//         viewportState.pViewports = &viewport;
//         viewportState.scissorCount = 1;
//         viewportState.pScissors = &scissor;

//         VkPipelineRasterizationStateCreateInfo rasterizer = {};
//         rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//         rasterizer.depthClampEnable = VK_FALSE;
//         rasterizer.rasterizerDiscardEnable = VK_FALSE;
//         rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
//         rasterizer.lineWidth = 1.0f;
//         rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
//         rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//         rasterizer.depthBiasEnable = VK_FALSE;
//         rasterizer.depthBiasConstantFactor = 0.0f;
//         rasterizer.depthBiasClamp = 0.0f;
//         rasterizer.depthBiasSlopeFactor = 0.0;

//         VkPipelineMultisampleStateCreateInfo multisampling = {};
//         multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//         multisampling.sampleShadingEnable = VK_FALSE;
//         multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//         multisampling.minSampleShading = 1.0f;
//         multisampling.pSampleMask = nullptr;
//         multisampling.alphaToCoverageEnable = VK_FALSE;
//         multisampling.alphaToOneEnable = VK_FALSE;

//         VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
//         colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
//             VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
//             VK_COLOR_COMPONENT_A_BIT;
//         colorBlendAttachment.blendEnable = VK_FALSE;
//         colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
//         colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
//         colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//         colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//         colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//         colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

//         colorBlendAttachment.blendEnable = VK_TRUE;
//         colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//         colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//         colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
//         colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
//         colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
//         colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

//         VkPipelineColorBlendStateCreateInfo colorBlending = {};
//         colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//         colorBlending.logicOpEnable = VK_FALSE;
//         colorBlending.logicOp = VK_LOGIC_OP_COPY;
//         colorBlending.attachmentCount = 1;
//         colorBlending.pAttachments = &colorBlendAttachment;
//         colorBlending.blendConstants[0] = 0.0f;
//         colorBlending.blendConstants[1] = 0.0f;
//         colorBlending.blendConstants[2] = 0.0f;
//         colorBlending.blendConstants[3] = 0.0f;

//         VkDynamicState dynamicStates[] = {
//             VK_DYNAMIC_STATE_VIEWPORT,
//             VK_DYNAMIC_STATE_LINE_WIDTH
//         };

//         VkPipelineDynamicStateCreateInfo dynamicState = {};
//         dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//         dynamicState.dynamicStateCount = 2;
//         dynamicState.pDynamicStates = dynamicStates;

//         VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
//         pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//         pipelineLayoutInfo.setLayoutCount = 0;
//         pipelineLayoutInfo.pSetLayouts = nullptr;
//         pipelineLayoutInfo.pushConstantRangeCount = 0;
//         pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
//         if (vkCreatePipelineLayout(m_v_vulkan_context->_device, &pipelineLayoutInfo, nullptr, &_render_pipelines[0].layout) != VK_SUCCESS)
//         {
//             throw std::runtime_error("failed to create pipeline layout!");
//         }

//         VkGraphicsPipelineCreateInfo pipelineInfo = {};
//         pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//         pipelineInfo.stageCount = 2;
//         pipelineInfo.pStages = shaderStages;

//         pipelineInfo.pVertexInputState = &vertexInputInfo;
//         pipelineInfo.pInputAssemblyState = &inputAssembly;
//         pipelineInfo.pViewportState = &viewportState;
//         pipelineInfo.pRasterizationState = &rasterizer;
//         pipelineInfo.pMultisampleState = &multisampling;
//         pipelineInfo.pDepthStencilState = nullptr;
//         pipelineInfo.pColorBlendState = &colorBlending;
//         pipelineInfo.pDynamicState = nullptr;

//         pipelineInfo.layout = _render_pipelines[0].layout;

//         pipelineInfo.renderPass = _framebuffer.render_pass;
//         pipelineInfo.subpass = 0;

//         pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//         pipelineInfo.basePipelineIndex = -1;

//         if (vkCreateGraphicsPipelines(m_v_vulkan_context->_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_render_pipelines[0].pipeline) != VK_SUCCESS)
//         {
//             throw std::runtime_error("failed to create graphic pipeline");
//         }

//         vkDestroyShaderModule(m_v_vulkan_context->_device, fragShaderModule, nullptr);
//         vkDestroyShaderModule(m_v_vulkan_context->_device, vertShaderModule, nullptr);
//     }

//     // void VTrianglePass::setupFramebuffer()
//     // {

//     // }

//     void VTrianglePass::createFramebuffers()
//     {
//         //[11]调整容器大小以容纳所有帧缓冲区
//         m_v_vulkan_context->_swapchain_framebuffers.resize(m_v_vulkan_context->_swapchain_imageviews.size());    //分配足够的空间来存储所有帧缓冲对象
//         // m_v_vulkan_context->_swapchain_framebuffers.resize(swapChainImageViews.size());    //分配足够的空间来存储所有帧缓冲对象

//         //[11] create framebuffers
//         for (size_t i = 0; i < m_v_vulkan_context->_swapchain_imageviews.size(); i++) 
//         { //为交换链的每一个图像视图对象创建对应的帧缓冲
//             VkImageView attachments[] = { m_v_vulkan_context->_swapchain_imageviews[i] };

//             VkFramebufferCreateInfo framebufferInfo = {};
//             framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
//             //指定帧缓冲需要兼容的渲染流程对象. 之后的渲染操作，我们可以使用与这个指定的渲染流程对象相兼容的其它渲染流程对象.一般来说，使用相同数量，相同类型附着的渲染流程对象是相兼容的。
//             framebufferInfo.renderPass = _framebuffer.render_pass;
//             framebufferInfo.attachmentCount = 1;    //指定附着个数
//             framebufferInfo.pAttachments = attachments; //指定渲染流程对象用于描述附着信息的 pAttachment 数组
//             //指定帧缓冲的大小
//             framebufferInfo.width = m_v_vulkan_context->_swapchain_extent.width;
//             framebufferInfo.height = m_v_vulkan_context->_swapchain_extent.height;
//             framebufferInfo.layers = 1;        //指定图像层数,这里的交换链图像都是单层的

//             if (vkCreateFramebuffer(m_v_vulkan_context->_device, &framebufferInfo, nullptr, &m_v_vulkan_context->_swapchain_framebuffers[i]) != VK_SUCCESS) {
//                 throw std::runtime_error("failed to create framebuffer!");
//             }
//         }

//     }


//     std::vector<char> VTrianglePass::readFile(const std::string& filename) {
//         std::ifstream file(filename, std::ios::ate | std::ios::binary);
//         if (!file.is_open()) {
//             throw std::runtime_error("failed to open file!");
//         }
//         //[10]ate 的优势是，可以获取文件的大小
//         size_t fileSize = (size_t)file.tellg();
//         std::vector<char> buffer(fileSize);
//         //[10]指针跳到头
//         file.seekg(0);
//         file.read(buffer.data(), fileSize);
//         file.close();
//         return buffer;
//     }

// } // namespace VE
