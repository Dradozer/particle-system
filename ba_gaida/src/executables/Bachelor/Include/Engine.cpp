//
// Created by pasab on 26.03.2019.
//

#include "Engine.h"

/*-------------------------------------------------------------------*/ //Constr. Destr.

ParticleSystem::Engine::Engine()
{
    m_timer = 0.0;

    //Init GLFW library
    if (!glfwInit())
    {
        std::cout << "Could not initialize GLFW!" << std::endl;
        exit(3);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Vsync?
    glfwSwapInterval(0);

    //Create a Window
    m_window = glfwCreateWindow(100, 100, "", 0, 0);
    if (!m_window) {
        glfwTerminate();
        std::cout << "Could not create Window!" << std::endl;
        exit(4);
    }

    glfwMakeContextCurrent(m_window);


    if (glGetError() != GL_NO_ERROR)
    {
        std::cout << "OpenGL Error: " << glGetError() << std::endl;
    }

    glewExperimental = GL_TRUE;

    //Init GLEW library
    glewInit();

    //Setup some GL magic
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Defaults for clear color and Viewport
    glClearColor(255, 255, 255, 0);
    glViewport(0, 0, 100, 100);

}

ParticleSystem::Engine::~Engine()
{
    std::cout << "Cleaning up!" << std::endl;
    //clean up everything
    for (size_t i = 0; i < m_particleSystems.size(); i++)
        delete m_particleSystems[i];

    //if (m_camera != nullptr)
    //    delete m_camera;

    //clean up OpenGL/GLFW
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/ //OPENGL/GLFW

void ParticleSystem::Engine::setWindowSize(const int width, const int height)
{
    glfwSetWindowSize(m_window, width, height);
    glViewport(0, 0, width, height);
}

void ParticleSystem::Engine::setWindowTitle(const char* title)
{
    glfwSetWindowTitle(m_window, title);
}

void ParticleSystem::Engine::setWindowSizeCallback(void (&f)(GLFWwindow*, int, int))
{
    glfwSetWindowSizeCallback(m_window, f);
}

void ParticleSystem::Engine::setWindowKeyCallback(void (&f)(GLFWwindow*, int, int, int, int))
{
    glfwSetKeyCallback(m_window, f);
}

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/ //Engine Func
void ParticleSystem::Engine::addParticleSystem(ParticleSystem::ParticleSystemParent *pSystem)
{
    //check if the given Particlesystem is already in our list
    auto p = std::find(m_particleSystems.begin(), m_particleSystems.end(), pSystem);

    //if not the insert it!
    if (p == m_particleSystems.end())
        m_particleSystems.push_back(pSystem);
}

void ParticleSystem::Engine::loop()
{
    while (!glfwWindowShouldClose(m_window))
    {
        //for fps counter and update step calculations
        calcFrameTime();

        //update the camera
        //if (m_camera != nullptr)
        //    m_camera->update(m_window, m_deltaTime, 0);

        //update every particle system
        for (size_t i = 0; i < m_particleSystems.size(); i++)
            m_particleSystems[i]->update(m_window, m_deltaTime);

        //clear previous frame
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        //now render every particle system
        for (size_t i = 0; i < m_particleSystems.size(); i++)
            m_particleSystems[i]->render();

        //show new rendered frame
        glfwSwapBuffers(m_window);

        //handle alle user events
        glfwPollEvents();
    }
}

void ParticleSystem::Engine::calcFrameTime()
{
    //TODO: m_timer + m_deltaTime is always -inf???????????
    //Difference to last frame time and count it so we can calc the fps
    m_deltaTime = glfwGetTime() - m_lastDelta;

    m_timer = m_timer + m_deltaTime;

    m_lastDelta = glfwGetTime();
    m_frame++;

    //if we measured over 1 second we got our fps
    //reset frame counter so the fps resets
    if (m_timer >= 1.0)
    {
        m_fps = m_frame;
        m_frame = 0;
        m_timer = 0.0;

        //for debug purposes
        std::cout << "FPS: " << m_fps << std::endl;
    }
}

//void ParticleSystem::Engine::setCamera(ParticleSystem::Camera::Camera *camera)
//{
//    m_camera = camera;
//}
/*-------------------------------------------------------------------*/