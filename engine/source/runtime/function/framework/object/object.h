#pragma once

#include "runtime/resource/res_type/common/object.h"

#include <string>
#include <unordered_set>

namespace VE
{
    class Component;

    // GObject : Game Object base class
    class GObject
    {
        typedef std::unordered_set<std::string> TypeNameSet;

    protected:
        size_t m_id; //compatible with ecs - Entity-Component-System(实体 组件 系统)
        std::string m_name;
        std::string m_definition_url;

        std::vector<Reflection::ReflectionPtr<Component>> m_components;
        std::vector<std::string> m_component_type_names;

    public:
        GObject(size_t id) : m_id{id} {}

        bool load(const ObjectInstanceRes& object_instance_res);
        void save(ObjectInstanceRes& out_object_instance_res);

        void setName(std::string name) { m_name = name; }
        const std::string& getName() const { return m_name; }

        bool loadComponents(const std::vector<std::string>& components, TypeNameSet& out_instance_component_type_set);
    
        bool loadComponentDefinition(const ComponentDefinitionRes& component_definition_res,
                                     const bool is_instance_component,
                                     TypeNameSet& out_instance_component_type_set);

        virtual void tick(float delta_time);

        const size_t getID() const { return m_id; }

        template<typename TComponent>
        TComponent* tryGetComponent(const std::string& component_type_name)
        {
            for (int i = 0; i < m_components.size(); ++i)
            {
                if (component_type_name == m_component_type_names[i])
                {
                    return static_cast<TComponent*>(m_components[i].operator->());
                }
            }

            return nullptr;
        }
        template<typename TComponent>
        const TComponent* tryGetComponentConst(const std::string& component_type_name) const
        {
            for (int i = 0; i < m_components.size(); ++i)
            {
                if (component_type_name == m_component_type_names[i])
                {
                    return static_cast<TComponent*>(m_components[i].operator->());
                }
            }

            return nullptr;
        }

#define tryGetComponent(COMPONENT_TYPE) tryGetComponent<COMPONENT_TYPE>(#COMPONENT_TYPE)
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<const COMPONENT_TYPE>(#COMPONENT_TYPE)
    };
} // namespace VE
