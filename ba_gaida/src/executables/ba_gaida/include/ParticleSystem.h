//
// Created by Dradozer on 03.04.2019.
//

#ifndef BA_GAIDA_PARTICLESYSTEM_H
#define BA_GAIDA_PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <time.h>
#include "Camera.h"

namespace ba_gaida
{
    class ParticleSystem
    {
    public:
        int m_particleCount;
        Camera *m_camera;
        float m_Boxsize;

        ParticleSystem(const GLFWwindow *window, const int particleCount, Camera *camera);

        ~ParticleSystem();

        void update(const double deltaTime);

        void render(GLFWwindow *window);

        void setVariables(const int index, float value);

    private:
        void init();
    };
}
#endif //BA_GAIDA_PARTIKELSYSTEM_H
