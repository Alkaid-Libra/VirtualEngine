#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/math/quaternion.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace VE
{
    REFLECTION_TYPE(Vector3)
    CLASS(Vector3, Fields)
    {
        REFLECTION_BODY(Vector3);

    public:
        float x{0.f}, y{0.f}, z{0.f};

    public:
        Vector3() = default;
        Vector3(float x_, float y_, float z_) : x{x_}, y{y_}, z{z_} {}

        explicit Vector3(const float coords[3]) : x{coords[0]}, y{coords[1]}, z{coords[2]} {}

        float* ptr() { return &x; }
        const float* ptr() const { return &x; }

        float operator[](size_t i) const // can be used by const Vector3, and don't modify value
        {
            assert(i < 3);
            return *(&x + i);
        }
        float& operator[](size_t i) // allowed to modify value
        {
            assert(i < 3);
            return *(&x + i);
        }

        bool operator==(const Vector3& rhs) const { return (x == rhs.x && y == rhs.y && z == rhs.z); }
        bool operator!=(const Vector3& rhs) const { return (x != rhs.x || y != rhs.y || z != rhs.z); }

        // arithmetic operations
        Vector3 operator+(const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
        Vector3 operator-(const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
        
        Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
        Vector3 operator*(const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }

        Vector3 operator/(float scalar) const
        {
            assert(scalar != 0.0);
            float inv = 1.0f / scalar;
            return Vector3(x * inv, y * inv, z * inv);
        }
        Vector3 operator/(const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }

        const Vector3& operator+() const { return *this; }

        Vector3 operator-() const { return Vector3(-x, -y, -z); }
        
        // overloaded operators to help Vector3
        friend Vector3 operator*(float scalar, const Vector3& rhs) { return Vector3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z); } // complete float * v, not v * float
        friend Vector3 operator/(float fScalar, const Vector3& rhs) { return Vector3(fScalar / rhs.x, fScalar / rhs.y, fScalar / rhs.z); }
        
        friend Vector3 operator+(const Vector3& lhs, float rhs) { return Vector3(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
        friend Vector3 operator+(float lhs, const Vector3& rhs) { return Vector3(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z); }

        friend Vector3 operator-(const Vector3& lhs, float rhs) { return Vector3(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
        friend Vector3 operator-(float lhs, const Vector3& rhs) { return Vector3(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z); }

        // arithmetic updates
        Vector3& operator+=(const Vector3& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }
        Vector3& operator+=(float scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;

            return *this;
        }

        Vector3& operator-=(const Vector3& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }
        Vector3& operator-=(float scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;

            return *this;
        }

        Vector3& operator*=(const Vector3& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;

            return *this;
        }
        Vector3& operator*=(float scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;

            return *this;
        }

        Vector3& operator/=(const Vector3& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;

            return *this;
        }
        Vector3& operator/=(float scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;

            return *this;
        }

        float length() const { return std::hypot(x, y, z); }
        float squaredLength() const { return x * x + y * y + z * z; }

        float distance(const Vector3& rhs) const { return (*this - rhs).length(); }
        float squaredDistance(const Vector3& rhs) const { return (*this - rhs).squaredLength(); }

        float dotProduct(const Vector3& vec) const { return x * vec.x + y * vec.y + z * vec.z; }

        void normalise()
        {
            float length = std::hypot(x, y, z);
            if (length == 0.f)
                return;

            float inv_length = 1.0f / length;
            x *= inv_length;
            y *= inv_length;
            z *= inv_length;            
        }
        
        void makeFloor(const Vector3& cmp)
        {
            if (cmp.x < x)
                x = cmp.x;
            if (cmp.y < y)
                y = cmp.y;
            if (cmp.z < z)
                z = cmp.z;
        }

        void makeCeil(const Vector3& cmp)
        {
            if (cmp.x > x)
                x = cmp.x;
            if (cmp.y > y)
                y = cmp.y;
            if (cmp.z > z)
                z = cmp.z;
        }

        Radian angleBetween(const Vector3& dest) const
        {
            float len_product = length() * dest.length();

            // divide by zero check
            if (len_product < 1e-6f)
                len_product = 1e-6f;

            float f = dotProduct(dest) / len_product;

            f = Math::clamp(f, (float)-1.0, (float)1.0);
            return Math::acos(f);
        }

        /** Gets the shortest arc quaternion to rotate this vector to the destination
        vector.
        @
        If you call this with a dest vector that is close to the inverse
        of this vector, we will rotate 180 degrees around the 'fallbackAxis'
        (if specified, or a generated axis if not) since in this case
        ANY axis of rotation is valid.
        */
        Quaternion getRotationTo(const Vector3& dest, const Vector3& fallback_axis = Vector3::ZERO) const
        {
            // Based on Stan Melax's article in Game Programming Gems
            Quaternion q;
            // Copy, since cannot modify local
            Vector3 v0 = *this;
            Vector3 v1 = dest;
            v0.normalise();
            v1.normalise();

            float d = v0.dotProduct(v1);
            // If dot == 1, vectors are the same
            if (d >= 1.0f)
            {
                return Quaternion::IDENTITY;
            }
            if ( d < (1e-6f - 1.0f))
            {
                if (fallback_axis != Vector3::ZERO)
                {
                    // rotate 180 degrees about the fall back axis
                    q.fromAngleAxis(Radian(Math_PI), fallback_axis);
                }
                else
                {
                    // generate an axis
                    Vector3 axis = Vector3::UNIT_X.crossProduct(*this);
                    if (axis.isZeroLength()) // pick another if collinear
                        axis = Vector3::UNIT_Y.crossProduct(*this);
                    axis.normalise();
                    q.fromAngleAxis(Radian(Math_PI), axis);
                }
            }
            else
            {
                float s = Math::sqrt((1 + d) * 2);
                float invs = 1 / s;

                Vector3 c = v0.crossProduct(v1);

                q.x = c.x * invs;
                q.y = c.y * invs;
                q.z = c.z * invs;
                q.w = s * 0.5f;
                q.normalise();
            }
            return q;
        }

        Vector3 crossProduct(const Vector3& rhs) const 
        {
            return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
        }

        bool isZeroLength() const
        {
            float sqlen = x * x + y * y + z * z;
            return (sqlen < (1e-06 * 1e-06));
        }

        bool isZero() const { return x == 0.f && y == 0.f && z == 0.f; }

        Vector3 normalisedCopy() const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

        // assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        // @param normal unit normal
        Vector3 reflect(const Vector3& normal) const
        {
            return Vector3(*this - (2 * this->dotProduct(normal) * normal));
        }
        // assumes 'this' is pointing AWAY FROM the plane, invert if it is not.
        // @param normal unit normal
        Vector3 project(const Vector3& normal) const { return Vector3(*this - (this->dotProduct(normal) * normal)); }

        Vector3 absoluteCopy() const { return Vector3(fabsf(x), fabsf(y), fabsf(z)); }
        
        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z); }

        static Vector3 lerp(const Vector3& lhs, const Vector3& rhs, float alpha) { return lhs + alpha * (rhs - lhs); }

        static Vector3 clamp(const Vector3& v, const Vector3& min, const Vector3& max)
        {
            return Vector3(Math::clamp(v.x, min.x, max.x), Math::clamp(v.y, min.y, max.y), Math::clamp(v.z, min.z, max.z));
        }

        static float getMaxElement(const Vector3& v) { return Math::getMaxElement(v.x, v.y, v.z); }

        // special points
        static const Vector3 ZERO;
        static const Vector3 UNIT_X;
        static const Vector3 UNIT_Y;
        static const Vector3 UNIT_Z;
        static const Vector3 NEGATIVE_UNIT_X;
        static const Vector3 NEGATIVE_UNIT_Y;
        static const Vector3 NEGATIVE_UNIT_Z;
        static const Vector3 UNIT_SCALE;
    };
} // namespace VE
