#pragma once

#include "runtime/function/render/include/framebuffer.h"

namespace VE
{
    class EditorTranslationAxis : public RenderMesh
    {
    public:
        EditorTranslationAxis();
    };
    
    class EditorRotationAxis : public RenderMesh
    {
    public:
        EditorRotationAxis();
    };
    class EditorScaleAxis : public RenderMesh
    {
    public:
        EditorScaleAxis();
    };
} // namespace VE
