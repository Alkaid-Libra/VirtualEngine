#pragma once

#include "editor/include/axis.h"
#include "runtime/function/render/include/surface.h"

#include "runtime/function/framework/object/object.h"

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

        GObject* getSelectedGObject() const;
        void onGObjectSelected(size_t selected_gobject_id);

    public:
        EditorUI(VirtualEditor* editor);

        void onTick(UIState* uistate) override;


    private:
        VirtualEditor* m_editor{nullptr};

        std::unordered_map<std::string, std::function<void(std::string, void*)>> m_editor_ui_creator;
        std::unordered_map<std::string, unsigned int> m_new_object_index_map;

        bool m_is_editor_mode{true};

        EditorAxisMode m_axis_mode{EditorAxisMode::TranslateMode};

        // 0 for x, 1 for y, 2 for z
        size_t m_selected_gobject_id{0};
        // 0 for yoz, 1 for xoz, 2 for xoy
        Matrix4x4 m_selected_object_matrix{Matrix4x4::IDENTITY};

        EditorTranslationAxis m_translation_axis;
        EditorRotationAxis m_rotation_axis;
        EditorScaleAxis m_scale_axis;

    };


} // namespace VE
