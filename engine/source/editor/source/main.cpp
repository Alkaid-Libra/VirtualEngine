#include <iostream>

#include "editor/include/editor.h"
#include "runtime/engine.h"

// macro stringlize
#define VIRTUAL_XSTR(s) VIRTUAL_STR(s)
#define VIRTUAL_STR(s) #s

#include "runtime/core/meta/meta_example.h"

int main()
{
    std::filesystem::path virtual_root_folder = std::filesystem::path(VIRTUAL_XSTR(VIRTUAL_ROOT_DIR));
    // std::cout << virtual_root_folder << std::endl;

    VE::EngineInitParams params;
    params.m_root_folder = virtual_root_folder;
    params.m_config_file_path = virtual_root_folder / "VirtualEditor.ini";

    // VE::VirtualEngine::getInstance().startEngine(params);
    // VE::VirtualEngine::getInstance().initialize(); // zero

    // VE::VirtualEditor::getInstance().initialize(&(VE::VirtualEngine::getInstance()));
    // VE::VirtualEditor::getInstance().run();
    // VE::VirtualEditor::getInstance().clear(); // zero

    // VE::VirtualEngine::getInstance().clear(); // zero
    // VE::VirtualEngine::getInstance().shutdownEngine();


    // VE::VirtualEngine::getInstance().initialize(params.m_config_file_path.generic_string());
    // VE::VirtualEditor::getInstance().run();
        // 引擎创建
    Piccolo::PiccoloEngine* engine = new Piccolo::PiccoloEngine();
    engine->initialize(params.m_config_file_path.generic_string()); // 通过config文件初始化引擎

    // 编辑器创建
    Piccolo::PiccoloEditor* editor = new Piccolo::PiccoloEditor();
    editor->initialize(engine);

    // 开始运行编辑器及其引擎
    editor->run();

    // 关机的收尾工作
    editor->shutdown();
    engine->shutdown();


    // std::cout << params.m_config_file_path << std::endl;
    return 0;
}