#pragma once

#include "runtime/function/render/include/framebuffer.h"
#include "runtime/function/render/include/vulkan_manager/vulkan_manager.h"
#include "runtime/function/render/include/surface_io.h"

namespace VE
{
    class VirtualRenderer;
    class VVulkanManager;
    class SurfaceRHI
    {
        VirtualRenderer* m_virtual_renderer;
        VRenderPath m_curpath{VRenderPath::Clustered};

    public:
        GLFWwindow* m_window;
        VVulkanManager* m_vulkan_manager;

        SurfaceRHI(VirtualRenderer* vrenderer) : m_virtual_renderer(vrenderer) {}
        
        int initialize(SurfaceIO* io, const FrameBuffer* framebuffer);
    };

    class SurfaceUI
    {
    public:
        UIState* m_tmp_uistate{nullptr};
        SurfaceRHI* m_rhi;
        std::shared_ptr<SurfaceIO> m_io;

    public:
        SurfaceUI() {}
        int initialize(SurfaceRHI* rhi, VirtualRenderer* vrenderer, std::shared_ptr<SurfaceIO> vio);

    protected:
        float getContentScale() const;
        float getIndentScale() const;
    };

    class Surface
    {
    public:
        Surface(VirtualRenderer* vrenderer, const char* title = "VirtualEngine", uint32_t width = 1280, uint32_t height = 768) :
        m_io(std::make_shared<SurfaceIO>(title, width, height)), m_rhi(std::make_shared<SurfaceRHI>(vrenderer)), m_render(vrenderer)
        {}

        int initialize(const FrameBuffer* framebuffer)
        {
            m_io->initialize();
            m_rhi->initialize(m_io.get(), framebuffer);
            return 0;
        }

        bool setSurfaceUI(std::shared_ptr<SurfaceUI> vui)
        {
            m_ui = vui;
            m_ui->initialize(m_rhi.get(), m_render, m_io);
            return true;
        }
        bool tick(const FrameBuffer* framebuffer, UIState* uistate, SceneReleaseHandles& release_handles);


    private:
        std::shared_ptr<SurfaceUI> m_ui;
        std::shared_ptr<SurfaceIO> m_io;
        std::shared_ptr<SurfaceRHI> m_rhi;
        VirtualRenderer* m_render = nullptr;
    };

} // namespace VE
