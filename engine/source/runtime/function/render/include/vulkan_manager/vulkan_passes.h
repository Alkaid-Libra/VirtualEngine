#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"

#include "runtime/function/render/include/vulkan_manager/vulkan_directional_light_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_point_light_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_pick_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_postprocess_pass.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_common.h"


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
    public:
        // 1: per mesh layout
        // 2: global layout
        // 3: per material layout
        // 4: sky box layout
        // 5: axis layout
        // 6: billboard type particle layout - keep particle forward to camera view
        enum LayoutType : uint8_t
        {
            _per_mesh = 0,
            _global,
            _material,
            _skybox,
            _axis,
            _particle,
            _layout_type_count
        };

        // 1: model
        // 2: sky box
        // 3: axis
        // 4: billboard type particle
        enum RenderPipelineType : uint8_t
        {
            _render_pipeline_type_model = 0,
            _render_pipeline_type_skybox,
            _render_pipeline_type_axis,
            _render_pipeline_type_particle,
            _render_pipeline_type_count
        };



        void initialize();

        void setHelperInfo(const VLightPassHelperInfo& helper_info);



    private:
        void setupRenderPass();
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();

        void setupModelGlobalDescriptorSet();
        void setupSkyboxDescriptorSet();
        void setupAxisDescriptorSet();
        void setupParticleDescriptorSet();

        void drawModel();
        void drawSkybox();
        void drawAxis();
        void drawBillboardParticle();

    private:
        VkImageView m_point_light_shadow_color_image_view;

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
