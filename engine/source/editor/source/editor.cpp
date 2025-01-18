#include "editor/include/editor.h"
#include "editor/include/editor_ui.h"

#include "runtime/engine.h"
#include "runtime/function/render/include/render.h"

namespace VE
{
    VirtualEditor::VirtualEditor() {}
    VirtualEditor::~VirtualEditor() {}

    void VirtualEditor::initialize(VirtualEngine* engine_runtime)
    {
        assert(engine_runtime);

        m_engine_runtime = engine_runtime;
        m_editor_ui = std::make_shared<EditorUI>(this);

        std::shared_ptr<VirtualRenderer> render = m_engine_runtime->getRender();
        assert(render);

        render->setSurfaceUI(m_editor_ui);
    }
    void VirtualEditor::clear() {}

    void VirtualEditor::run()
    {
        assert(m_engine_runtime);
        assert(m_editor_ui);

        m_engine_runtime->run();
    }

    void VirtualEditor::onWindowChanged(float pos_x, float pos_y, float width, float height) const
    {
        std::shared_ptr<VirtualRenderer> render = m_engine_runtime->getRender();
        assert(render);

        render->updateWindow(pos_x, pos_y, width, height);
    }

} // namespace VE
