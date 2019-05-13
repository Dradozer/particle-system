//
// Created by Dradozer on 05.04.2019.
//

#ifndef BA_GAIDA_CAMERA_H
#define BA_GAIDA_CAMERA_H

#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ba_gaida
{
    class Camera
    {
    public:

        Camera(GLFWwindow *window, glm::vec3 center, glm::vec3 up, int width, int height);

        ~Camera();

        void update();

        void updateWidthHeight(int width, int heigth);

        void updateWidthHeight(float fov, int width, int height, float near, float far);

        void lookAt(glm::vec3 center, glm::vec3 up, glm::vec3 camPos);

        glm::mat4 GetViewProjMatrix() const;

        const glm::vec3 &getCameraPos() const;

        const glm::mat4 &getViewMatrix() const;

        const glm::mat4 &getProjectionMatrix() const;

        const glm::vec3 &getCameraCenter() const;

        void setCenter(const glm::vec3 &m_center);

        void setRadius(const float &radius);

        const glm::vec3 &getUp() const;


    private:
        float m_sensitivity;
        float m_theta, m_phi;
        float m_oldX, m_oldY;
        float m_radius;
        float m_maxRadius;
        float m_changeX;
        float m_changeY;

        GLFWwindow *m_window;

        glm::vec3 m_cameraPos;
        glm::vec3 m_center;
        glm::vec3 m_up;


        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewProjMatrix;

    };
}

#endif //BA_GAIDA_CAMERA_H
