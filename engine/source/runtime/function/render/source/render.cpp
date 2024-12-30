#include "runtime/function/render/include/render.h"

namespace VE
{
    void VirtualRenderer::initialize()
    {
        auto framebuffer = f_get_framebuffer(this);
        m_surface->initialize(framebuffer);
        // 
        // m_surface->initialize(nullptr);
    }


    bool VirtualRenderer::tick()
    {
        auto framebuffer = f_get_framebuffer(this);
        SceneReleaseHandles release_handles;
        bool tick_result = m_surface->tick(framebuffer, nullptr, release_handles);
        return true;   
    }
} // namespace VE
