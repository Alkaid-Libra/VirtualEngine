#pragma once

#include <unordered_map>

namespace VE
{
    static const size_t s_invalid_guid = 0;

    template<typename T>
    class SceneAllocator
    {
    public:

        size_t allocGuid(const T& t)
        {
            auto find_it = m_elements_guid_map.find(t);
            if (find_it != m_elements_guid_map.end())
                return find_it->second;

            for (size_t i = 0; i < m_guid_elements_map.size() + 1; i++)
            {
                // guid from 1 not 0 ?
                size_t guid = i + 1;
                if (m_guid_elements_map.find(guid) == m_guid_elements_map.end())
                {
                    m_guid_elements_map.insert(std::make_pair(guid, t));
                    m_elements_guid_map.insert(std::make_pair(t, guid));
                    return guid;
                }
            }

            return s_invalid_guid;
        }

        void freeElement(const T& t)
        {
            auto find_it = m_elements_guid_map.find(t);
            if (find_it != m_elements_guid_map.end())
            {
                const auto& guid = find_it->second;
                m_elements_guid_map.erase(t);
                m_guid_elements_map.erase(guid);
            }
        }

        void clear()
        {
            m_elements_guid_map.clear();
            m_guid_elements_map.clear();
        }

    private:
        std::unordered_map<T, size_t> m_elements_guid_map;
        std::unordered_map<size_t, T> m_guid_elements_map;
    };
} // namespace VE
