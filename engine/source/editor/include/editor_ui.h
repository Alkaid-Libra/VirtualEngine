#pragma once

#include "runtime/function/render/include/surface.h"

namespace VE
{
    class VirtualEditor;

    enum class EditorAxisMode : int
    {
        TranslateMode = 0,
        RotateMode = 1,
        ScaleMode = 2,
        Default = 3
    };

    class EditorUI : public SurfaceUI
    {
    public:
        EditorUI(VirtualEditor* editor);


    private:
        VirtualEditor* m_editor{nullptr};

    };


} // namespace VE
