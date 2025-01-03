#pragma once

#include "runtime/core/base/public_singleton.h"
#include "runtime/core/math/vector2.h"

namespace VE
{
    class EditorUI;
    class VirtualEngine;

    class VirtualEditor : public PublicSingleton<VirtualEditor>
    {
        friend class PublicSingleton<VirtualEditor>;

    public:
        virtual ~VirtualEditor();

        void initialize(VirtualEngine* engine_runtime);
        void clear();

        void run();

    protected:
        VirtualEditor();

        void onWindowChanged(float pos_x, float pos_y, float width, float height) const;
        size_t onUpdateCursorOnAxis(int axis_mode, const Vector2& cursor_uv, const Vector2& window_size) const;
        
        std::shared_ptr<EditorUI> m_editor_ui;
        VirtualEngine* m_engine_runtime{nullptr};
    };
}



#pragma once

#include <memory>

#include "engine.h"

#include "editor/include/editor_ui.h"

namespace Piccolo
{
    class PiccoloEditor
    {
    public:
        void initialize(PiccoloEngine* engine_runtime);
        void run();
        void shutdown();

    protected:
        std::shared_ptr<EditorUI> editor_ui;
        PiccoloEngine*            engine_runtime = nullptr;
    };
} // namespace Piccolo