//
// Created by pasab on 28.03.2019.
//

#ifndef CPP_PROJEKTE_CAMERATRACKBALL_H
#define CPP_PROJEKTE_CAMERATRACKBALL_H

#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Camera.h"

namespace ParticleSystem::Camera {

    class CameraTrackball : public Camera {
        //Constructor & Destructor
        public:
            CameraTrackball(glm::vec3 center, glm::vec3 up, glm::vec3 camPos, const float width, const float height);
            ~CameraTrackball();

        //needed functions
        public:
            void update(GLFWwindow* window, float deltaTime, GLuint uniformView, GLuint unifromProj);

            void setProjection(const float fov, const float ratio, const float near, const float far);

        //private members
        private:
            float m_sensitivity;
            float m_zoomSensitivity;
            float m_theta, m_phi, m_radius;
            float m_oldX = 0, m_oldY = 0;
    };

}

#endif //CPP_PROJEKTE_CAMERATRACKBALL_H
