#include "runtime/function/framework/world/world_manager.h"

#include "runtime/resource/asset_manager/asset_manager.h"
#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/framework/level/level.h"

#include <unordered_set>
// 
#include <algorithm>
#include <string>
#include <iostream>
#include "runtime/resource/res_type/data/material.h"

namespace VE
{
    void WorldManager::initialize() 
    { 
        m_pending_load_world_path = ConfigManager::getInstance().getDefaultWorldPath();
    }

    void WorldManager::clear()
    {
        m_pending_load_world_path.clear();
        m_current_world_name.clear();

        m_current_active_level = nullptr;
        for (Level* level : m_levels)
        {
            level->clear();
            delete level;
        }
        m_levels.clear();
    }

    void WorldManager::reloadCurrentLevel()
    {
        if (m_current_active_level == nullptr)
            return;

        m_levels.erase(std::find(m_levels.begin(), m_levels.end(), m_current_active_level));

        std::string current_level_url = m_current_active_level->getLevelResUrl();
        m_current_active_level->clear();
        delete m_current_active_level;

        loadLevel(current_level_url);
    }
    void WorldManager::saveCurrentLevel()
    {
        if (m_current_active_level == nullptr)
            return;

        m_current_active_level->save();
    }



    void WorldManager::tick(float delta_time)
    {
        processPendingLoadWorld();

        for (Level* level : m_levels)
        {
            level->tickAll(delta_time);
        }
    }

    void WorldManager::processPendingLoadWorld()
    {
        if (m_pending_load_world_path.empty())
            return;

        std::string pending_load_world_path = m_pending_load_world_path.generic_string();

        WorldRes world_res;
        // load asset from json file, and anti-serialize to world_res(class data)
        AssetManager::getInstance().loadAsset(m_pending_load_world_path, world_res);
        m_pending_load_world_path.clear();
        if (world_res.m_name == m_current_world_name)
            return;

        clear();

        loadWorld(world_res);
    }
    void WorldManager::loadWorld(const WorldRes& pending_load_world)
    {
        m_current_world_name = pending_load_world.m_name;

        // typedef std::unordered_set<size_t> TypeIDSet;
        for (const std::string& level_url : pending_load_world.m_levels)
        {
            loadLevel(level_url);
        }
    }
    void WorldManager::loadLevel(const std::string& level_url)
    {
        Level* level = new Level;
        level->load(level_url);
        m_levels.push_back(level);

        // now only one level, so activate it
        if (m_current_active_level == nullptr)
        {
            m_current_active_level = level;
        }
    }


} // namespace VE
