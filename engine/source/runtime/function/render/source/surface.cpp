#include "runtime/function/render/include/surface.h"

namespace VE
{
    bool Surface::tick(const FrameBuffer* framebuffer, UIState* uistate, SceneReleaseHandles& release_handles)
    {
        if (m_io->tick_NotQuit())
        {
            // if (framebuffer && framebuffer->m_scene && framebuffer->m_scene->m_loaded)
            if (framebuffer)
            {
                m_rhi->tick_pre(framebuffer, release_handles);
                m_ui->tick_pre(uistate);

                m_ui->tick_post(uistate);
                m_rhi->tick_post(framebuffer);
            }


            return true;
        }
        else
            return false;
    }

    void Surface::updateWindow(float pos_x, float pos_y, float width, float height) const
    {
        VkViewport viewport;
        viewport.x = pos_x;
        viewport.y = pos_y;
        viewport.width = width;
        viewport.height = height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        m_ui->m_tmp_uistate->m_editor_camera->setAspect(width / height);
        m_rhi->m_vulkan_manager->updateUIRenderSceneViewport(viewport);
    }
} // namespace VE
