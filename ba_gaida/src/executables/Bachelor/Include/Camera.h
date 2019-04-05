//
// Created by pasab on 28.03.2019.
//

#ifndef CPP_PROJEKTE_CAMERA_H
#define CPP_PROJEKTE_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ParticleSystem::Camera {

    class Camera {
        //needed functions
        public:
            virtual void update(GLFWwindow* window, float deltaTime, GLuint uniformID, GLuint unifromProj) = 0;

            virtual void setProjection(const float fov, const float ratio, const float near, const float far) = 0;
            void lookAt(glm::vec3 center, glm::vec3 up, glm::vec3 camPos);

        //private member every camera needs
        protected:
            glm::vec3 m_cameraPos;
            glm::vec3 m_center;
            glm::vec3 m_up;

            glm::mat4 m_viewMatrix;
            glm::mat4 m_projectionMatrix;
    };

}

#endif //CPP_PROJEKTE_CAMERA_H
