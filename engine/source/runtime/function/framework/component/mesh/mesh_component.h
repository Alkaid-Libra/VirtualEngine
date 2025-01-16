#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/resource/res_type/components/mesh.h"

#include "runtime/function/scene/scene_object.h"

#include <vector>

namespace VE
{
    REFLECTION_TYPE(MeshComponent)
    CLASS(MeshComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(MeshComponent)

    private:
        META(Enable) std::vector<GameObjectComponentDesc> m_raw_meshes;
    
    public:
        MeshComponent() {};
        MeshComponent(const MeshComponentRes& mesh_ast, GObject* );

        void tick(float delta_time) override;
        
    };
} // namespace VE
