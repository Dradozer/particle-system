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
        FpsCounter(GLFWwindow *window ,const int timerQuantity);
        FpsCounter(GLFWwindow *window);
        FpsCounter(const int timerQuantity);
        ~FpsCounter();
        void update(const double deltaTime);
        int getFPS();
        void setTitle(const std::string &Title);

        void resetTimestamp();
        void setTimestamp( const int index);
        float getTimestamp(const int index);

    private:
        GLFWwindow *m_window;
        std::string m_title;
        float m_timePerFrame;
        int m_Fps;
        int m_lastFPS;

        float m_lastTimer;
        int *m_iterations;
        float *m_timeStamps;
        float *m_timeCounter;

    };
}
#endif //BA_GAIDA_FPSCOUNTER_H
