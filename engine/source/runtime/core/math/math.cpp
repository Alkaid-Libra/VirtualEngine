#include "runtime/core/math/math.h"

namespace VE
{
    float Math::degreesToRadians(float degrees) { return degrees * Math_fDeg2Rad; }
    float Math::radiansToDegrees(float radians) { return radians * Math_fRad2Deg; }

    float Math::radiansToAngleUnits(float radians)
    {
        // if (k_AngleUnit == AngleUnit::AU_DEGREE)
        //     return radians * Math_fRad2Deg;

        return radians;
    }
} // namespace VE
