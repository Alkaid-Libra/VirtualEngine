#pragma once

#include "runtime/core/math/transform.h"

#include "runtime/resource/res_type/components/rigid_body.h"

namespace VE
{
    class GObject;

    class PhysicsActor
    {
    public:
        PhysicsActor(GObject* gobject, const Transform& global_transform);

        void createShapes(const std::vector<RigidBodyShape>& shape_defs, const Transform& global_transform);

        void setInverseMass(float inverse_mass);

        void setActorType(int type) { m_actor_type = type; }

        void setGlobalTransform(const Transform& global_transform);

    protected:
        GObject* m_parent_object{nullptr};
        Transform m_global_transform;

        float m_friction{0.f};
        float m_inverse_mass{0.f};

        int m_actor_type{0};

        std::vector<RigidBodyShape> m_rigidbody_shapes;
    };
} // namespace VE
