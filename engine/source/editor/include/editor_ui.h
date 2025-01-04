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
    private:

        std::string getLeafUINodeParentLabel();
        void drawSelectedEntityAxis();

        void showEditorUI();
        void showEditorMenu(bool* p_open);
        void showEditorWorldObjectsWindow(bool* p_open);
        void showEditorFileContentWindow(bool* p_open);
        void showEditorGameWindow(bool* p_open);
        void showEditorDetailWindow(bool* p_open);

        void processEditorCommand();

    public:
        EditorUI(VirtualEditor* editor);

        void onTick(UIState* uistate) override;


    private:
        VirtualEditor* m_editor{nullptr};

        std::unordered_map<std::string, std::function<void(std::string, void*)>> m_editor_ui_creator;
        std::unordered_map<std::string, unsigned int> m_new_object_index_map;

    };


} // namespace VE
