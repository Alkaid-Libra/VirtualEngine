#pragma once

#include "runtime/function/render/include/surface.h"

namespace VE
{
    template<typename R, typename S>
    struct VRegister
    {
        typedef std::function<const R(S s)> GetPtr;
        typedef std::function<const R(S s)> FuncPtr;
    };


    class VirtualRenderer
    {
        std::shared_ptr<Surface> m_surface;

    public:
        VirtualRenderer() : m_surface(std::make_shared<Surface>(this)) {}
       
        void initialize();

        bool tick();

        void setSurfaceUI(std::shared_ptr<SurfaceUI> pui) { m_surface->setSurfaceUI(pui); }

        bool ResgisterGetPtr(VRegister<const FrameBuffer*, const VirtualRenderer*>::GetPtr get)
        {
            f_get_framebuffer = get;
            return true;
        }

        void updateWindow(float pos_x, float pos_y, float width, float height) const;


        VRegister<const FrameBuffer*, const VirtualRenderer*>::GetPtr f_get_framebuffer;
    };

} // namespace VE
