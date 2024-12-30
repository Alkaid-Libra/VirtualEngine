#include <iostream>

#include "editor/include/editor.h"
#include "runtime/engine.h"

// macro stringlize
#define VIRTUAL_XSTR(s) VIRTUAL_STR(s)
#define VIRTUAL_STR(s) #s



int main()
{
    std::filesystem::path virtual_root_folder = std::filesystem::path(VIRTUAL_XSTR(VIRTUAL_ROOT_DIR));
    // std::cout << virtual_root_folder << std::endl;

    VE::EngineInitParams params;
    params.m_root_folder = virtual_root_folder;
    params.m_config_file_path = virtual_root_folder / "VirtualEditor.ini";

    VE::VirtualEngine::getInstance().startEngine(params);
    VE::VirtualEngine::getInstance().initialize();


    VE::VirtualEditor::getInstance().initialize(&(VE::VirtualEngine::getInstance()));
    VE::VirtualEditor::getInstance().run();




    // VE::VirtualEngine::getInstance().shutdownEngine();


    // std::cout << params.m_config_file_path << std::endl;
    return 0;
}