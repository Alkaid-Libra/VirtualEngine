#pragma once

#include "runtime/function/render/include/surface.h"

namespace VE
{
    class VirtualRenderer
    {
    std::shared_ptr<Surface> m_surface;

    public:
        VirtualRenderer() : m_surface(std::make_shared<Surface>(this)) {}
       
        void initialize();

        bool tick();

        void setSurfaceUI(std::shared_ptr<SurfaceUI> vui) { m_surface->setSurfaceUI(vui); }
    };

} // namespace VE
