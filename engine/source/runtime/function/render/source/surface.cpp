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
} // namespace VE
