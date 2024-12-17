#pragma once

#include "runtime/core/math/math.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace VE
{
    class Matrix3x3;
    class Vector3;

    REFLECTION_TYPE(Quaternion)
    CLASS(Quaternion, Fields)
    {
        REFLECTION_BODY(Quaternion);

    public:
        float w{1.f}, x{0.f}, y{0.f}, z{0.f};

    public:
        Quaternion() = default;
        Quaternion(float w_, float x_, float y_, float z_) : w{w_}, x{x_}, y{y_}, z{z_} {}

        // Construct a quaternion from a rotation matrix
        explicit Quaternion(const Matrix3x3& rot) { this->fromRotationMatrix(rot); }
        // Construct a quaternion from an angle/axis
        Quaternion(const Radian& angle, const Vector3& axis) { this->fromAngleAxis(angle, axis); }
        // Construct a quaternion from 3 orthonormal local axes
        // the axes are assumed to be orthonormal
        Quaternion(const Vector3& xaxis, const Vector3& yaxis, const Vector3& zaxis)
        {
            this->fromAxes(xaxis, yaxis, zaxis);
        }

        // Pointer accessor for direct copying
        float* ptr() { return &w; }
        const float* ptr() const { return &w; }

        void fromRotationMatrix(const Matrix3x3& rotation);
        void toRotationMatrix(Matrix3x3& rotation) const;

        void fromAngleAxis(const Radian& angle, const Vector3& axis);
        void toAngleAxis(Radian& angle, Vector3& axis) const;
        
        void fromAxes(const Vector3& x_axis, const Vector3& y_axis, const Vector3& z_axis);
        void toAxes(Vector3& x_axis, Vector3& y_axis, Vector3& z_axis) const;

        void fromDirection(const Vector3& direction, const Vector3& up_direction);

        static Quaternion getQuaternionFromAngleAxis(const Radian& angle, const Vector3& axis);
        static Quaternion getQuaternionFromDirection(const Vector3& direction, const Vector3& up_direction);

        // also called the local XYZ-axis
        // same as doing xAxis = Vector3::UNIT_X * this
        Vector3 xAxis() const;
        Vector3 yAxis() const;
        Vector3 zAxis() const;

        Quaternion operator+(const Quaternion& rhs) const
        {
            return Quaternion(w + rhs.w, x + rhs.x, y + rhs.y, z + rhs.z);
        }
        Quaternion operator-(const Quaternion& rhs) const
        {
            return Quaternion(w - rhs.w, x - rhs.x, y - rhs.y, z - rhs.z);
        }

        Quaternion mul(const Quaternion& rhs) const { return (*this) * rhs; }
        Quaternion operator*(const Quaternion& rhs) const;

        Quaternion operator*(float scalar) const { return Quaternion(w * scalar, x * scalar, y * scalar, z * scalar); }
        Quaternion operator/(float scalar) const
        {
            assert(scalar != 0.0f);
            return Quaternion(w / scalar, x / scalar, y / scalar, z / scalar);
        }

        // rotation of a vector by a quaternion
        // Quaternion * vector
        Vector3 operator*(const Vector3& rhs) const;
        // scalar * Quaternion
        friend Quaternion operator*(float scalar, const Quaternion& rhs)
        {
            return Quaternion(scalar * rhs.w, scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
        }

        Quaternion operator-() const { return Quaternion(-w, -x, -y, -z); }

        bool operator==(const Quaternion& rhs) const
        {
            return (rhs.x == x) && (rhs.y == y) && (rhs.z == z) && (rhs.w == w);
        }
        bool operator!=(const Quaternion& rhs) const
        {
            return (rhs.x != x) || (rhs.y != y) || (rhs.z != z) || (rhs.w != w);
        }

        bool isNaN() const { return Math::isNan(x) || Math::isNan(y) || Math::isNan(z) || Math:: isNan(w); }

        float getX() const { return x; }
        float getY() const { return y; }
        float getZ() const { return z; }
        float getW() const { return w; }

        float dot(const Quaternion& rkQ) const { return w * rkQ.w + x * rkQ.x + y * rkQ.y + z * rkQ.z; }

        float length() const { return std::sqrt(w * w + x * x + y * y + z * z); }

        void normalise()
        {
            float factor = 1.0f / this->length();
            *this = *this * factor;
        }

        Quaternion inverse() const // apply to non-zero quaternion
        {
            float norm = w * w + x * x + y * y + z * z;
            if (norm > 0.0)
            {
                float inv_norm = 1.0f / norm;
                return Quaternion(w * inv_norm, -x * inv_norm, -y * inv_norm, -z * inv_norm);
            }
            else
            {
                // return an invalid result to flag the error
                return ZERO;
            }
        }

        /** Calculate the local roll element of this quaternion
            By default the method returns the 'intutive result
            that is, if you based X rotate, projected the local Z of the quaternion onto the Y-Z axes, the angle between them is returned. 
            If set tok_false though, the result is the actual yaw that will be used to implement the quaternion, which is 
            the shortest possible path to get to the same orientation and may involve less axial rotation. 
        */
        Radian getRoll(bool reproject_axis = true) const;
        // Calculate the local pitch element of this quaternion, based Y rotate
        Radian getPitch(bool reproject_axis = true) const;
        // Calculate the local yaw element of this quaternion, , based Z rotate
        Radian getYaw(bool reproject_axis = true) const;

        /** Performs Spherical linear interpolation between two quaternions, and returns the result.
            sLerp ( 0.0f, A, B ) = A
            sLerp ( 1.0f, A, B ) = B
            *
            sLerp has the proprieties of performing the interpolation at constant
            velocity, and being torque-minimal (unless shortestPath=false).
            However, it's NOT commutative, which means
            Slerp ( 0.75f, A, B ) != Slerp ( 0.25f, B, A );
            therefore be careful if your code relies in the order of the operands.
            This is specially important in IK animation.
        */
        static Quaternion sLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);
        /** Performs Normalised linear interpolation between two quaternions, and returns the result.
            nLerp ( 0.0f, A, B ) = A
            nLerp ( 1.0f, A, B ) = B
            *
            nLerp is faster than sLerp.
            Nlerp has the proprieties of being commutative (@See Slerp;
            commutativity is desired in certain places, like IK animation), and
            being torque-minimal (unless shortestPath=false). However, it's performing
            the interpolation at non-constant velocity; sometimes this is desired,
            sometimes it is not. Having a non-constant velocity can produce a more
            natural rotation feeling without the need of tweaking the weights; however
            if your scene relies on the timing of the rotation or assumes it will point
            at a specific angle at a specific weight value, sLerp is a better choice.
        */
        static Quaternion nLerp(float t, const Quaternion& kp, const Quaternion& kq, bool shortest_path = false);

        Quaternion conjugate() const { return Quaternion(w, -x, -y, -z); }

        // special values
        static const Quaternion ZERO;
        static const Quaternion IDENTITY;

        static const float k_epsilon;
    };
} // namespace VE
