//
// Created by pasab on 27.03.2019.
//

#ifndef CPP_PROJEKTE_PARTICLESYSTEM_CPU_V1_H
#define CPP_PROJEKTE_PARTICLESYSTEM_CPU_V1_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>

#include "../../ParticleSystemParent.h"
#include "../../Shader.h"

namespace ParticleSystem::CPU {

    class ParticleSystem_v1 : public ParticleSystemParent {
        //Constructor & Destructor
        public:
            ParticleSystem_v1();
            ParticleSystem_v1(const unsigned int nCount);
            ParticleSystem_v1(const unsigned int nCount, glm::vec3 &origin);
            ~ParticleSystem_v1();

        //System Functions
        public:
            //Needed Functions from parent class
            void update(GLFWwindow* window, const double delteTime);
            void render();

        private:
            //just to dont repeat the code for every constructor
            void Init();

        private:
            //Data to the Particles
            glm::vec4*      m_particlePos;
            glm::vec4*      m_particleVel;
            unsigned int*   m_indexList;

            //gl shader and buffer ids
            GLuint          m_vertexBuffer;
            GLuint          m_vertexArrayObject;
            GLuint          m_indexListBuffer;

            GLuint          m_shaderProgram;
            GLuint          m_uniformViewMat;
            GLuint          m_uniformProjMat;


    };

}

#endif //CPP_PROJEKTE_PARTICLESYSTEM_CPU_V1_H
