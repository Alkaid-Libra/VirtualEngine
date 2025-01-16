#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace VE
{
    class GObject;

    // Component
    REFLECTION_TYPE(Component)
    CLASS(Component, WhiteListFields)
    {
        REFLECTION_BODY(Component);

    protected:
        GObject* m_parent_object;
        

    public:
        Component() {}
        Component(GObject* object) : m_parent_object{object} {}
        // virtual ~Component() {}


        void setParentObject(GObject* object) { m_parent_object = object; }

        bool m_tick_in_editor_mode{false};

        virtual void tick(float delta_time) {};
    };
} // namespace VE
