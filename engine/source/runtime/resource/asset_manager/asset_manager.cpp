#include "runtime/resource/asset_manager/asset_manager.h"

#include "runtime/resource/config_manager/config_manager.h"

#include "runtime/function/framework/component/mesh/mesh_component.h"
#include "runtime/function/framework/component/rigidbody/rigidbody_component.h"

#include "runtime/resource/res_type/components/mesh.h"
#include "runtime/resource/res_type/components/animation.h"

namespace VE
{
    void AssetManager::initialize()
    {
        REGISTER_COMPONENT(MeshComponent, MeshComponentRes, true);
        REGISTER_COMPONENT(RigidBodyComponent, RigidBodyActorRes, false);
    }

    std::filesystem::path AssetManager::getFullPath(const std::string& relative_path) const
    {
        return ConfigManager::getInstance().getRootFolder() / relative_path;
    }
} // namespace VE
