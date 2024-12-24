#include "runtime/function/render/include/surface.h"

namespace VE
{
    bool Surface::tick(const FrameBuffer* framebuffer, UIState* uistate, SceneReleaseHandles& release_handles)
    {
        if (m_io->tick_NotQuit())
        {
            // if (framebuffer && framebuffer->m_scene && framebuffer_>m_render->m_loaded)
            if (true)
            {
                // m_rhi->tick_pre(framebuffer, release_handles);
                m_ui->tick_pre(uistate);
            }


            return true;
        }
        else
            return false;
    }
} // namespace VE
