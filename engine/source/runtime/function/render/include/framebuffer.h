#pragma once

#include "runtime/core/math/math_headers.h"

#include <memory>

namespace VE
{


    enum class VRenderPath : int
    {
        Forward,
        Deferred,
        Clustered
    };

    class Scene
    {

    };

    class UIState
    {
    public:
        bool m_writeLog{true};
        VRenderPath m_renderPath{VRenderPath::Forward};

        bool m_fullscreen{false};
        bool m_showUI{true};
        bool m_showConfigWindow{true};
        bool m_showLog{false};

        bool m_showStatsOverlay{false};
        struct
        {
            bool fps;
            bool frameTime;
            bool profiler;
            bool gpuMemory;
        } m_overlays{true, true, true, true};

        // std::shared_ptr<VCamera> m_editor_camera;
        
    };

    struct SceneReleaseHandles
    {

    };

    class FrameBuffer
    {
    public:
        FrameBuffer() : m_uistate(std::make_unique<UIState>()) {}
        
        size_t logicalFrameIndex{0};

        std::unique_ptr<UIState> m_uistate;
        std::shared_ptr<Scene> m_scene;
        VRenderPath m_renderpath{VRenderPath::Clustered};
    };
} // namespace VE
