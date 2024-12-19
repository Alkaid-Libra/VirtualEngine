#include "runtime/function/render/include/surface.h"

namespace VE
{
    bool Surface::tick(const FrameBuffer* framebuffer, UIState* uistate, SceneReleaseHandles& release_handles)
    {
        if (m_io->tick_NotQuit())
        {



            return true;
        }
        else
            return false;
    }
} // namespace VE
