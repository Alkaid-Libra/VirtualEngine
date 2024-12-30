#include "runtime/function/render/include/vulkan_manager/vulkan_global_resource.h"

#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_util.h"

void VE::VGlobalRenderResource::initialize(VVulkanContext& context, int frames_in_flight)
{
    // initializeIBLResource(context);
    // create storage buffer(must in vulkan for data) and map them to device
    initializeStorageBuffer(context, frames_in_flight);
    mapStorageBuffer(context);
}

void VE::VGlobalRenderResource::initializeStorageBuffer(VVulkanContext& context, int frames_in_flight)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(context._physical_device, &properties);

    _storage_buffer._min_uniform_buffer_offset_alignment =
        static_cast<uint32_t>(properties.limits.minUniformBufferOffsetAlignment);
    _storage_buffer._min_storage_buffer_offset_alignment =
        static_cast<uint32_t>(properties.limits.minStorageBufferOffsetAlignment);
    _storage_buffer._max_storage_buffer_range = properties.limits.maxStorageBufferRange;
    _storage_buffer._non_coherent_atom_size = properties.limits.nonCoherentAtomSize;

    // In vulkan, the storage buffer should be pre-allocated
    // The size is 128MB in NVIDIA D3D11 drive
    uint32_t global_storage_buffer_size = 1024 * 1024 * 128;
    VVulkanUtil::createBuffer(context._physical_device, context._device, global_storage_buffer_size,
                              VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                              _storage_buffer._global_upload_ringbuffer, _storage_buffer._global_upload_ringbuffer_memory);
    _storage_buffer._global_upload_ringbuffers_begin.resize(frames_in_flight);
    _storage_buffer._global_upload_ringbuffers_end.resize(frames_in_flight);
    _storage_buffer._global_upload_ringbuffers_size.resize(frames_in_flight);
    for (uint32_t i = 0; i < frames_in_flight; ++i)
    {
        _storage_buffer._global_upload_ringbuffers_begin[i] = (global_storage_buffer_size * i) / frames_in_flight;
        _storage_buffer._global_upload_ringbuffers_size[i] = (global_storage_buffer_size * (i + 1)) / frames_in_flight -
                                                             (global_storage_buffer_size * i) / frames_in_flight;
    }

    // axis
    VVulkanUtil::createBuffer(context._physical_device, context._device, sizeof(AxisStorageBufferObject),
                              VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                              _storage_buffer._axis_inefficient_storage_buffer, _storage_buffer._axis_inefficient_storage_buffer_memory);
    
    // null descriptor
    VVulkanUtil::createBuffer(context._physical_device, context._device, 64, 
                              VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 0, 
                              _storage_buffer._global_null_descriptor_storage_buffer, _storage_buffer._global_null_descriptor_storage_buffer_memory);
    
    static_assert(64 >= sizeof(VMeshVertex::VulkanMeshVertexJointBinding), "");
}

void VE::VGlobalRenderResource::mapStorageBuffer(VVulkanContext& context)
{
    // TODO: Unmap when program terminates
    vkMapMemory(context._device, _storage_buffer._global_upload_ringbuffer_memory,
                0, VK_WHOLE_SIZE, 0, &_storage_buffer._global_upload_ringbuffer_memory_pointer);
    
    vkMapMemory(context._device, _storage_buffer._axis_inefficient_storage_buffer_memory,
                0, VK_WHOLE_SIZE, 0, &_storage_buffer._axis_inefficient_storage_buffer_memory_pointer);
}

void VE::VGlobalRenderResource::unmapStorageBuffer(VVulkanContext& context) {}