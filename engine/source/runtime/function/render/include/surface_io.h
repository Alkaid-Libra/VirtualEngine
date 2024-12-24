#pragma once

#include <GLFW/glfw3.h>

namespace VE
{
    class SurfaceIO
    {


    public:

        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        const char* getTitle() const { return m_title; }



        GLFWwindow* m_window{nullptr};

        const char* m_title;
        uint32_t m_width;
        uint32_t m_height;

        SurfaceIO(const char* title, uint32_t width, uint32_t height)
        {
            m_width = width;
            m_height = height;
            m_title = title;
        }




        int initialize()
        {
            if (!glfwInit())
            {
                return -1;
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            m_window = glfwCreateWindow(getWidth(), getHeight(), getTitle(), NULL, NULL);
            // m_window = glfwCreateWindow(1080, 960, getTitle(), NULL, NULL);
            if (!m_window)
            {
                glfwTerminate();
                return -1;
            }

            // Setup input callbacks


            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
            return 0;
        }
        bool tick_NotQuit()
        {
            while (!glfwWindowShouldClose(m_window))
            {
                glfwPollEvents();
                return true;
            }
            return false;
        }
        int clear()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();
            return 0;
        }
    };
} // namespace VE
