//
// Created by Dradozer on 05.04.2019.
//

#ifndef BA_GAIDA_FPSCOUNTER_H
#define BA_GAIDA_FPSCOUNTER_H

#include <time.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ba_gaida
{
    class FpsCounter
    {
    public:
        static void update(double deltaTime);

        static GLFWwindow *m_window;
        static std::string m_title;
    private:
        static double m_timePerFrame;
        static int m_Fps;
    };
}
#endif //BA_GAIDA_FPSCOUNTER_H
