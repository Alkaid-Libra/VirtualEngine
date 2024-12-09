#include <iostream>
#include "runtime/engine.h"

// macro stringlize
#define VIRTUAL_XSTR(s) VIRTUAL_STR(s)
#define VIRTUAL_STR(s) #s

int main()
{
    std::filesystem::path virtual_root_folder = std::filesystem::path(VIRTUAL_XSTR(VIRTUAL_ROOT_DIR));
    std::cout << virtual_root_folder << std::endl;
    return 0;
}