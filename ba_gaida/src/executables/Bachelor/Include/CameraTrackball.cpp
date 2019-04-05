//
// Created by pasab on 28.03.2019.
//

#include "CameraTrackball.h"


ParticleSystem::Camera::CameraTrackball::CameraTrackball(glm::vec3 center, glm::vec3 up, glm::vec3 camPos, const float width, const float height)
{
    m_center = center;
    m_cameraPos = camPos;
    m_up = up;

    m_sensitivity = 0.010f;
    m_zoomSensitivity = 5.0f;
    m_theta = glm::pi<float>() / 2.0f;
    m_phi = 0.f;
    m_radius = 1.5;

    m_oldX = width/2.f;
    m_oldY = height/2.f;

    lookAt(center, up, camPos);
    m_projectionMatrix = glm::perspective(glm::radians(90.0f), width / height, 0.01f, 50.0f);
}

ParticleSystem::Camera::CameraTrackball::~CameraTrackball()
{

}

void ParticleSystem::Camera::CameraTrackball::setProjection(const float fov, const float ratio, const float near, const float far)
{
    m_projectionMatrix = glm::perspective(glm::radians(fov), ratio, near, far);
}

void ParticleSystem::Camera::CameraTrackball::update(GLFWwindow* window, float deltaTime, GLuint uniformView, GLuint unifromProj)
{
    double x, y;

    glfwGetCursorPos(window, &x, &y);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        float changeX = ((float) x - m_oldX) * m_sensitivity;
        float changeY = ((float) y - m_oldY) * m_sensitivity;

        m_theta -= changeY;
        if (m_theta < 0.01f) m_theta = 0.01f;
        else if (m_theta > glm::pi<float>() - 0.01f) m_theta = glm::pi<float>() - 0.01f;

        m_phi -= changeX;
        if (m_phi < 0) m_phi += 2*glm::pi<float>();
        else if (m_phi > 2*glm::pi<float>()) m_phi -= 2*glm::pi<float>();
    }

    m_oldX = (float) x;
    m_oldY = (float) y;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        m_radius -= m_zoomSensitivity * (float)deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        m_radius += m_zoomSensitivity * (float)deltaTime;
    if (m_radius < 0.1f) m_radius = 0.1f;

    m_cameraPos.x = m_center.x + m_radius * sin(m_theta) * sin(m_phi);
    m_cameraPos.y = m_center.y + m_radius * cos(m_theta);
    m_cameraPos.z = m_center.z + m_radius * sin(m_theta) * cos(m_phi);

    m_viewMatrix = glm::lookAt(m_cameraPos, m_center, m_up);
    glUniformMatrix4fv(uniformView, 1 , GL_FALSE, glm::value_ptr(m_viewMatrix));
    glUniformMatrix4fv(unifromProj, 1 , GL_FALSE, glm::value_ptr(m_projectionMatrix));
}