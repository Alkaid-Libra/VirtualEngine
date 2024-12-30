#pragma once

#include "runtime/core/base/public_singleton.h"

namespace VE
{
    class WorldManager : public PublicSingleton<WorldManager>
    {
        friend class PublicSingleton<WorldManager>;

    public:
        WorldManager(const WorldManager&) = delete;
        WorldManager& operator=(const WorldManager&) = delete;

        void initialize();

        void tick(float delta_time);

    protected:
        WorldManager() = default;
    };
} // namespace VE
