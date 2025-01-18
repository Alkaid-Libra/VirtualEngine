#pragma once
#include "runtime/core/math/math_headers.h"

// 
#include <mutex>

namespace VE
{
    enum class VCurrentCameraType : int
    {
        Editor,
        Motor
    };

    class VCamera
    {
        float m_aspect;
        float m_fovy;
        float m_fovx{Degree(89.f).valueDegrees()};

    public:
        Vector3 m_position{0.0f, 0.0f, 0.0f};
        Quaternion m_rotation{Quaternion::IDENTITY};
        Quaternion m_invRotation{Quaternion::IDENTITY}; // conjugate
        float m_znear{1000.0f};
        float m_zfar{0.1f};

        static const Vector3 X, Y, Z;

        float m_exposure{1.0f};

        std::mutex m_view_matrix_mutex;
        static const int MAIN_VIEW_MATRIX_INDEX = 0;
        std::vector<Matrix4x4> m_view_matrices = {Matrix4x4::IDENTITY};

        VCurrentCameraType m_current_camera_type{VCurrentCameraType::Editor};

        void setMainViewMatrix(const Matrix4x4& view_matrix, VCurrentCameraType type = VCurrentCameraType::Editor)
        {
            std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
            m_current_camera_type = type;
            m_view_matrices[MAIN_VIEW_MATRIX_INDEX] = view_matrix;

            // convert location in camera to world coord -- world coordinate -> camera coordinate
            /**
             * Pb point in B coord -> Pa point in A coord
             * A = [i, j, k]
             * B = [iB, jB, kB]
             * iB = a11 * i + a12 * j + a13 * k = [a11 a12 a13] * [i j k]T = s * [i j k]T
             * jB = a21 * i + a22 * j + a23 * k = [a21 a22 a23] * [i j k]T = u * [i j k]T
             * kB = a31 * i + a32 * j + a33 * k = [a31 a32 a33] * [i j k]T = f * [i j k]T
             * -->
             * Pa(xA, yA, zA) = xB * iB + yB * jB + zB * kB (Pb = (xB, yB, zB))
             * -> [xA, yA, zA]T = [iB jB kB] * [xB yB zB]T
             *      [iB jB kB] 
             * R(from camera to world) = [a11 a21 a31  = [s[T] u[T] f[T]]
             *                            a12 a22 a32
             *                            a13 a23 a33]
             * 
             * Pcamera = T * R[-1] * Pworld -> R[-1] = R[T](from world to camera)
             * Pworld = R * T[-1] * Pcamera -> R * T[-1] = R * [-Tx -Ty -Tz]
             * 
             * view_matrix = [a11 a12 a13 a14  = [R[T] T
             *                a21 a22 a23 a24      0   1]
             *                a31 a32 a33 a34
             *                a41 a42 a43 a44]
            */
            Vector3 s = Vector3(view_matrix[0][0], view_matrix[0][1], view_matrix[0][2]);
            Vector3 u = Vector3(view_matrix[1][0], view_matrix[1][1], view_matrix[1][2]);
            Vector3 f = Vector3(-view_matrix[2][0], -view_matrix[2][1], -view_matrix[2][2]);
            m_position = s * (-view_matrix[0][3]) + u * (-view_matrix[1][3]) + f * view_matrix[2][3];
        }

        void lookAt(const Vector3& position, const Vector3& target, const Vector3& up)
        {
            m_position = position;

            // model rotation
            // maps vectors to camera space (x, y, z)
            Vector3 forward = (target - position).normalisedCopy();
            m_rotation = forward.getRotationTo(Y);

            // correct the up vector
            Vector3 right = forward.crossProduct(up.normalisedCopy()).normalisedCopy();
            Vector3 orthUp = right.crossProduct(forward);

            Quaternion upRotation = (m_rotation * orthUp).getRotationTo(Z);

            // inverse of the model rotation
            // maps camera space vectors to model vectors
            m_invRotation = m_rotation.conjugate();
        }

        Vector3 position() const { return m_position; }
        Quaternion rotation() const { return m_rotation; }

        // view matrix
        Matrix4x4 getViewMatrix()
        {
            std::lock_guard<std::mutex> lock_guard(m_view_matrix_mutex);
            auto view_matrix = Matrix4x4::IDENTITY;
            switch (m_current_camera_type)
            {
                case VE::VCurrentCameraType::Editor:
                    view_matrix = Math::makeLookAtMatrix(position(), position() + forward(), up());
                    break;
                case VE::VCurrentCameraType::Motor:
                    view_matrix = m_view_matrices[MAIN_VIEW_MATRIX_INDEX];
                    break;
                
                default:
                    break;
            }
            return view_matrix;
        }

        void setAspect(float aspect)
        {
            m_aspect = aspect;
            // degree m_fovxy °
            m_fovy = Radian(Math::atan(Math::tan(Radian(Degree(m_fovx) * 0.5f)) / m_aspect) * 2.0f).valueDegrees();
        }


        // camera vectors in world-space coordinates
        // Forward-Y
        Vector3 forward() const { return m_invRotation * Y; }
        Vector3 up() const { return m_invRotation * Z; }
        Vector3 right() const { return m_invRotation * X; }
    };

    inline const Vector3 VCamera::X = {1.0f, 0.0f, 0.0f};
    inline const Vector3 VCamera::Y = {0.0f, 1.0f, 0.0f};
    inline const Vector3 VCamera::Z = {0.0f, 0.0f, 1.0f};
} // namespace VE
