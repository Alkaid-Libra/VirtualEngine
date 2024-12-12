#include "runtime/core/math/math.h"

namespace VE
{
    Math::AngleUnit Math::k_AngleUnit; // !!must definition out of class and explicit initialize
    Math::Math() { k_AngleUnit = AngleUnit::AU_DEGREE; }

    bool Math::realEqual(float a, float b, float tolerance /* = std::numeric_limits<float>::epsilon() */)
    {
        return std::fabs(b - a) <= tolerance;
    }
    
    float Math::degreesToRadians(float degrees) { return degrees * Math_fDeg2Rad; }
    
    float Math::radiansToDegrees(float radians) { return radians * Math_fRad2Deg; }

    float Math::angleUnitsToRadians(float angleunits)
    {
        if (k_AngleUnit == AngleUnit::AU_DEGREE)
            return angleunits * Math_fDeg2Rad;
        
        return angleunits;
    }

    float Math::radiansToAngleUnits(float radians)
    {
        if (k_AngleUnit == AngleUnit::AU_DEGREE)
            return radians * Math_fRad2Deg;

        return radians;
    }

    float Math::angleUnitsToDegrees(float angleunits)
    {
        if (k_AngleUnit == AngleUnit::AU_RADIAN)
            return angleunits * Math_fRad2Deg;
        
        return angleunits;
    }

    float Math::degreesToAngleUnits(float degrees)
    {
        if (k_AngleUnit == AngleUnit::AU_RADIAN)
            return degrees * Math_fDeg2Rad;

        return degrees;
    }

    Radian Math::acos(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(acos(value));

            return Radian(0.0);
        }

        return Radian(Math_PI);
    }

    Radian Math::asin(float value)
    {
        if (-1.0 < value)
        {
            if (value < 1.0)
                return Radian(asin(value));

            return Radian(Math_HALF_PI);
        }

        return Radian(-Math_HALF_PI);
    }

    // Matrix4x4
    // Math::makeViewMatrix(const Vector3& position, const Quaternion& orientation, const Matrix4x4* reflect_matrix)
    // {
    //     Matrix4x4 viewMatrix;

    //     return viewMatrix;
    // }

    static Matrix4x4

    makeLookMatrix(const Vector3& eye_position, const Vector3& target_position, const Vector3& up_dir);

    static Matrix4x4 makePerspectiveMatrix(Radian fovy, float aspect, float znear, float zfar);

    static Matrix4x4

    makeOrthographicProjectionMatrix(float left, float right, float bottom, float top, float znear, float zfar);

} // namespace VE
