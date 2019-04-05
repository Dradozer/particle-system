//
// Created by pasab on 27.03.2019.
//

#ifndef CPP_PROJEKTE_PARTICLESYSTEMPARENT_H
#define CPP_PROJEKTE_PARTICLESYSTEMPARENT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>

#include "Camera.h"

namespace ParticleSystem
{

    class ParticleSystemParent {
        //Engine Functions
        public:
            //make sure the update and render method is implemented in
            //the subclasses, becuase they might handle things differently
            virtual void update(GLFWwindow* window, const double deltaTime) = 0;
            virtual void render() = 0;
            unsigned int getParticleCount() const;

            //function to add and control cameras
            void addCamera(Camera::Camera* camera);
            void activateCamera(const unsigned int id);
            void activateCamera(const Camera::Camera* camera);
            Camera::Camera* getActiveCamera();

        protected:
            unsigned int    m_particleCount;
            glm::vec3       m_origin;
            unsigned int    m_currentCamera;

            std::vector<Camera::Camera*> m_cameras;

        private:


    };

}

#endif //CPP_PROJEKTE_PARTICLESYSTEMPARENT_H
