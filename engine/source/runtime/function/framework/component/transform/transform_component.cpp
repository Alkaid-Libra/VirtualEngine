#include "runtime/function/framework/component/transform/transform_component.h"


namespace VE
{
    TransformComponent::TransformComponent(const Transform& transform, GObject* parent_gobject) : 
        Component(parent_gobject)
    {
        m_transform_buffer[0] = transform;
        m_transform_buffer[1] = transform;

        m_transform = transform;
    }
} // namespace VE
