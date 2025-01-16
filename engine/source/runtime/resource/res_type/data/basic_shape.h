#pragma once

#include "runtime/core/meta/reflection/reflection.h"

namespace VE
{
    REFLECTION_TYPE(Geometry)
    CLASS(Geometry, Fields)
    {
        REFLECTION_BODY(Geometry);

    public:

    };

    REFLECTION_TYPE(Box)
    CLASS(Box : public Geometry, Fields)
    {
        REFLECTION_BODY(Box);
    
    public:


        Vector3 m_half_extents{0.5f, 0.5f, 0.5f};
    };

} // namespace VE
