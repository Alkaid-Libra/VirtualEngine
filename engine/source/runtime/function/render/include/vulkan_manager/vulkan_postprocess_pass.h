#include "runtime/function/render/include/vulkan_manager/vulkan_render_pass.h"

namespace VE
{
    class VPostprocessPass : public VRenderPassBase
    {
    public:
        void initialize();

        void updateAfterFramebufferRecreate();
        
        // post-process pass is the second subpass of the common lighting pass
        VkRenderPass _render_pass;


    private:
        void setupAttachments();
        void setupRenderPass();
        void setupFramebuffer();
        void setupDescriptorSetLayout();
        void setupPipelines();
        void setupDescriptorSet();
    };
} // namespace VE
