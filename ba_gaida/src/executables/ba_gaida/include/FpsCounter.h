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
        FpsCounter(GLFWwindow *window);
        void update(double deltaTime);

    private:
        GLFWwindow *m_window;
        std::string m_title;
    public:
        void setTitle(const std::string &Title);

    private:
        double m_timePerFrame;
        int m_Fps;
    };
}
#endif //BA_GAIDA_FPSCOUNTER_H
