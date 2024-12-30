#include "runtime/function/render/include/vulkan_manager/vulkan_util.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"

#include <algorithm>
#include <stdexcept>


VkSampler VE::VVulkanUtil::m_nearest_sampler = VK_NULL_HANDLE;

uint32_t VE::VVulkanUtil::findMemoryType(VkPhysicalDevice         physical_device,
                                            uint32_t              type_filter,
                                            VkMemoryPropertyFlags properties_flag)
{
    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);
    for (uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++)
    {
        if (type_filter & (1 << i) &&
            (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties_flag) == properties_flag)
        {
            return i;
        }
    }
    throw std::runtime_error("findMemoryType");
}

VkShaderModule VE::VVulkanUtil::createShaderModule(VkDevice device, const std::vector<unsigned char>& shader_code)
{
    VkShaderModuleCreateInfo shader_module_create_info{};
    shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shader_module_create_info.codeSize = shader_code.size();
    shader_module_create_info.pCode = reinterpret_cast<const uint32_t*>(shader_code.data());

    VkShaderModule shader_module;
    if (vkCreateShaderModule(device, &shader_module_create_info, nullptr, &shader_module) != VK_SUCCESS)
    {
        return VK_NULL_HANDLE;
    }
    return shader_module;
}

VkImageView VE::VVulkanUtil::createImageView(VkDevice           device,
                                             VkImage&           image,
                                             VkFormat           format,
                                             VkImageAspectFlags image_aspect_flags,
                                             VkImageViewType    view_type,
                                             uint32_t           layout_count,
                                             uint32_t           miplevels)
{
    VkImageViewCreateInfo image_view_create_info{};
    image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    image_view_create_info.image                           = image;
    image_view_create_info.viewType                        = view_type;
    image_view_create_info.format                          = format;
    image_view_create_info.subresourceRange.aspectMask     = image_aspect_flags;
    image_view_create_info.subresourceRange.baseMipLevel   = 0;
    image_view_create_info.subresourceRange.levelCount     = miplevels;
    image_view_create_info.subresourceRange.baseArrayLayer = 0;
    image_view_create_info.subresourceRange.layerCount     = layout_count;

    VkImageView image_view;
    if (vkCreateImageView(device, &image_view_create_info, nullptr, &image_view) != VK_SUCCESS)
    {
        return image_view;
        // todo
    }

    return image_view;
}

void VE::VVulkanUtil::createImage(VkPhysicalDevice      physical_device,
                                     VkDevice              device,
                                     uint32_t              image_width,
                                     uint32_t              image_height,
                                     VkFormat              format,
                                     VkImageTiling         image_tiling,
                                     VkImageUsageFlags     image_usage_flags,
                                     VkMemoryPropertyFlags memory_property_flags,
                                     VkImage&              image,
                                     VkDeviceMemory&       memory,
                                     VkImageCreateFlags    image_create_flags,
                                     uint32_t              array_layers,
                                     uint32_t              miplevels)
{
    VkImageCreateInfo image_create_info {};
    image_create_info.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image_create_info.flags         = image_create_flags;
    image_create_info.imageType     = VK_IMAGE_TYPE_2D;
    image_create_info.extent.width  = image_width;
    image_create_info.extent.height = image_height;
    image_create_info.extent.depth  = 1;
    image_create_info.mipLevels     = miplevels;
    image_create_info.arrayLayers   = array_layers;
    image_create_info.format        = format;
    image_create_info.tiling        = image_tiling;
    image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    image_create_info.usage         = image_usage_flags;
    image_create_info.samples       = VK_SAMPLE_COUNT_1_BIT;
    image_create_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(device, &image_create_info, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo {};
    allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physical_device, memRequirements.memoryTypeBits, memory_property_flags);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(device, image, memory, 0);
}

void VE::VVulkanUtil::createBuffer(VkPhysicalDevice      physical_device,
                                   VkDevice              device,
                                   VkDeviceSize          size,
                                   VkBufferUsageFlags    usage,
                                   VkMemoryPropertyFlags properties,
                                   VkBuffer&             buffer,
                                   VkDeviceMemory&       buffer_memory)
{
    VkBufferCreateInfo buffer_create_info {};
    buffer_create_info.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_create_info.size        = size;
    buffer_create_info.usage       = usage;                     // use as a vertex/staging/index buffer
    buffer_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // not sharing among queue families

    if (vkCreateBuffer(device, &buffer_create_info, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("vkCreateBuffer");
    }

    VkMemoryRequirements buffer_memory_requirements; // for allocate_info.allocationSize and
                                                     // allocate_info.memoryTypeIndex
    vkGetBufferMemoryRequirements(device, buffer, &buffer_memory_requirements);

    VkMemoryAllocateInfo buffer_memory_allocate_info {};
    buffer_memory_allocate_info.sType          = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    buffer_memory_allocate_info.allocationSize = buffer_memory_requirements.size;
    buffer_memory_allocate_info.memoryTypeIndex =
        VVulkanUtil::findMemoryType(physical_device, buffer_memory_requirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &buffer_memory_allocate_info, nullptr, &buffer_memory) != VK_SUCCESS)
    {
        throw std::runtime_error("vkAllocateMemory");
    }

    // bind buffer with buffer memory
    vkBindBufferMemory(device, buffer, buffer_memory, 0); // offset = 0
}

VkSampler VE::VVulkanUtil::getOrCreateNearestSampler(VkPhysicalDevice physical_device, VkDevice device)
{
    if (m_nearest_sampler == VK_NULL_HANDLE)
    {
        VkPhysicalDeviceProperties physical_device_properties {};
        vkGetPhysicalDeviceProperties(physical_device, &physical_device_properties);

        VkSamplerCreateInfo samplerInfo {};

        samplerInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter               = VK_FILTER_NEAREST;
        samplerInfo.minFilter               = VK_FILTER_NEAREST;
        samplerInfo.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_NEAREST;
        samplerInfo.addressModeU            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeV            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.addressModeW            = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
        samplerInfo.mipLodBias              = 0.0f;
        samplerInfo.anisotropyEnable        = VK_FALSE;
        samplerInfo.maxAnisotropy           = physical_device_properties.limits.maxSamplerAnisotropy; // close :1.0f
        samplerInfo.compareEnable           = VK_FALSE;
        samplerInfo.compareOp               = VK_COMPARE_OP_ALWAYS;
        samplerInfo.minLod                  = 0.0f;
        samplerInfo.maxLod                  = 8.0f; // todo: m_irradiance_texture_miplevels
        samplerInfo.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;

        if (vkCreateSampler(device, &samplerInfo, nullptr, &m_nearest_sampler) != VK_SUCCESS)
        {
            throw std::runtime_error("vk create sampler");
        }
    }

    return m_nearest_sampler;
}