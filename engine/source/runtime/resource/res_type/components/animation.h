#pragma once

#include "runtime/core/meta/reflection/reflection.h"


// 
#include "runtime/core/math/transform.h"

namespace VE
{
    REFLECTION_TYPE(AnimationResultElement)
    CLASS(AnimationResultElement, WhiteListFields)
    {
        REFLECTION_BODY(AnimationResultElement);

    public:
        int index;
        Matrix4x4_ tranform;
    };
    REFLECTION_TYPE(AnimationResult)
    CLASS(AnimationResult, Fields)
    {
        REFLECTION_BODY(AnimationResult);

    public:
        std::vector<AnimationResultElement> node;
    };


    REFLECTION_TYPE(AnimationComponentRes)
    CLASS(AnimationComponentRes, Fields)
    {
        REFLECTION_BODY(AnimationComponentRes);

    public:
        AnimationResult animation_result;
    };
} // namespace VE
