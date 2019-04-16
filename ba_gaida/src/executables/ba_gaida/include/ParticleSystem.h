//
// Created by Dradozer on 03.04.2019.
//

#ifndef BA_GAIDA_PARTICLESYSTEM_H
#define BA_GAIDA_PARTICLESYSTEM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <time.h>
#include "Camera.h"
#include "ComputeShader.h"
#include "Shader.h"

namespace ba_gaida
{
    class ParticleSystem
    {
    public:
        int m_particleCount;
        Camera *m_camera;
        glm::uvec3 m_Boxsize;


        /**
        * Constructor of the ParticleSystem
        * @param window The window of the ParticleSystem
        * @param particleCount The number of the initial particle
        * @param camera The corresponding camera
        */

        ParticleSystem(const GLFWwindow *window, const int particleCount, Camera *camera, const glm::uvec3 boxSize);

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

        template<typename T>
        void createSSBO(GLuint &id, int bindingID, int size, T *data);

    private:
        void init();

        GLuint m_renderID;
        GLuint m_uniform_viewM;
        GLuint m_uniform_projM;
        GLuint m_uniform_camPos;

        glm::vec4 *m_particle_pos;
        glm::vec4 *m_particle_vel;

        GLuint m_ssbo_pos_id[2];
        GLuint m_ssbo_vel_id[2];

        glm::vec3 m_boxCenter;
    };
}
#endif //BA_GAIDA_PARTIKELSYSTEM_H
