//
// Created by Dradozer on 05.04.2019.
//

#include "FpsCounter.h"
ba_gaida::FpsCounter::FpsCounter(GLFWwindow *window ,const int timerQuantity)
{
    m_window = window;
    m_timePerFrame = 0.f;
    m_Fps = 0;
    m_lastFPS = 0;

    m_lastTimer = 0.0f;
    m_iterations = new int [timerQuantity]{0};
    m_timeStamps = new float [timerQuantity]{0.f};
    m_timeCounter = new float [timerQuantity]{0.f};
}

ba_gaida::FpsCounter::FpsCounter(GLFWwindow *window)
{
    m_window = window;
    m_timePerFrame = 0.f;
    m_Fps = 0;
    m_lastFPS = 0;
}
ba_gaida::FpsCounter::FpsCounter(const int timerQuantity)
{
    m_lastTimer = 0.0f;
    m_iterations = new int [timerQuantity]{0};
    m_timeStamps = new float [timerQuantity]{0.f};
    m_timeCounter = new float [timerQuantity]{0.f};
}

template<typename T>
std::string ToString(T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void ba_gaida::FpsCounter::update(const double deltaTime)
{
    m_timePerFrame += deltaTime;
    m_Fps++;
    if (m_timePerFrame >= 1.f)
    {
        m_lastFPS = m_Fps;
        std::string title = m_title + " | FPS: " + ToString(m_Fps);
        glfwSetWindowTitle(m_window, title.c_str());
        m_Fps = 0;
        m_timePerFrame = 0.f;
    }
}

int ba_gaida::FpsCounter::getFPS()
{
    return m_lastFPS;
}

void ba_gaida::FpsCounter::setTitle(const std::string &Title)
{
    m_title = Title;
}

void ba_gaida::FpsCounter::resetTimestamp()
{
    m_lastTimer = glfwGetTime();
}

void ba_gaida::FpsCounter::setTimestamp( const int index)
{
    m_iterations[index]++;
    m_timeCounter[index] += glfwGetTime() - m_lastTimer;
    if(m_iterations[index] >= 1000)
    {
        m_timeStamps[index] = m_timeCounter[index]/1000.f;
        m_iterations[index] = 0;
        m_timeCounter[index] = 0.f;
    }
    m_lastTimer = glfwGetTime();
}

float ba_gaida::FpsCounter::getTimestamp(const int index)
{
    return m_timeStamps[index];
}

ba_gaida::FpsCounter::~FpsCounter()
{
    delete m_iterations;
    delete m_timeStamps;
    delete m_timeCounter;
}
