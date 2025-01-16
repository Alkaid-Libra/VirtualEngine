#include "runtime/function/physics/physics_actor.h"

#include "runtime/function/framework/component/transform/transform_component.h"

#include "runtime/core/base/macro.h"

// 
#include <iostream>

namespace VE
{
    PhysicsActor::PhysicsActor(GObject* gobject, const Transform& global_transform) :
        m_parent_object{gobject}, m_global_transform{global_transform}
    {
        m_friction = 0.8f;
    }

    void PhysicsActor::createShapes(const std::vector<RigidBodyShape>& shape_defs, const Transform& global_transform)
    {
        m_rigidbody_shapes = shape_defs;
        for (RigidBodyShape& shape : m_rigidbody_shapes)
        {
            const std::string shape_type = shape.m_geometry.getTypeName();
            if (shape_type == "Box")
            {
                Box* box_shape_geom = new Box;

                (*box_shape_geom) = *static_cast<Box*>(shape.m_geometry.operator->());

                shape.m_geometry.getPtrReference() = box_shape_geom;
                shape.m_type = RigidBodyShapeType::box;
            }
            else
            {
                LOG_ERROR("Unsupported shape type: " + shape_type);
                continue;
            }
        }
    }

    void PhysicsActor::setInverseMass(float inverse_mass) { m_inverse_mass = inverse_mass; }

    void PhysicsActor::setGlobalTransform(const Transform& global_transform)
    {
        for (RigidBodyShape& shape : m_rigidbody_shapes)
        {
            if (shape.m_type == RigidBodyShapeType::box)
            {
                Matrix4x4 global_transform_matrix = global_transform.getMatrix() * shape.m_local_transform.getMatrix();
                global_transform_matrix.decomposition(shape.m_global_transform.m_position,
                                                      shape.m_global_transform.m_scale,
                                                      shape.m_global_transform.m_rotation);

                Box* box = static_cast<Box*>(shape.m_geometry);
                AxisAlignedBox bounding;
                for (unsigned char i = 0; i < 2; ++i)
                {
                    for (unsigned char j = 0; j < 2; ++j)
                    {
                        for (unsigned char k = 0; k < 2; ++k)
                        {
                            Vector3 point = box->m_half_extents;
                            if (i == 0)
                                point.x = -point.x;
                            if (j == 0)
                                point.y = -point.y;
                            if (k == 0)
                                point.z = -point.z;

                            point = shape.m_global_transform.getMatrix() * point;

                            bounding.merge(point);
                        }
                    }
                }

                shape.m_bounding_box.update(bounding.getCenter(), bounding.getHalfExtent());
            }
        }
    }
} // namespace VE
