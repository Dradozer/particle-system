//
// Created by Dradozer on 05.04.2019.
//

#include "FpsCounter.h"

std::string ba_gaida::FpsCounter::m_title;
double ba_gaida::FpsCounter::m_timePerFrame;
int ba_gaida::FpsCounter::m_Fps;
GLFWwindow *ba_gaida::FpsCounter::m_window;

template<typename T>
std::string ToString(T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void ba_gaida::FpsCounter::update(double deltaTime)
{
    m_timePerFrame += deltaTime;
    m_Fps++;
    if (m_timePerFrame >= 1.0f)
    {
        std::string title = m_title + " | FPS: " + ToString(m_Fps);
        glfwSetWindowTitle(m_window, title.c_str());
        m_Fps = 0;
        m_timePerFrame = 0.0f;
    }
}
