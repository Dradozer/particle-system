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
#include <math.h>
#include "Camera.h"
#include "ComputeShader.h"
#include "Shader.h"
#include "SSBO.h"
#include "FpsCounter.h"
#include "Particle.h"
#include "Grid.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ba_gaida
{
    class ParticleSystem
    {
    public:
        Camera *m_camera;
        FpsCounter *m_fps;

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
        void update(double deltaTime);

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
         * initalize the Grid-Structure
         */
        void initGrid();

        /**
         * initializes the uniformVariables
         */
        void initShader();

        /**
         * sets the Id depending on the grid dimensions: x * dimensions^2 + y * dimensions + z
         * @param id The ShaderID-Pointer
         */
        void setUniform(GLuint *id);

        void setUniformPrefix(GLuint *id);

        void setUniformParticles(GLuint *id);

        GLFWwindow *m_window;

        GLuint m_resetParticleID[4];
        GLuint m_resetGridID[4];
        GLuint m_lableParticleID[4];
        GLuint m_prefixSumInitID[4];
        GLuint m_prefixSumCalcID[4];
        GLuint m_prefixSumBufferID[4];
        GLuint m_prefixSumExcludeID[4];
        GLuint m_rearrangingParticlesID[4];
        GLuint m_densityID[6];
        GLuint m_arbitraryID[6];
        GLuint m_calcForcesID[6];
        GLuint m_swapParticlesID[4];
        GLuint m_updateForceID[6];
        GLuint m_collisionID[4];
        GLuint m_renderID;

        GLuint m_uniform_viewM;
        GLuint m_uniform_projM;
        GLuint m_uniform_camPos;

        Particle *m_particle;
        Grid *m_eulerianGrid;
        int *m_gridBuffer;

        GLuint m_ssbo_particleId[2];
        GLuint m_ssbo_gridId;
        GLuint m_ssbo_gridBufferID;

        glm::vec3 m_boxCenter;

        int m_heigth;
        int m_width;
        int m_step;
        unsigned int m_iterations;
        unsigned int m_particleCount;
        glm::ivec4 m_dimensions;
        glm::uvec3 m_boxSize;
        const glm::vec3 m_gravityV4 = glm::vec3(0.f, -9.80665, 0.f);
        glm::vec3 m_externalForce;
        glm::vec4 m_Forces;
        glm::vec4 m_settings;
        bool m_running;
        bool m_reset;
        bool m_gravity;
        bool m_imgui_once;
        float m_imgui_applications;
        float m_timeMultiplyer;

    };
}
#endif //BA_GAIDA_PARTIKELSYSTEM_H
