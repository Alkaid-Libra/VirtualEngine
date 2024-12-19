#pragma once

#include "runtime/core/base/public_singleon.h"

namespace VE
{
    enum class EditorCommand : unsigned int
    {
        camera_left = 1 << 0, // A
        camera_back = 1 << 1, // S
        camera_forward = 1 << 2, // W
        camera_right = 1 << 3, // D
        camera_up = 1 << 4, // Q
        camera_down = 1 << 5, // E
        translation_mode = 1 << 6, // T
        rotation_mode = 1 << 7, // R
        scale_mode = 1 << 8, // C
        exit = 1 << 9, // Esc
        delete_object = 1 << 10, // Delete
    };

    enum class GameCommand : unsigned int
    {
        forward = 1 << 0, // W
        backward = 1 << 1, // S
        left = 1 << 2, // A
        right = 1 << 3, // D
        jump = 1 << 4, // not implemented yet
        squat = 1 << 5, // not implemented yet
        sprint = 1 << 6, // LEFT SHIFT
        fire = 1 << 7, // not implemented yet
        invalid = (unsigned int)(1 << 31) // lost focus

    };

    class InputSystem : public PublicSingleton<InputSystem>
    {
        friend class PublicSingleton<InputSystem>;

    public:
        void tick();

    protected:
        InputSystem() = default;
    };
} // namespace VE
