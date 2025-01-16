#include "runtime/function/framework/object/object.h"

#include "runtime/function/framework/component/transform/transform_component.h"

#include "runtime/resource/asset_manager/asset_manager.h"

// #include "_generated/serializer/all_serializer.h"

namespace VE
{
    

    bool GObject::load(const ObjectInstanceRes& object_instance_res)
    {
        setName(object_instance_res.m_name);

        // load transform component
        const Transform& transform = object_instance_res.m_transform;
        auto transform_component_ptr = VIRTUAL_REFLECTION_NEW(TransformComponent, transform, this);
        // TransformComponent cast to Component, into vector<ReflectionPtr<Component>> Object::m_components
        m_components.push_back(transform_component_ptr);
        m_component_type_names.push_back("TransformComponent");

        // load object instance components
        TypeNameSet instance_component_type_set;
        // -load instance_components in level_res, none in level.json:wall
        if (loadComponents(object_instance_res.m_instance_components, instance_component_type_set) == false)
            return false;

        AssetManager& asset_manager = AssetManager::getInstance();
        // load object definition components - level.json:wall:definition for example
        m_definition_url = object_instance_res.m_definition;
        std::string definition_path = asset_manager.getFullPath(m_definition_url).generic_string();
        
        ObjectDefinitionRes definition_res;
        // -read wall.object.json as definition_res for example
        AssetManager::getInstance().loadAsset(definition_path, definition_res);

        // -load wall.object.json:components and read wall.mesh.component.json, and then load wall.mesh.component.json for example
        if (loadComponents(definition_res.m_components, instance_component_type_set) == false)
            return false;

        return true;
    }

    void GObject::save(ObjectInstanceRes &out_object_instance_res)
    {
        out_object_instance_res.m_name = m_name;
        out_object_instance_res.m_definition = m_definition_url;

        const TransformComponent* transform_component = tryGetComponentConst(TransformComponent);
        out_object_instance_res.m_transform = transform_component->getTransformConst();
    }

    bool GObject::loadComponents(const std::vector<std::string> &components,
                                 TypeNameSet &out_instance_component_type_set)

    {
        AssetManager& asset_manager = AssetManager::getInstance();
        ComponentDefinitionRes definition_res;

        for (const std::string& definition_res_path : components)
        {
            // read wall.mesh.component.json to definition_res for example
            asset_manager.loadAsset(asset_manager.getFullPath(definition_res_path), definition_res);
            // load the content of definition_res
            if (loadComponentDefinition(definition_res, false, out_instance_component_type_set) == false)
                return false;
        }

        return true;
    }

    bool GObject::loadComponentDefinition(const ComponentDefinitionRes& component_definition_res,
                                          const bool is_instance_component,
                                          TypeNameSet& out_instance_component_type_set)
    {
        AssetManager& asset_manager = AssetManager::getInstance();
        ComponentDefinitionRes component_definition;

        if (is_instance_component || out_instance_component_type_set.count(component_definition_res.m_type_name) == 0)
        {
            // register func
            auto&& component_loader = asset_manager.getComponentLoader(component_definition_res.m_type_name);
            // read wall.mesh.component.json:component(wall.mesh.json)
            // return MeshComponent instance RefletionPtr
            auto&& component = component_loader(component_definition_res.m_component, this);
        
            // * component_loader:
            // [this](std::string component_res_file, GObject* parent_object) 
            // { 
            //     MeshComponentRes component_res; 
            //     loadAsset(getFullPath(component_res_file), component_res); 
            //     auto component = Reflection::ReflectionPtr(
            //         "MeshComponent", new MeshComponent(component_res, parent_object));; 
            //     component->m_tick_in_editor_mode = true; 
            //     return component; 
            // * }

            if (component)
            {
                m_components.push_back(component);
                m_component_type_names.push_back(component_definition_res.m_type_name);
                component->setParentObject(this);
                out_instance_component_type_set.insert(component_definition_res.m_type_name);
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    void GObject::tick(float delta_time)
    {
        for (auto& component : m_components)
        {
            component->tick(delta_time);
        }
    }

} // namespace VE
