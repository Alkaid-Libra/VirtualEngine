#pragma once

#include "runtime/core/base/public_singleton.h"

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
        std::shared_ptr<Scene> m_scene = std::make_shared<Scene>();

    public:
        void initialize();

        int tick(FrameBuffer* buffer);



        std::shared_ptr<Scene> getCurrentScene() const { return m_scene; }
    };
} // namespace VE
