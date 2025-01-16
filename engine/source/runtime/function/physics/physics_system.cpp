#include "runtime/function/physics/physics_system.h"

namespace VE
{
    PhysicsActor* PhysicsSystem::createPhysicsActor(GObject* gobject,
                                                    const Transform& global_transform,
                                                    const RigidBodyActorRes& rigid_body_actor_res)
    {
        PhysicsActor* actor = new PhysicsActor(gobject, global_transform);

        actor->createShapes(rigid_body_actor_res.m_shapes, global_transform);
        actor->setInverseMass(rigid_body_actor_res.m_inverse_mass);
        actor->setActorType(rigid_body_actor_res.m_actor_type);

        Transform actor_transform(global_transform.m_position, global_transform.m_rotation, Vector3::UNIT_SCALE);
    
        actor->setGlobalTransform(actor_transform);
    
        m_physics_actors.push_back(actor);

        return actor;
    }
} // namespace VE
