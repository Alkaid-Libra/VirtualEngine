#pragma once

#include "runtime/core/base/public_singleon.h"

#include "runtime/function/render/include/framebuffer.h"

namespace VE
{
    class SceneManager final : public PublicSingleton<SceneManager>
    {
        friend class PublicSingleton<SceneManager>;

    public:
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;

    protected:
        SceneManager() = default;

    public:
        void initialize();

        int tick(FrameBuffer* buffer);
    };
} // namespace VE
