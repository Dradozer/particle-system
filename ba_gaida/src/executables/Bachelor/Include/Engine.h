//
// Created by pasab on 26.03.2019.
//

#ifndef CPP_PROJEKTE_ENGINE_H
#define CPP_PROJEKTE_ENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include "ParticleSystemParent.h"
#include "Camera.h"

//TODO: Shader-Klasse implementieren

namespace ParticleSystem
{

    class Engine {
        //Constructor and Destructor
        public:
            Engine();
            ~Engine();

        //OpenGL/GLFW Setup functions
        public:
            void setWindowSize(const int width, const int height);
            void setWindowTitle(const char *title);
            void setWindowSizeCallback(void (&f)(GLFWwindow *, int, int));
            void setWindowKeyCallback(void (&f)(GLFWwindow *, int, int, int, int));

        //Engine Functions
        public:
            //make the system that 1 or more particle systems can be rendered
            void addParticleSystem(ParticleSystemParent* pSystem);

            //set the camera you want
            //void setCamera(Camera::Camera* camera);

            //main loop to handle everything
            void loop();

        //OpenGL/GLFW members
        private:
            //handle to the window we are drawing in
            GLFWwindow      *m_window;

            //measure frametime and fps
            double          m_deltaTime;
            double          m_lastDelta;
            double          m_timer;
            unsigned int    m_fps;
            unsigned int    m_frame;

        //Engine members
        private:
            //vector that holds all particle system to update and render
            std::vector<ParticleSystemParent*>  m_particleSystems;

        //Private Engine Functions
        private:
            //calculate the frametime
            void calcFrameTime();
    };
}

#endif //CPP_PROJEKTE_ENGINE_H
