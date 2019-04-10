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
#include "Shader.h"

namespace ba_gaida
{
    class ParticleSystem
    {
    public:
        int m_particleCount;
        Camera *m_camera;
        float m_Boxsize;

        GLuint m_renderID;
        GLuint m_uniform_viewM;
        GLuint m_uniform_projM;
        GLuint m_uniform_camPos;

        /**
        * Constructor of the ParticleSystem
        * @param window The window of the ParticleSystem
        * @param particleCount The number of the initial particle
        * @param camera The corresponding camera
        */

        ParticleSystem(const GLFWwindow *window, const int particleCount, Camera *camera);

        ~ParticleSystem();

        /**
         * Updates the ComputeShader of ParticleSystem
         * @param deltaTime The time used for the frame
         */
        void update(const double deltaTime);

        /**
         * Renderer the result
         * @param window Render in this window
         */
        void render(GLFWwindow *window);

        void setVariables(const int index, float value);

    private:
        void init();
    };
}
#endif //BA_GAIDA_PARTIKELSYSTEM_H
