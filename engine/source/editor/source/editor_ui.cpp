#include "editor/include/editor_ui.h"

#include "editor/include/editor.h"

#include "runtime/platform/path/path.h"

#include "runtime/resource/config_manager/config_manager.h"


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
    
} // namespace VE



#include "editor/include/editor_ui.h"

namespace Piccolo
{
    EditorUI::EditorUI() {}
    void EditorUI::initialize(WindowUIInitInfo init_info) {}
} // namespace Piccolo
