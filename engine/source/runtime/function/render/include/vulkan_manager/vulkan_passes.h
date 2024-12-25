#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"

#include "runtime/function/render/include/vulkan_manager/vulkan_directional_light_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_point_light_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_pick_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_postprocess_pass.h"


// 
#include <string>

namespace VE
{
    struct VLightPassHelperInfo
    {
        VkImageView point_light_shadow_color_image_view;
        VkImageView directional_light_shadow_color_image_view;
    };


    class VLightingPass : public VRenderPassBase
    {

    };

    // class VTrianglePass : public VRenderPassBase
    // {


    // public:
    //     void setupRenderPass();
    //     void setupPipelines();
    //     void setupFramebuffer();

    //     // 
    //     void createFramebuffers();
    //     std::vector<char> readFile(const std::string& filename);
    // };
} // namespace VE
