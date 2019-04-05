//
// Created by pasab on 27.03.2019.
//

#ifndef CPP_PROJEKTE_PARTICLESYSTEM_GPU_V1_H
#define CPP_PROJEKTE_PARTICLESYSTEM_GPU_V1_H

#include <glm/glm.hpp>

#include "../../ParticleSystemParent.h"

namespace ParticleSystem::GPU
{

    class ParticleSystem_v1 : public ParticleSystemParent {
        //Constructor & Destructor
        public:
            ParticleSystem_v1();
            ParticleSystem_v1(const unsigned int nCount);
            ParticleSystem_v1(const unsigned int nCount, glm::vec3 origin);

            //System Functions
        public:
            //Needed Functions from parent class
            void update(const double delteTime);
            void render();
            GLuint getProjViewUniformLocation() const;

        private:


        private:
    };

}

#endif //CPP_PROJEKTE_PARTICLESYSTEM_GPU_V1_H
