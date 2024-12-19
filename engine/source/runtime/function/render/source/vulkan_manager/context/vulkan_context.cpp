#include "runtime/function/render/include/vulkan_manager/vulkan_context.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"



#define VIRTUAL_XSTR(s) VIRTUAL_STR(s)
#define VIRTUAL_STR(s) #s

#if defined(__GNUC__)

#if defined(__linux__)
#include <stdlib.h>
// #elif defined(__MACH__) // Apple
// #include <stdlib.h>
// #else
// #error Unknow Platform
#endif

// #elif defined(_MSC_VER)
// #include <sdkddkver.h>
// #define WIN32_LEAN_AND_MEAN 1
// #define NOGDICAPMASKS 1
// #define NOVIRTUALKEYCODES 1
// #define NOWINMESSAGES 1
// #define NOWINSTYLES 1
// #define NOSYSMETRICS 1
// #define NOMENUS 1
// #define NOICONS 1
// #define NOKEYSTATES 1
// #define NOSYSCOMMANDS 1
// #define NORASTEROPS 1
// #define NOSHOWWINDOW 1
// #define NOATOM 1
// #define NOCLIPBOARD 1
// #define NOCOLOR 1
// #define NOCTLMGR 1
// #define NODRAWTEXT 1
// #define NOGDI 1
// #define NOKERNEL 1
// #define NOUSER 1
// #define NONLS 1
// #define NOMB 1
// #define NOMEMMGR 1
// #define NOMETAFILE 1
// #define NOMINMAX 1
// #define NOMSG 1
// #define NOOPENFILE 1
// #define NOSCROLL 1
// #define NOSERVICE 1
// #define NOSOUND 1
// #define NOTEXTMETRIC 1
// #define NOWH 1
// #define NOWINOFFSETS 1
// #define NOCOMM 1
// #define NOKANJI 1
// #define NOHELP 1
// #define NOPROFILER 1
// #define NODEFERWINDOWPOS 1
// #define NOMCX 1
// #include <Windows.h>
// #else
// #error Unknown Compiler

#endif









#include <iostream>
#include <set>

// 
#include <algorithm>

void VE::VVulkanContext::initialize(GLFWwindow* window)
{
    _window = window;

#if defined(__GNUC__)

#if defined(__linux__)
    char const* vk_layer_path = VIRTUAL_XSTR(VIRTUALENGINE_VK_LAYER_PATH);
    setenv("VK_LAYER_PATH", vk_layer_path, 1);
// #elif defined(__MACH__)
//     char const* vk_layer_path    = PILOT_XSTR(VIRTUALENGINE_VK_LAYER_PATH);
//     char const* vk_icd_filenames = PILOT_XSTR(VIRTUALENGINE_VK_ICD_FILENAMES);
//     setenv("VK_LAYER_PATH", vk_layer_path, 1);
//     setenv("VK_ICD_FILENAMES", vk_icd_filenames, 1);
// #else
// #error Unknown Platform
#endif

// #elif defined(_MSC_VER)
//     char const* vk_layer_path = PILOT_XSTR(VIRTUALENGINE_VK_LAYER_PATH);
//     SetEnvironmentVariableA("VK_LAYER_PATH", vk_layer_path);
//     SetEnvironmentVariableA("DISABLE_LAYER_AMD_SWITCHABLE_GRAPHICS_1", "1");
// #else
// #error Unknown Compiler

#endif

    createInstance();

    initializeDebugMessenger();

    createWindowSurface();

    initializePhysicalDevice();

    createLogicalDevice();

    // createCommandPool();

    // createSwapchain();

    // createSwapchainImageViews();

    // createFramebufferImageAndView();

    // createAssetAllocator();
}


// ---------------------------------------------------------------------------------------------------------------
std::vector<const char*> VE::VVulkanContext::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (VE::VVulkanManager::m_enable_validation_layers || VE::VVulkanManager::m_enable_debug_utils_label)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

// debug callback, accept message from validation layer
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT,
                                                    VkDebugUtilsMessageTypeFlagsEXT,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void*)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void VE::VVulkanContext::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}
// ===============================================================================================================
void VE::VVulkanContext::createInstance()
{
    // // validation layer will be enabled in debug mode
    // if (VE::VVulkanManager::m_enable_validation_layers && !checkValidationLayerSupport())
    // {
    //     throw std::runtime_error("validation layers requested, but not available");
    // }

    m_vulkan_api_version = VK_API_VERSION_1_0;

    // app info
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "virtual_renderer";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VirtualEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = m_vulkan_api_version;

    // create info
    VkInstanceCreateInfo instance_create_info{};
    instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pApplicationInfo = &appInfo; // the appInfo is stored here
    // /Extensions required by GLFW
    auto extensions = getRequiredExtensions();
    instance_create_info.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    instance_create_info.ppEnabledExtensionNames = extensions.data();

    // debug layers and validation layers
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (VE::VVulkanManager::m_enable_validation_layers)
    {
        instance_create_info.enabledLayerCount = static_cast<uint32_t>(m_validation_layers.size());
        instance_create_info.ppEnabledLayerNames = m_validation_layers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        instance_create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }

    // create m_vulkan_context._instance
    if (vkCreateInstance(&instance_create_info, nullptr, &_instance) != VK_SUCCESS)
    {
        throw std::runtime_error("vk create instance");
    }
}

// ===============================================================================================================
void VE::VVulkanContext::initializeDebugMessenger()
{
    if (VE::VVulkanManager::m_enable_validation_layers)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);
        if (VK_SUCCESS != createDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &m_debug_messenger))
        {
            throw std::runtime_error("failed to set up debug messager!");
        }
    }

    if (VE::VVulkanManager::m_enable_debug_utils_label)
    {
        _vkCmdBeginDebugUtilsLabelEXT =
            (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetInstanceProcAddr(_instance, "vkCmdBeginDebugUtilsLabelEXT"); // Proc：procedure； here means get function pointer from vulkan instance
        _vkCmdEndDebugUtilsLabelEXT = 
            (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetInstanceProcAddr(_instance, "vkCmdEndDebugUtilsLabelEXT");
    }
}

// ===============================================================================================================
void VE::VVulkanContext::createWindowSurface()
{
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("glfwCreateWindowSurface");
    }
}

// ===============================================================================================================
void VE::VVulkanContext::initializePhysicalDevice()
{
    uint32_t physical_device_count;
    vkEnumeratePhysicalDevices(_instance, &physical_device_count, nullptr);
    if (physical_device_count == 0)
    {
        throw std::runtime_error("enumerate physical devides");
    }
    else
    {
        // find one device that matches our requirement
        // or find which is the best
        std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
        vkEnumeratePhysicalDevices(_instance, &physical_device_count, physical_devices.data());

        std::vector<std::pair<int, VkPhysicalDevice>> ranked_physical_devices;
        for (const auto& device : physical_devices)
        {
            VkPhysicalDeviceProperties physical_device_properties;
            vkGetPhysicalDeviceProperties(device, &physical_device_properties);
            int score = 0;

            if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                score += 1000;
            }
            else if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
            {
                score += 100;
            }

            ranked_physical_devices.push_back({score, device});
        }

        std::sort(ranked_physical_devices.begin(),
                  ranked_physical_devices.end(),
                  [](const std::pair<int, VkPhysicalDevice>& p1, const std::pair<int, VkPhysicalDevice>& p2) {
                    return p1 > p2;
                  });

        for (const auto& device : ranked_physical_devices)
        {
            if (isDeviceSuitable(device.second))
            {
                _physical_device = device.second;
                break;
            }
        }

        if (_physical_device == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find suitable physical device");
        }
    }
}

// ===============================================================================================================
// logical device (m_vulkan_context._device : graphic queue, present queue,
// feature:samplerAnisotropy)
void VE::VVulkanContext::createLogicalDevice()
{
    _queue_indices = findQueueFamilies(_physical_device);

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos; // all queues that need to be created
    std::set<uint32_t> queue_families = {_queue_indices.graphicsFamily.value(), _queue_indices.presentFamily.value()};

    float queue_priority = 1.0f;
    for (uint32_t queue_family : queue_families) // for every queue family
    {
        // queue create info
        VkDeviceQueueCreateInfo queue_create_info{};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    VkPhysicalDeviceFeatures physical_device_features = {};
    // physical device features
    physical_device_features.samplerAnisotropy = VK_TRUE;

    // vertex and fragment shader stores and atomic operations
    physical_device_features.vertexPipelineStoresAndAtomics = VK_TRUE;
    // support inefficient readback storage buffer
    physical_device_features.fragmentStoresAndAtomics = VK_TRUE;

    // support geometry shader
    // if (VE::VVulkanManager::m_enable_point_light_shadow)
    // {
    //     physical_device_features.geometryShader = VK_TRUE;
    // }

    // device create info
    VkDeviceCreateInfo device_create_info{};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos.data();
    device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
    device_create_info.pEnabledFeatures = &physical_device_features;
    device_create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size());
    device_create_info.ppEnabledExtensionNames = m_device_extensions.data();
    device_create_info.enabledLayerCount = 0;

    if (vkCreateDevice(_physical_device, &device_create_info, nullptr, &_device) != VK_SUCCESS)
    {
        throw std::runtime_error("vk create device");
    }

    // initialize queues of this device
    vkGetDeviceQueue(_device, _queue_indices.graphicsFamily.value(), 0, &_graphics_queue);
    vkGetDeviceQueue(_device, _queue_indices.presentFamily.value(), 0, &_present_queue);
    
    // more efficient pointer
    _vkWaitForFences         = (PFN_vkWaitForFences)vkGetDeviceProcAddr(_device, "vkWaitForFences"); // for sync operation
    _vkResetFences           = (PFN_vkResetFences)vkGetDeviceProcAddr(_device, "vkResetFences"); 
    _vkResetCommandPool      = (PFN_vkResetCommandPool)vkGetDeviceProcAddr(_device, "vkResetCommandPool");
    _vkBeginCommandBuffer    = (PFN_vkBeginCommandBuffer)vkGetDeviceProcAddr(_device, "vkBeginCommandBuffer");
    _vkEndCommandBuffer      = (PFN_vkEndCommandBuffer)vkGetDeviceProcAddr(_device, "vkEndCommandBuffer");
    _vkCmdBeginRenderPass    = (PFN_vkCmdBeginRenderPass)vkGetDeviceProcAddr(_device, "vkCmdBeginRenderPass");
    _vkCmdNextSubpass        = (PFN_vkCmdNextSubpass)vkGetDeviceProcAddr(_device, "vkCmdNextSubpass"); // in renderpass, render the next subpass, can share data
    _vkCmdEndRenderPass      = (PFN_vkCmdEndRenderPass)vkGetDeviceProcAddr(_device, "vkCmdEndRenderPass");
    _vkCmdBindPipeline       = (PFN_vkCmdBindPipeline)vkGetDeviceProcAddr(_device, "vkCmdBindPipeline"); // bind pipeline to buffer
    _vkCmdSetViewport        = (PFN_vkCmdSetViewport)vkGetDeviceProcAddr(_device, "vkCmdSetViewport");
    _vkCmdSetScissor         = (PFN_vkCmdSetScissor)vkGetDeviceProcAddr(_device, "vkCmdSetScissor"); // limit rendering area
    _vkCmdBindVertexBuffers  = (PFN_vkCmdBindVertexBuffers)vkGetDeviceProcAddr(_device, "vkCmdBindVertexBuffers");
    _vkCmdBindIndexBuffer    = (PFN_vkCmdBindIndexBuffer)vkGetDeviceProcAddr(_device, "vkCmdBindIndexBuffer");
    _vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)vkGetDeviceProcAddr(_device, "vkCmdBindDescriptorSets"); //descriptorSets include texture/const buffer etc
    _vkCmdDrawIndexed        = (PFN_vkCmdDrawIndexed)vkGetDeviceProcAddr(_device, "vkCmdDrawIndexed"); // draw  according to vertex array and index array

    _depth_image_format = findDepthFormat();
}






// -----------------------------------------------------------------------------------------------------------------
VE::QueueFamilyIndices VE::VVulkanContext::findQueueFamilies(VkPhysicalDevice physical_device) // for device and surface
{
    QueueFamilyIndices indices;
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    int i = 0;
    for (const auto& queue_family : queue_families)
    {
        if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) // if support graphics command queue
            indices.graphicsFamily = i;

        VkBool32 is_present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
                                             i, 
                                             _surface,
                                             &is_present_support); // if support surface presentation
        if (is_present_support)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }
    return indices;
}

bool VE::VVulkanContext::checkDeviceExtensionSupport(VkPhysicalDevice physical_device)
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> available_extensions(extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, available_extensions.data());
    
    std::set<std::string> required_extensions(m_device_extensions.begin(), m_device_extensions.end());
    for (const auto& extension : available_extensions)
    {
        required_extensions.erase(extension.extensionName);
    }

    return required_extensions.empty();
}

bool VE::VVulkanContext::isDeviceSuitable(VkPhysicalDevice physical_device)
{
    auto queue_indices = findQueueFamilies(physical_device);
    bool is_extensions_supported = checkDeviceExtensionSupport(physical_device);
    bool is_swapchain_adequate = false;

    if (is_extensions_supported)
    {
        SwapChainSupportDetails swapchain_support_details = querySwapChainSupport(physical_device);
        is_swapchain_adequate = 
            !swapchain_support_details.formats.empty() && !swapchain_support_details.presentModes.empty();
    }

    VkPhysicalDeviceFeatures physical_device_features;
    vkGetPhysicalDeviceFeatures(physical_device, &physical_device_features);

    if (!queue_indices.isComplete() || !is_swapchain_adequate || !physical_device_features.samplerAnisotropy)
    {
        return false;
    }

    return true;
}

VE::SwapChainSupportDetails VE::VVulkanContext::querySwapChainSupport(VkPhysicalDevice physical_device)
{
    SwapChainSupportDetails details_result;

    // capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, _surface, &details_result.capabilities);

    // format
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, _surface, &format_count, nullptr);
    if (format_count != 0)
    {
        details_result.formats.resize(format_count);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, _surface, &format_count, details_result.formats.data());
    }

    // present modes
    uint32_t presentmode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, _surface, &presentmode_count, nullptr);
    if (presentmode_count != 0)
    {
        details_result.presentModes.resize(presentmode_count);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, _surface, &presentmode_count, details_result.presentModes.data());
    }

    return details_result;
}

// ----------------------------------------------------------------------------------------------------------------
VkFormat VE::VVulkanContext::findDepthFormat()
{                               // single float32   single float32 + uint8 stencil   unsigned normal depth24 + uint8 stencil
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
                                VK_IMAGE_TILING_OPTIMAL,
                                VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT); // support depth and stencil attachment
}

VkFormat VE::VVulkanContext::findSupportedFormat(const std::vector<VkFormat>& candidates,
                                                 VkImageTiling tiling,
                                                 VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_physical_device, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }

    throw std::runtime_error("findSupportedFormat failed");
}
// ----------------------------------------------------------------------------------------------------------------


VkResult VE::VVulkanContext::createDebugUtilsMessengerEXT(VkInstance instance,
                                                          const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                          const VkAllocationCallbacks* pAllocator,
                                                          VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}