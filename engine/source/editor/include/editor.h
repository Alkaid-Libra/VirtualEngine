#include "runtime/core/base/public_singleon.h"

namespace VE
{
    class EditorUI;
    class VirtualEngine;

    class VirtualEditor : public PublicSingleton<VirtualEditor>
    {
    public:
        virtual ~VirtualEditor();

        void initialize(VirtualEngine* engine_runtime);
        void clear();

        void run();

    protected:
        VirtualEditor();

        void onWindowChanged(float pos_x, float pos_y, float width, float height) const;
        size_t onUpdateCursorOnAxis(int axis_mode, const Vector2& cursor_uv, const Vector2& window_size) const;
        
    };
}