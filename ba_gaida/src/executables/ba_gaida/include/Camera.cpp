//
// Created by Dradozer on 05.04.2019.
//

#include "Camera.h"

ba_gaida::Camera::Camera(GLFWwindow *window,glm::vec3 center, glm::vec3 up, int width, int height)
{

    m_window = window;
    m_radius = center.z * 5.f;
    m_maxRadius = m_radius *1.5f;
    m_center = center;
    m_cameraPos = center + glm::vec3(0.f,0.f, m_radius);
    m_up = up;

    m_sensitivity = 0.01f;
    m_theta = glm::pi<float>() / 2.f;
    m_phi = 0.f;

    m_oldX = width / 2.f;
    m_oldY = height / 2.f;

    lookAt(m_cameraPos,m_center, m_up);
    m_projectionMatrix = glm::perspective(glm::radians(60.f), (float) width / height, 0.01f, 100.f);
    m_viewProjMatrix = m_viewMatrix * m_projectionMatrix;
}

ba_gaida::Camera::~Camera()
{

}

void ba_gaida::Camera::update()
{
    double x, y;

    glfwGetCursorPos(m_window, &x, &y);
    if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        m_changeX = ((float) x - m_oldX) * m_sensitivity;
        m_changeY = ((float) y - m_oldY) * m_sensitivity;

        m_theta -= m_changeY;
        if (m_theta < 0.01f) m_theta = 0.01f;
        else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

        m_phi -= m_changeX;
        if (m_phi < 0) m_phi += 2*glm::pi<float>();
        else if (m_phi > 2*glm::pi<float>()) m_phi -= 2*glm::pi<float>();
    }

    m_oldX = (float) x;
    m_oldY = (float) y;

    m_cameraPos.x = m_center.x + m_radius * sin(m_theta) * sin(m_phi);
    m_cameraPos.y = m_center.y + m_radius * cos(m_theta);
    m_cameraPos.z = m_center.z + m_radius * sin(m_theta) * cos(m_phi);

    m_viewMatrix = glm::lookAt( m_cameraPos, m_center, m_up);
}

void ba_gaida::Camera::lookAt(glm::vec3 camPos,glm::vec3 center, glm::vec3 up)
{
    m_cameraPos = camPos;
    m_center = center;

    m_up = up;

    m_viewMatrix = glm::lookAt(camPos, center, up);
    m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
}

glm::mat4 ba_gaida::Camera::GetViewProjMatrix() const
{
    return m_viewProjMatrix;
}

void ba_gaida::Camera::updateWidthHeight(int width, int height)
{
    m_projectionMatrix = glm::perspective(glm::radians(60.f), (float) width / height, 0.01f, 100.f);
    m_viewProjMatrix = m_viewMatrix * m_projectionMatrix;
}

void ba_gaida::Camera::updateWidthHeight(float fov, int width, int height, float near, float far)
{
    m_projectionMatrix = glm::perspective(glm::radians(fov), (float) width / height, near, far);
    m_viewProjMatrix = m_viewMatrix * m_projectionMatrix;
}

const glm::vec3 &ba_gaida::Camera::getCameraPos() const
{
    return m_cameraPos;
}

const glm::mat4 &ba_gaida::Camera::getViewMatrix() const
{
    return m_viewMatrix;
}

const glm::mat4 &ba_gaida::Camera::getProjectionMatrix() const
{
    return m_projectionMatrix;
}

const glm::vec3 &ba_gaida::Camera::getCameraCenter() const
{
    return m_center;
}

void ba_gaida::Camera::setCenter(const glm::vec3 &m_center)
{
    Camera::m_center = m_center;
}

void ba_gaida::Camera::setRadius(const float &radius)
{
    Camera::m_radius -= radius;
    if(m_radius < 1.f)
    {
        m_radius = 1.f;
    }
    else if(m_radius > m_maxRadius)
    {
        m_radius = m_maxRadius;
    }
}

const glm::vec3 &ba_gaida::Camera::getUp() const
{
    return m_up;
}

