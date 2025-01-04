#include "runtime/function/scene/scene_manager.h"

#include "runtime/function/render/include/framebuffer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace VE
{
    void SceneManager::initialize() {}

    int SceneManager::tick(FrameBuffer* buffer)
    {
        return 0;
    }
} // namespace VE
