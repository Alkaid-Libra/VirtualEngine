#include "editor/include/editor_ui.h"

#include "editor/include/editor.h"

#include "runtime/core/base/macro.h"

#include "runtime/platform/path/path.h"

#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/framework/component/transform/transform_component.h"

#include "runtime/function/framework/level/level.h"
#include "runtime/function/framework/world/world_manager.h"

#include "runtime/function/scene/scene_manager.h"


#include <imgui.h>
#include <imgui_internal.h>

namespace VE
{
    int g_node_depth = -1;
    std::vector<std::pair<std::string, bool>> g_editor_node_state_array;

    void DrawVecControl(const std::string& label, VE::Vector3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
    void DrawVecControl(const std::string& label, VE::Quaternion& values, float resetValue = 0.0f, float columnWidth = 100.0f);

    EditorUI::EditorUI(VirtualEditor* editor) : m_editor(editor)
    {
        Path& path_service = Path::getInstance();
        const auto& asset_folder = ConfigManager::getInstance().getAssetFolder();
        
        m_editor_ui_creator["TreeNodePush"] = [this](std::string name, void* value_ptr) -> void {
            static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings;
            bool node_state = false;
            g_node_depth++;

            if (g_node_depth > 0)
            {
                if (g_editor_node_state_array[g_node_depth - 1].second)
                {
                    node_state = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
                }
                else
                {
                    g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
                    return;
                }
            }
            else
            {
                node_state = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
            }
            g_editor_node_state_array.emplace_back(std::pair(name.c_str(), node_state));
        };

        m_editor_ui_creator["TreeNodePop"] = [this](std::string name, void* value_ptr) -> void {
            if (g_editor_node_state_array[g_node_depth].second)
            {
                ImGui::TreePop();
            }
            g_editor_node_state_array.pop_back();
            g_node_depth--;
        };

        m_editor_ui_creator["Transform"] = [this](std::string name, void* value_ptr) -> void {
            if (g_editor_node_state_array[g_node_depth].second)
            {
                Transform* trans_ptr = static_cast<Transform*>(value_ptr);

                DrawVecControl("Position", trans_ptr->m_position);
                DrawVecControl("Rotation", trans_ptr->m_rotation);
                DrawVecControl("Scale", trans_ptr->m_scale);

                // drawSelectedEntityAxis();
            }
        };

        m_editor_ui_creator["int"] = [this](std::string name, void* value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::InputInt(label.c_str(), static_cast<int*>(value_ptr));
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::InputInt(full_label.c_str(), static_cast<int*>(value_ptr));
                }
            }
        };

        m_editor_ui_creator["float"] = [this](std::string name, void* value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::InputFloat(label.c_str(), static_cast<float*>(value_ptr));
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::InputFloat(full_label.c_str(), static_cast<float*>(value_ptr));
                }
            }
        };

        m_editor_ui_creator["Vector3"] = [this](std::string name, void* value_ptr) -> void {
            Vector3* vec_ptr = static_cast<Vector3*>(value_ptr);
            float    val[3]  = {vec_ptr->x, vec_ptr->y, vec_ptr->z};
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::DragFloat3(label.c_str(), val);
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::DragFloat3(full_label.c_str(), val);
                }
            }
            vec_ptr->x = val[0];
            vec_ptr->y = val[1];
            vec_ptr->z = val[2];
        };

        m_editor_ui_creator["Quaternion"] = [this](std::string name, void* value_ptr) -> void {
            Quaternion* qua_ptr = static_cast<Quaternion*>(value_ptr);
            float       val[4]  = {qua_ptr->x, qua_ptr->y, qua_ptr->z, qua_ptr->w};
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::DragFloat4(label.c_str(), val);
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    ImGui::DragFloat4(full_label.c_str(), val);
                }
            }
            qua_ptr->x = val[0];
            qua_ptr->y = val[1];
            qua_ptr->z = val[2];
            qua_ptr->w = val[3];
        };

        m_editor_ui_creator["std::string"] = [this, &path_service, &asset_folder](std::string name,
                                                                                  void*       value_ptr) -> void {
            if (g_node_depth == -1)
            {
                std::string label = "##" + name;
                ImGui::Text("%s", name.c_str());
                ImGui::SameLine();
                ImGui::Text("%s", (*static_cast<std::string*>(value_ptr)).c_str());
            }
            else
            {
                if (g_editor_node_state_array[g_node_depth].second)
                {
                    std::string full_label = "##" + getLeafUINodeParentLabel() + name;
                    ImGui::Text("%s", (name + ":").c_str());
                    std::string value_str = *static_cast<std::string*>(value_ptr);
                    if (value_str.find_first_of('/') != std::string::npos)
                    {
                        std::filesystem::path value_path(value_str);
                        if (value_path.is_absolute())
                        {
                            value_path = path_service.getRelativePath(asset_folder, value_path);
                        }
                        value_str = value_path.generic_string();
                        if (value_str.size() >= 2 && value_str[0] == '.' && value_str[1] == '.')
                        {
                            value_str.clear();
                        }
                    }
                    ImGui::Text("%s", value_str.c_str());
                }
            }
        };
    }

    void DrawVecControl(const std::string& label, VE::Vector3& values, float resetValue, float columnWidth)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.45f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.55f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.45f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }
    void DrawVecControl(const std::string& label, VE::Quaternion& values, float resetValue, float columnWidth)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.45f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.55f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.45f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.5f, 0.25f, 0.5f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.6f, 0.35f, 0.6f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.5f, 0.25f, 0.5f, 1.0f});
        if (ImGui::Button("W", buttonSize))
            values.w = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##W", &values.w, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }

    // void EditorUI::drawSelectedEntityAxis()
    // {
    //     GObject* selected_object = getSelectedGObject();

    //     if (m_is_editor_mode && selected_object != nullptr)
    //     {
    //         const TransformComponent* transform_component = selected_object->tryGetComponentConst(TransformComponent);
    //         std::vector<RenderMesh>   axis_meshs;

    //         Vector3    scale;
    //         Quaternion rotation;
    //         Vector3    translation;
    //         transform_component->getMatrix().decomposition(translation, scale, rotation);
    //         Matrix4x4 translation_matrix = Matrix4x4::getTrans(translation);
    //         Matrix4x4 scale_matrix       = Matrix4x4::buildScaleMatrix(1.0f, 1.0f, 1.0f);
    //         Matrix4x4 axis_model_matrix  = translation_matrix * scale_matrix;
    //         if (m_axis_mode == EditorAxisMode::TranslateMode)
    //         {
    //             m_translation_axis.m_model_matrix = axis_model_matrix;
    //             axis_meshs.push_back(m_translation_axis);
    //             SceneManager::getInstance().setAxisMesh(axis_meshs);
    //         }
    //         else if (m_axis_mode == EditorAxisMode::RotateMode)
    //         {
    //             m_rotation_axis.m_model_matrix = axis_model_matrix;
    //             axis_meshs.push_back(m_rotation_axis);
    //             SceneManager::getInstance().setAxisMesh(axis_meshs);
    //         }
    //         else if (m_axis_mode == EditorAxisMode::ScaleMode)
    //         {
    //             axis_model_matrix           = axis_model_matrix * Matrix4x4(rotation);
    //             m_scale_aixs.m_model_matrix = axis_model_matrix;
    //             axis_meshs.push_back(m_scale_aixs);
    //             SceneManager::getInstance().setAxisMesh(axis_meshs);
    //         }
    //     }
    //     else
    //     {
    //         std::vector<RenderMesh> axis_meshs;
    //         SceneManager::getInstance().setAxisMesh(axis_meshs);
    //     }
    // }


    std::string EditorUI::getLeafUINodeParentLabel()
    {
        std::string parent_label;
        int         array_size = g_editor_node_state_array.size();
        for (int index = 0; index < array_size; index++)
        {
            parent_label += g_editor_node_state_array[index].first + "::";
        }
        return parent_label;
    }

    void EditorUI::drawSelectedEntityAxis()
    {
        GObject* selected_object = getSelectedGObject();
        if (m_is_editor_mode && selected_object != nullptr)
        {
            const TransformComponent* transform_component = selected_object->tryGetComponentConst(TransformComponent);
            std::vector<RenderMesh> axis_meshes;

            Vector3 scale;
            Quaternion rotation;
            Vector3 translation;
            transform_component->getMatrix().decomposition(translation, scale, rotation);
            Matrix4x4 translation_matrix = Matrix4x4::getTrans(translation);
            Matrix4x4 scale_matrix = Matrix4x4::buildScaleMatrix(1.0f, 1.0f, 1.0f);
            Matrix4x4 axis_model_matrix = translation_matrix * scale_matrix;
            if (m_axis_mode == EditorAxisMode::TranslateMode)
            {
                m_translation_axis.m_model_matrix = axis_model_matrix;
                axis_meshes.push_back(m_translation_axis);
                SceneManager::getInstance().setAxisMesh(axis_meshes);
            }   
            else if (m_axis_mode == EditorAxisMode::RotateMode)
            {
                m_rotation_axis.m_model_matrix = axis_model_matrix;
                axis_meshes.push_back(m_rotation_axis);
                SceneManager::getInstance().setAxisMesh(axis_meshes);
            }
            else if (m_axis_mode == EditorAxisMode::ScaleMode)
            {
                axis_model_matrix = axis_model_matrix * Matrix4x4(rotation);
                m_scale_axis.m_model_matrix = axis_model_matrix;
                axis_meshes.push_back(m_scale_axis);
                SceneManager::getInstance().setAxisMesh(axis_meshes);
            }
        }
        else
        {
            std::vector<RenderMesh> axis_meshes;
            SceneManager::getInstance().setAxisMesh(axis_meshes);
        }
    }

    void EditorUI::onTick(UIState* uistate)
    {
        showEditorUI();
        processEditorCommand();
    }

    void EditorUI::showEditorUI()
    {
        bool editor_menu_window_open = true;
        bool asset_window_open = true;
        bool game_engine_window_open = true;
        bool file_content_window_open = true;
        bool detail_window_open = true;
        bool scene_lights_window_open = true;
        bool scene_lights_data_window_open = true;

        showEditorMenu(&editor_menu_window_open);
        showEditorWorldObjectsWindow(&asset_window_open);
        showEditorGameWindow(&game_engine_window_open);
        showEditorFileContentWindow(&file_content_window_open);
        showEditorDetailWindow(&detail_window_open);
    }
    void EditorUI::processEditorCommand()
    {
        float camera_speed = 0.05f;
        // Quaternion camera_rotate = m_tmp_uistate->m_editor_camera->rotation().inverse();
    }

    GObject *EditorUI::getSelectedGObject() const
    {
        GObject* selected_object = nullptr;
        if (m_selected_gobject_id != VIRTUAL_INVALID_GOBJECT_ID)
        {
            Level* level = WorldManager::getInstance().getCurrentActiveLevel();
            if (level != nullptr)
            {
                selected_object = level->getGObjectByID(m_selected_gobject_id);
            }
        }

        return selected_object;
    }

    void EditorUI::onGObjectSelected(size_t selected_gobject_id)
    {
        if (selected_gobject_id == m_selected_gobject_id)
            return;

        m_selected_gobject_id = selected_gobject_id;

        GObject* selected_gobject = getSelectedGObject();
        if (selected_gobject)
        {
            const TransformComponent* transform_component = selected_gobject->tryGetComponentConst(TransformComponent);
            m_selected_object_matrix = transform_component->getMatrix();
        }

        drawSelectedEntityAxis();

        if (m_selected_gobject_id != VIRTUAL_INVALID_GOBJECT_ID)
        {
            LOG_INFO("select game object " + std::to_string(m_selected_gobject_id));
        }
        else
        {
            LOG_INFO("no game object selected");
        }
    }



    void EditorUI::showEditorMenu(bool *p_open)
    {
        ImGuiDockNodeFlags dock_flags   = ImGuiDockNodeFlags_DockSpace;
        ImGuiWindowFlags   window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar |
                                        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground |
                                        ImGuiConfigFlags_NoMouseCursorChange | ImGuiWindowFlags_NoBringToFrontOnFocus;
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(main_viewport->WorkPos, ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(m_io->m_width, m_io->m_height), ImGuiCond_Always);
        ImGui::SetNextWindowViewport(main_viewport->ID);

        ImGui::Begin("Editor menu", p_open, window_flags);

        ImGuiID main_docking_id = ImGui::GetID("Main Docking");
        if (ImGui::DockBuilderGetNode(main_docking_id) == nullptr)
        {
            ImGui::DockBuilderRemoveNode(main_docking_id);

            ImGui::DockBuilderAddNode(main_docking_id, dock_flags);
            ImGui::DockBuilderSetNodePos(main_docking_id,
                                         ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y + 18.0f));
            ImGui::DockBuilderSetNodeSize(main_docking_id, ImVec2(m_io->m_width, m_io->m_height - 18.0f));

            ImGuiID center = main_docking_id;
            ImGuiID left;
            ImGuiID right = ImGui::DockBuilderSplitNode(center, ImGuiDir_Right, 0.25f, nullptr, &left);

            ImGuiID left_other;
            ImGuiID left_file_content = ImGui::DockBuilderSplitNode(left, ImGuiDir_Down, 0.30f, nullptr, &left_other);

            ImGuiID left_game_engine;
            ImGuiID left_asset =
                ImGui::DockBuilderSplitNode(left_other, ImGuiDir_Left, 0.30f, nullptr, &left_game_engine);

            ImGui::DockBuilderDockWindow("World Objects", left_asset);
            ImGui::DockBuilderDockWindow("Components Details", right);
            ImGui::DockBuilderDockWindow("File Content", left_file_content);
            ImGui::DockBuilderDockWindow("Game Engine", left_game_engine);

            ImGui::DockBuilderFinish(main_docking_id);
        }

        ImGui::DockSpace(main_docking_id);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu"))
            {
                if (ImGui::MenuItem("Reload Current Level"))
                {
                    WorldManager::getInstance().reloadCurrentLevel();
                    onGObjectSelected(VIRTUAL_INVALID_GOBJECT_ID);
                }
                if (ImGui::MenuItem("Save Current Level"))
                {
                    WorldManager::getInstance().saveCurrentLevel();
                }
                if (ImGui::MenuItem("Exit"))
                {
                    exit(0);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
    void EditorUI::showEditorWorldObjectsWindow(bool* p_open)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        if (!ImGui::Begin("World Objects", p_open, window_flags))
        {
            ImGui::End();
            return;
        }

        // const Level* current_active_level = WorldManager::getInstance().getCurrentActiveLevel();
        // if (current_active_level == nullptr)
        //     return;

        // const auto& all_gobjects = current_active_level->getAllGObjects();
        // for (auto& id_object_pair : all_gobjects)
        // {
        //     const size_t      object_id = id_object_pair.first;
        //     GObject*          object    = id_object_pair.second;
        //     const std::string name      = object->getName();
        //     if (name.size() > 0)
        //     {
        //         if (ImGui::Selectable(name.c_str(), m_selected_gobject_id == object_id))
        //         {
        //             if (m_selected_gobject_id != object_id)
        //             {
        //                 onGObjectSelected(object_id);
        //             }
        //             else
        //             {
        //                 onGObjectSelected(PILOT_INVALID_GOBJECT_ID);
        //             }
        //             break;
        //         }
        //     }
        // }
        ImGui::End();
    }
    void EditorUI::showEditorGameWindow(bool* p_open)
    {
        ImGuiIO&         io           = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        if (!ImGui::Begin("Game Engine", p_open, window_flags))
        {
            ImGui::End();
            return;
        }

        static bool trans_button_ckecked  = false;
        static bool rotate_button_ckecked = false;
        static bool scale_button_ckecked  = false;

        // switch (m_axis_mode)
        // {
        //     case EditorAxisMode::TranslateMode:
        //         trans_button_ckecked  = true;
        //         rotate_button_ckecked = false;
        //         scale_button_ckecked  = false;
        //         break;
        //     case EditorAxisMode::RotateMode:
        //         trans_button_ckecked  = false;
        //         rotate_button_ckecked = true;
        //         scale_button_ckecked  = false;
        //         break;
        //     case EditorAxisMode::ScaleMode:
        //         trans_button_ckecked  = false;
        //         rotate_button_ckecked = false;
        //         scale_button_ckecked  = true;
        //         break;
        //     default:
        //         break;
        // }

        if (ImGui::BeginMenuBar())
        {
            // ImGui::Indent(10.f);
            // drawAxisToggleButton("Trans", trans_button_ckecked, EditorAxisMode::TranslateMode);
            // ImGui::Unindent();

            // ImGui::SameLine();

            // drawAxisToggleButton("Rotate", rotate_button_ckecked, EditorAxisMode::RotateMode);

            // ImGui::SameLine();

            // drawAxisToggleButton("Scale", scale_button_ckecked, EditorAxisMode::ScaleMode);

            // ImGui::SameLine();

            // float indent_val = 0.0f;
            // indent_val       = m_engine_window_size.x - 100.0f * getIndentScale();

            // ImGui::Indent(indent_val);
            // if (m_is_editor_mode)
            // {
            //     ImGui::PushID("Editor Mode");
            //     ImGui::Button("Editor Mode");
            //     if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            //     {
            //         m_is_editor_mode = !m_is_editor_mode;
            //         drawSelectedEntityAxis();
            //         g_is_editor_mode = false;
            //         m_io->setFocusMode(true);
            //     }
            //     ImGui::PopID();
            // }
            // else
            // {
            //     ImGui::Button("Game Mode");
            //     if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            //     {
            //         m_is_editor_mode = !m_is_editor_mode;
            //         g_is_editor_mode = true;
            //         SceneManager::getInstance().setMainViewMatrix(m_tmp_uistate->m_editor_camera->getViewMatrix());
            //     }
            // }
            // m_io->setEditorMode(m_is_editor_mode);
            // ImGui::Unindent();
            ImGui::EndMenuBar();
        }

//         if (!m_is_editor_mode)
//         {
//             ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Press Left Alt key to display the mouse cursor!");
//         }

//         auto menu_bar_rect = ImGui::GetCurrentWindow()->MenuBarRect();

//         Vector2 new_window_pos  = {0.0f, 0.0f};
//         Vector2 new_window_size = {0.0f, 0.0f};
//         new_window_pos.x        = ImGui::GetWindowPos().x;
//         new_window_pos.y        = ImGui::GetWindowPos().y + menu_bar_rect.Min.y;
//         new_window_size.x       = ImGui::GetWindowSize().x;
//         new_window_size.y       = ImGui::GetWindowSize().y - menu_bar_rect.Min.y;

//         // if (new_window_pos != m_engine_window_pos || new_window_size != m_engine_window_size)
//         {
// #if defined(__GNUC__)
//             m_editor->onWindowChanged(new_window_pos.x, new_window_pos.y, new_window_size.x, new_window_size.y);
// #endif

//             m_engine_window_pos  = new_window_pos;
//             m_engine_window_size = new_window_size;
//             SceneManager::getInstance().setWindowSize(m_engine_window_size);
//         }

        ImGui::End();
    }
    void EditorUI::showEditorFileContentWindow(bool* p_open)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        if (!ImGui::Begin("File Content", p_open, window_flags))
        {
            ImGui::End();
            return;
        }

        static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH |
                                       ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg |
                                       ImGuiTableFlags_NoBordersInBody;

        if (ImGui::BeginTable("File Content", 2, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            // auto current_time = std::chrono::steady_clock::now();
            // if (current_time - m_last_file_tree_update > std::chrono::seconds(1))
            // {
            //     m_editor_file_service.buildEngineFileTree();
            //     m_last_file_tree_update = current_time;
            // }
            // m_last_file_tree_update = current_time;

            // EditorFileNode* editor_root_node = m_editor_file_service.getEditorRootNode();
            // buildEditorFileAssstsUITree(editor_root_node);
            ImGui::EndTable();
        }

        // file image list

        ImGui::End();
    }
    void EditorUI::showEditorDetailWindow(bool* p_open)
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();

        if (!ImGui::Begin("Components Details", p_open, window_flags))
        {
            ImGui::End();
            return;
        }

        // GObject* selected_object = getSelectedGObject();
        // if (selected_object == nullptr)
        // {
        //     ImGui::End();
        //     return;
        // }

        // const std::string& name = selected_object->getName();
        // static char        cname[128];
        // memset(cname, 0, 128);
        // memcpy(cname, name.c_str(), name.size());

        ImGui::Text("Name");
        ImGui::SameLine();
        // ImGui::InputText("##Name", cname, IM_ARRAYSIZE(cname), ImGuiInputTextFlags_ReadOnly);

        // static ImGuiTableFlags flags                      = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings;
        // auto&&                 selected_object_components = selected_object->getComponents();
        // for (auto component_ptr : selected_object_components)
        // {
        //     m_editor_ui_creator["TreeNodePush"](("<" + component_ptr.getTypeName() + ">").c_str(), nullptr);
        //     auto object_instance = Reflection::ReflectionInstance(
        //         Pilot::Reflection::TypeMeta::newMetaFromName(component_ptr.getTypeName().c_str()),
        //         component_ptr.operator->());
        //     createComponentUI(object_instance);
        //     m_editor_ui_creator["TreeNodePop"](("<" + component_ptr.getTypeName() + ">").c_str(), nullptr);
        // }
        ImGui::End();
    }






} // namespace VE
