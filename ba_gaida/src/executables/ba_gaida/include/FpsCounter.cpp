//
// Created by Dradozer on 05.04.2019.
//

#include "FpsCounter.h"

ba_gaida::FpsCounter::FpsCounter(GLFWwindow *window)
{
    m_window = window;
    m_timePerFrame = 0.0f;
    m_Fps = 0;
}

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

void ba_gaida::FpsCounter::setTitle(const std::string &Title)
{
    m_title = Title;
}


