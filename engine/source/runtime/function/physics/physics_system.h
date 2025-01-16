#pragma once

#include "runtime/core/base/public_singleton.h"

#include "runtime/function/physics/physics_actor.h"

#include "runtime/resource/res_type/components/rigid_body.h"

namespace VE
{
    class PhysicsSystem : public PublicSingleton<PhysicsSystem>
    {
    public:
        

        PhysicsActor* createPhysicsActor(GObject* gobject,
                                         const Transform& global_transform,
                                         const RigidBodyActorRes& rigid_body_actor_res);

    private:
        std::vector<PhysicsActor*> m_physics_actors;
    };
} // namespace VE
