#include <iostream>

#include "editor/include/editor.h"
#include "runtime/engine.h"

// macro stringlize
#define VIRTUAL_XSTR(s) VIRTUAL_STR(s)
#define VIRTUAL_STR(s) #s



int main()
{
    std::filesystem::path virtual_root_folder = std::filesystem::path(VIRTUAL_XSTR(VIRTUAL_ROOT_DIR));
    
    VE::EngineInitParams params;
    params.m_root_folder = virtual_root_folder;
    params.m_config_file_path = virtual_root_folder / "VirtualEditor.ini";

    VE::VirtualEngine::getInstance().startEngine(params);



    VE::VirtualEditor::getInstance().initialize(&(VE::VirtualEngine::getInstance()));
    VE::VirtualEditor::getInstance().run();




    // VE::VirtualEngine::getInstance().shutdownEngine();


    // std::cout << params.m_config_file_path << std::endl;
    return 0;
}



// #include <vulkan/vulkan.h>
// #include <GLFW/glfw3.h>
// #include <iostream>
// #include <vector>
// #include <stdexcept>
// #include <cstdlib>

// class VulkanApplication {
// public:
//     void run() {
//         initWindow();
//         initVulkan();
//         mainLoop();
//         cleanup();
//     }

// private:
//     GLFWwindow* window;
//     VkInstance instance;
//     VkSurfaceKHR surface;
//     VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//     VkDevice device;
//     VkQueue graphicsQueue;
//     VkQueue presentQueue;

//     const uint32_t WIDTH = 800;
//     const uint32_t HEIGHT = 600;

//     const std::vector<const char*> validationLayers = {
//         "VK_LAYER_KHRONOS_validation"
//     };

// #ifdef NDEBUG
//     const bool enableValidationLayers = false;
// #else
//     const bool enableValidationLayers = true;
// #endif

//     void initWindow() {
//         glfwInit();

//         // No default OpenGL context, only Vulkan
//         glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//         glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

//         window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Window", nullptr, nullptr);
//     }

//     void initVulkan() {
//         createInstance();
//         createSurface();
//         pickPhysicalDevice();
//         createLogicalDevice();
//     }

//     void mainLoop() {
//         while (!glfwWindowShouldClose(window)) {
//             glfwPollEvents();
//         }
//     }

//     void cleanup() {
//         vkDestroyDevice(device, nullptr);
//         vkDestroySurfaceKHR(instance, surface, nullptr);
//         vkDestroyInstance(instance, nullptr);
//         glfwDestroyWindow(window);
//         glfwTerminate();
//     }

//     void createInstance() {
//         VkApplicationInfo appInfo{};
//         appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//         appInfo.pApplicationName = "Hello Vulkan";
//         appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
//         appInfo.pEngineName = "No Engine";
//         appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
//         appInfo.apiVersion = VK_API_VERSION_1_3;

//         VkInstanceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//         createInfo.pApplicationInfo = &appInfo;

//         // Extensions required by GLFW
//         uint32_t glfwExtensionCount = 0;
//         const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
//         createInfo.enabledExtensionCount = glfwExtensionCount;
//         createInfo.ppEnabledExtensionNames = glfwExtensions;

//         // Validation layers
//         if (enableValidationLayers) {
//             createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//             createInfo.ppEnabledLayerNames = validationLayers.data();
//         } else {
//             createInfo.enabledLayerCount = 0;
//         }

//         if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
//             throw std::runtime_error("failed to create instance!");
//         }
//     }

//     void createSurface() {
//         if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
//             throw std::runtime_error("failed to create window surface!");
//         }
//     }

//     void pickPhysicalDevice() {
//         uint32_t deviceCount = 0;
//         vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

//         if (deviceCount == 0) {
//             throw std::runtime_error("failed to find GPUs with Vulkan support!");
//         }

//         std::vector<VkPhysicalDevice> devices(deviceCount);
//         vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

//         for (const auto& device : devices) {
//             if (isDeviceSuitable(device)) {
//                 physicalDevice = device;
//                 break;
//             }
//         }

//         if (physicalDevice == VK_NULL_HANDLE) {
//             throw std::runtime_error("failed to find a suitable GPU!");
//         }
//     }

//     bool isDeviceSuitable(VkPhysicalDevice device) {
//         VkPhysicalDeviceProperties deviceProperties;
//         VkPhysicalDeviceFeatures deviceFeatures;
//         vkGetPhysicalDeviceProperties(device, &deviceProperties);
//         vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

//         return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
//                deviceFeatures.geometryShader;
//     }

//     void createLogicalDevice() {
//         VkDeviceQueueCreateInfo queueCreateInfo{};
//         queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//         queueCreateInfo.queueFamilyIndex = 0; // Assume index 0 is valid
//         queueCreateInfo.queueCount = 1;

//         float queuePriority = 1.0f;
//         queueCreateInfo.pQueuePriorities = &queuePriority;

//         VkPhysicalDeviceFeatures deviceFeatures{};

//         VkDeviceCreateInfo createInfo{};
//         createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//         createInfo.pQueueCreateInfos = &queueCreateInfo;
//         createInfo.queueCreateInfoCount = 1;
//         createInfo.pEnabledFeatures = &deviceFeatures;

//         createInfo.enabledExtensionCount = 0;

//         if (enableValidationLayers) {
//             createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//             createInfo.ppEnabledLayerNames = validationLayers.data();
//         } else {
//             createInfo.enabledLayerCount = 0;
//         }

//         if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
//             throw std::runtime_error("failed to create logical device!");
//         }

//         vkGetDeviceQueue(device, 0, 0, &graphicsQueue);
//     }
// };

// int main() {
//     VulkanApplication app;

//     try {
//         app.run();
//     } catch (const std::exception& e) {
//         std::cerr << e.what() << std::endl;
//         return EXIT_FAILURE;
//     }

//     return EXIT_SUCCESS;
// }