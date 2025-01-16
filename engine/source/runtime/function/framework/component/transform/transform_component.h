#pragma once

#include "runtime/function/framework/object/object.h"
#include "runtime/function/framework/component/component.h"

namespace VE
{
    REFLECTION_TYPE(TransformComponent)
    CLASS(TransformComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(TransformComponent);

    protected:
        Transform m_transform;
        Transform m_transform_buffer[2];

        size_t m_current_index{0};

    public:
        TransformComponent() {}
        TransformComponent(const Transform& transform, GObject* parent_gobject);

        Matrix4x4 getMatrix() const { return m_transform_buffer[m_current_index].getMatrix(); }

        const Transform& getTransformConst() const { return m_transform_buffer[m_current_index]; }
    };
} // namespace VE
