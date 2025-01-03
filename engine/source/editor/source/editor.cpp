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
} // namespace VE


#include "editor/include/editor_ui.h"
#include "function/global/global_context.h"
#include <cassert>

#include "editor/include/editor.h"
#include <memory>

namespace Piccolo
{
    void PiccoloEditor::initialize(PiccoloEngine* engine_runtime)
    {
        assert(engine_runtime);                // 确保engine正确被初始化了
        this->engine_runtime = engine_runtime; // 绑定engine_runtime

        editor_ui                     = std::make_shared<EditorUI>();
        WindowUIInitInfo ui_init_info = {g_runtime_global_context.m_window_system,
                                         g_runtime_global_context.m_render_system};
        editor_ui->initialize(ui_init_info);
    }
    void PiccoloEditor::run()
    {
        assert(engine_runtime);
        assert(editor_ui);
        float delta_time = 0;
        do
        {
            delta_time = engine_runtime->calculateDeltaTime();
        } while (engine_runtime->tickOneFrame(delta_time));
    }
    void PiccoloEditor::shutdown() {}
} // namespace Piccolo

