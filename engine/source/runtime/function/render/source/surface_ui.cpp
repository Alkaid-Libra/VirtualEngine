#include "runtime/function/render/include/surface.h"
#include "runtime/resource/config_manager/config_manager.h"


#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>


using namespace VE;

inline void windowContentScaleUpdate(float scale)
{
// #if defined(__MACH__)
//     float font_scale               = fmaxf(1.0f, scale);
//     ImGui::GetIO().FontGlobalScale = 1.0f / font_scale;
// #endif
// // TODO: Reload fonts if DPI scale is larger than previous font loading DPI scale    
}

inline void windowContentScaleCallback(GLFWwindow* window, float x_scale, float y_scale)
{
    windowContentScaleUpdate(fmaxf(x_scale, y_scale));
}


float SurfaceUI::getContentScale() const
{
    float x_scale, y_scale;
    glfwGetWindowContentScale(m_io->m_window, &x_scale, &y_scale);
    return fmaxf(1.0f, fmaxf(x_scale, y_scale));
}

float SurfaceUI::getIndentScale() const
{
#if defined(__MACH__)
    return 1.0f;
#else // Not tested on Linux
    return getContentScale();
#endif
}

int SurfaceUI::initialize(SurfaceRHI* rhi, VirtualRenderer* vrenderer, std::shared_ptr<SurfaceIO> vio)
{
    m_io = vio;
    m_rhi = rhi;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigDockingAlwaysTabBar = true;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    float content_scale = getContentScale();
    windowContentScaleUpdate(content_scale);
    glfwSetWindowContentScaleCallback(vio->m_window, windowContentScaleCallback);




    return 0;
}