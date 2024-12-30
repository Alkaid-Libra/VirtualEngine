#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"

namespace VE
{
    class VPointLightPass : public VRenderPassBase
    {
    public:
        void initialize();

        void postInitialize();

        VkDescriptorSetLayout _per_mesh_layout;


    private:
        void setupAttachments();
        void setupRenderPass();
        void setupFramebuffer();
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
        void drawModel();
    };
} // namespace VE
