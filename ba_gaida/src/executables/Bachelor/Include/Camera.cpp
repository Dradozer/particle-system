//
// Created by pasab on 28.03.2019.
//

#include "Camera.h"


void ParticleSystem::Camera::Camera::lookAt(glm::vec3 center, glm::vec3 up, glm::vec3 camPos)
{
    m_center = center;
    m_cameraPos = camPos;
    m_up = up;

    m_viewMatrix = glm::lookAt(camPos, center, up);
}
