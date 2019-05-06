//
// Created by Dradozer on 03.04.2019.
//

#ifndef BA_GAIDA_PARTICLESYSTEM_H
#define BA_GAIDA_PARTICLESYSTEM_H

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>
#include <time.h>
#include "Camera.h"
#include "ComputeShader.h"
#include "Shader.h"
#include "SSBO.h"
#include "FpsCounter.h"
#include "PerformanceSettings.h"
#include "Particle.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ba_gaida
{
    class ParticleSystem
    {
    public:
        Camera *m_camera;
        FpsCounter  *m_fps;

        /**
        * Constructor of the ParticleSystem
        * @param window The window of the ParticleSystem
        * @param particleCount The number of the initial particle
        * @param camera The corresponding camera
        * @param WIDTH Window width
        * @param HEIGTH Window height
        * @param boxSize size of the box the particle are spawned in
        */
        ParticleSystem(GLFWwindow *window, const int particleCount, const int WIDTH, const int HEIGTH,
                       const glm::uvec3 boxSize);

        /**
         * Destroys all context of the Particlesystem
         */
        ~ParticleSystem();

        /**
         * Updates the ComputeShader of ParticleSystem
         * @param deltaTime The time used for the frame
         */
        void update(const double deltaTime);

        void setVariables(const int index, float value);

    private:

        /**
         * Renderer the result
         */
        void render();

        /**
         * Runs all inits
         */
        void init();

        /**
         * initializes the Particle Position
         */
        void initParticle();

        /**
         *
         * @param id The ShaderID-Pointer
         * @param particleCount
         */
        void setUniform(GLuint *id, const int particleCount);

        GLFWwindow *m_window;

        GLuint m_externalForceID[3];

        GLuint m_renderID;
        GLuint m_uniform_viewM;
        GLuint m_uniform_projM;
        GLuint m_uniform_camPos;

        Particle *m_particle;

        GLuint m_ssbo_particleId[2];

        glm::vec3 m_boxCenter;

        int m_heigth;
        int m_width;
        int m_particleCount;
        glm::uvec3 m_Boxsize;
#ifndef maxFPS
        bool m_imgui_once;
        float m_imgui_applications;
        ImVec4 m_imgui_clear_color; // ClearColor >> sets background color
#endif

    };
}
#endif //BA_GAIDA_PARTIKELSYSTEM_H
