#pragma once

#include "editor/include/axis.h"
#include "editor/include/editor_file_service.h"

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

        void drawAxisToggleButton(const char* string_id, bool check_state, EditorAxisMode axis_mode);

        void showEditorUI();
        void showEditorMenu(bool* p_open);
        void showEditorWorldObjectsWindow(bool* p_open);
        void showEditorFileContentWindow(bool* p_open);
        void showEditorGameWindow(bool* p_open);
        void showEditorDetailWindow(bool* p_open);

        void buildEditorFileAssetsUITree(EditorFileNode* node);
        void onFileContentItemClicked(EditorFileNode* node);

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

        Vector2 m_engine_window_pos{0.0f, 0.0f};
        Vector2 m_engine_window_size{1280.0f, 768.0f};
        float m_mouse_x{0.0f};
        float m_mouse_y{0.0f};

        bool m_is_editor_mode{true};

        EditorAxisMode m_axis_mode{EditorAxisMode::TranslateMode};

        // 0 for x, 1 for y, 2 for z
        size_t m_selected_gobject_id{0};
        // 0 for yoz, 1 for xoz, 2 for xoy
        Matrix4x4 m_selected_object_matrix{Matrix4x4::IDENTITY};

        EditorTranslationAxis m_translation_axis;
        EditorRotationAxis m_rotation_axis;
        EditorScaleAxis m_scale_axis;

        EditorFileService m_editor_file_service;
        std::chrono::time_point<std::chrono::steady_clock> m_last_file_tree_update;
    };


} // namespace VE
