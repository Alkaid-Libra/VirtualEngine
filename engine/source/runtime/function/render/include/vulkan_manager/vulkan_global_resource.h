#pragma once


#include "runtime/function/render/include/framebuffer.h"

// 
#include <vulkan/vulkan.h>
#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"

namespace VE
{
    
    struct VStorageBuffer
    {
        // limits
        uint32_t _min_uniform_buffer_offset_alignment = 256;
        uint32_t _min_storage_buffer_offset_alignment = 256;
        uint32_t _max_storage_buffer_range = 1 << 27;
        uint32_t _non_coherent_atom_size = 256;

        VkBuffer              _global_upload_ringbuffer;
        VkDeviceMemory        _global_upload_ringbuffer_memory;
        void*                 _global_upload_ringbuffer_memory_pointer;
        std::vector<uint32_t> _global_upload_ringbuffers_begin;
        std::vector<uint32_t> _global_upload_ringbuffers_end;
        std::vector<uint32_t> _global_upload_ringbuffers_size;

        VkBuffer _global_null_descriptor_storage_buffer;
        VkDeviceMemory _global_null_descriptor_storage_buffer_memory;

        // axis
        VkBuffer _axis_inefficient_storage_buffer;
        VkDeviceMemory _axis_inefficient_storage_buffer_memory;
        void* _axis_inefficient_storage_buffer_memory_pointer;
    };

    class VGlobalRenderResource
    {
    public:

    VStorageBuffer _storage_buffer;


        void initialize(VVulkanContext& context, int frames_in_flight = 3);


    private:
        void initializeStorageBuffer(VVulkanContext& context, int frames_in_flight);
        void mapStorageBuffer(VVulkanContext& context);
        void unmapStorageBuffer(VVulkanContext& context);
    };
} // namespace VE
