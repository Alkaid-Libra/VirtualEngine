#pragma once

#include "runtime/resource/res_type/components/rigid_body.h"

#include "runtime/function/framework/component/component.h"
#include "runtime/function/physics/physics_actor.h"

namespace VE
{
    REFLECTION_TYPE(RigidBodyComponent)
    CLASS(RigidBodyComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(RigidBodyComponent);

    public:
        RigidBodyComponent() {}
        RigidBodyComponent(const RigidBodyActorRes& rigidbody_ast, GObject* parent_object);

        PhysicsActor* m_physics_actor{nullptr};
    };
} // namespace VE
