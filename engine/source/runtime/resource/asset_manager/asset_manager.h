#pragma

#include "runtime/core/base/public_singleton.h"
#include "runtime/core/meta/serializer/serializer.h"

#include <fstream>
#include <sstream>
#include <filesystem>

#include "_generated/serializer/all_serializer.h"

namespace VE
{
    class AssetManager : public PublicSingleton<AssetManager>
    {
    public:
        void initialize();
        void clear() {}

        // load asset from json file, and anti-serialize to class data
        template<typename AssetType>
        void loadAsset(const std::filesystem::path& asset_path, AssetType& out_asset) const
        {
            // read json file to string
            std::ifstream asset_json_file(asset_path);
            std::stringstream buffer;
            buffer << asset_json_file.rdbuf();
            std::string asset_json_text(buffer.str());

            // parse to json object and read to runtime result object
            std::string error;
            auto&& asset_json = Json::parse(asset_json_text, error);
            assert(error.empty());

            Serializer::read(asset_json, out_asset);
        }

        std::filesystem::path getFullPath(const std::string& relative_path) const;

        typedef std::function<Reflection::ReflectionPtr<Component>(std::string, GObject*)> ComponentLoaderFunc;
        ComponentLoaderFunc getComponentLoader(std::string component_type_name)
        {
            return m_loader_map[component_type_name];
        }

        void registerComponentType(std::string component_type_name, ComponentLoaderFunc func)
        {
            m_loader_map[component_type_name] = func;
        }

#define REGISTER_COMPONENT(COMPONENT_TYPE, COMPONENT_RES_TYPE, TICK_IN_EDITOR_MODE) \
    registerComponentType(#COMPONENT_TYPE, [this](std::string component_res_file, GObject* parent_object) { \
        COMPONENT_RES_TYPE component_res; \
        loadAsset(getFullPath(component_res_file), component_res); \
        auto component = VIRTUAL_REFLECTION_NEW(COMPONENT_TYPE, component_res, parent_object); \
        component->m_tick_in_editor_mode = TICK_IN_EDITOR_MODE; \
        return component; \
    });



    private:
        std::unordered_map<std::string, ComponentLoaderFunc> m_loader_map;

    };
} // namespace VE
