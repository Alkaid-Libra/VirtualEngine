#pragma once

#include "runtime/function/framework/component/component.h"

#include "runtime/resource/res_type/components/animation.h"

namespace VE
{
    REFLECTION_TYPE(AnimationComponent)
    CLASS(AnimationComponent : public Component, WhiteListFields)
    {
        REFLECTION_BODY(AnimationComponent);

    private:
        AnimationComponentRes animation_component;


    public:
        AnimationComponent() = default;

        const AnimationResult& getResult() const;
    };
} // namespace VE
