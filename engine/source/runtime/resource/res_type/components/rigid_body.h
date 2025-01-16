#pragma once

#include "runtime/core/meta/reflection/reflection.h"
#include "runtime/core/math/transform.h"
#include "runtime/core/math/axis_aligned.h"

#include "runtime/resource/res_type/data/basic_shape.h"

namespace VE
{
    enum class RigidBodyShapeType : unsigned char
    {
        box,
        sphere,
        capsule,
        invalid
    };

    REFLECTION_TYPE(RigidBodyShape)
    CLASS(RigidBodyShape, WhiteListFields)
    {
        REFLECTION_BODY(RigidBodyShape);

    public:
        META(Enable)
        Transform m_local_transform;
        META(Enable)
        Reflection::ReflectionPtr<Geometry> m_geometry;

        RigidBodyShapeType m_type{RigidBodyShapeType::invalid};
        Transform m_global_transform;
        AxisAlignedBox m_bounding_box;
    };

    REFLECTION_TYPE(RigidBodyActorRes)
    CLASS(RigidBodyActorRes, Fields)
    {
        REFLECTION_BODY(RigidBodyActorRes);

    public:
        std::vector<RigidBodyShape> m_shapes;
        float m_inverse_mass;
        int m_actor_type;
    };
} // namespace VE
