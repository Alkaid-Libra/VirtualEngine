#pragma once

#include <string>
#include <unordered_map>

namespace VE
{
    class GObject;
    class ObjectInstanceRes;

    class Level
    {
    protected:
        std::string m_level_res_url;

        std::unordered_map<size_t, GObject*> m_gobjects;
        size_t m_next_gobject_id{0};

    public:

        void clear();

        void load(const std::string& level_res_url);
        void save();

        void tickAll(float delta_time);


        const std::string& getLevelResUrl() const { return m_level_res_url; }

        const size_t createObject(const ObjectInstanceRes& object_instance_res);

        GObject* getGObjectByID(size_t go_id) const;
    };
} // namespace VE
