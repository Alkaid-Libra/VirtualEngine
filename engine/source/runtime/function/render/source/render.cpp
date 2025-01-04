#include "runtime/function/render/include/render.h"

namespace VE
{
    void VirtualRenderer::initialize()
    {
        auto framebuffer = f_get_framebuffer(this);
        m_surface->initialize(framebuffer);
    }


    bool VirtualRenderer::tick()
    {
        auto framebuffer = f_get_framebuffer(this);
        SceneReleaseHandles release_handles;
        bool tick_result = m_surface->tick(framebuffer, framebuffer->m_uistate.get(), release_handles);
        
        return tick_result;
    }
} // namespace VE
