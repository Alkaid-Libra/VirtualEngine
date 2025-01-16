#pragma once

#include "runtime/core/base/public_singleton.h"

#include "runtime/resource/res_type/common/world.h"
// #include "runtime/resource/res_type/data/world.h"

#include <filesystem>

// 
#include <vector>
#include <string>

namespace VE
{
    class Level;

    class WorldManager : public PublicSingleton<WorldManager>
    {
        friend class PublicSingleton<WorldManager>;

    public:
        WorldManager(const WorldManager&) = delete;
        WorldManager& operator=(const WorldManager&) = delete;

        void initialize();
        void clear();

        void reloadCurrentLevel();
        void saveCurrentLevel();

        void tick(float delta_time);

        Level* getCurrentActiveLevel() const { return m_current_active_level; }

    protected:
        WorldManager() = default;

    private:
        void loadLevel(const std::string& level_url);
        void loadWorld(const WorldRes& pending_load_world);

        void processPendingLoadWorld();

        Level* m_current_active_level{nullptr};
        std::vector<Level*> m_levels;

        std::filesystem::path m_pending_load_world_path;
        std::string m_current_world_name;
    };
} // namespace VE
