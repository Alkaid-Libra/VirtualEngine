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

    public:
        EditorUI(VirtualEditor* editor);


    private:
        VirtualEditor* m_editor{nullptr};

        std::unordered_map<std::string, std::function<void(std::string, void*)>> m_editor_ui_creator;
        std::unordered_map<std::string, unsigned int> m_new_object_index_map;

    };


} // namespace VE





#pragma once

#include "function/ui/window_ui.h"

namespace Piccolo
{
    class EditorUI : public WindowUI
    {
    public:
        EditorUI();
        virtual void initialize(WindowUIInitInfo init_info) override final;
        // override: 当需要重写基类方法时，使用override关键字，这样可以将重写的检查工作交给编译器会去做
        // final: 重写函数到此为止，不能继续被重写
    };
} // namespace Piccolo
