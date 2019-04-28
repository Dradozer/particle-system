//
// Created by Dradozer on 02.04.2019.
//

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <string.h>
#include <sstream>
#include <GL/wglew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "include/PerformanceSettings.h"
#include "include/ParticleSystem.h"
#include "include/Objects/CVK_Sphere.h"
#include "include/Objects/CVK_Cube.h"

//particleCount is multiplied by 128, keep it between 64 and 256 for now
#define particleCount   64
#define Title "ba_gaida"
#define WIDTH 1024
#define HEIGTH 768
//VSync, parameter must be 0 or 1, 0 -> disabled , 1 -> enabled
#define VSync FALSE

ba_gaida::ParticleSystem *particleSystem;
GLFWwindow *window;

void resizeCallback(GLFWwindow *window, int w, int h)
{
    particleSystem->m_camera->updateWidthHeight(w, h);
    glViewport(0, 0, w, h);
    }

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // modes
//	if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
//		setAllUniforms(honey);
//		std::cout << "Parameters set to Honey." << std::endl;
//	}
}

void handleInput(GLFWwindow *window, float deltaTime)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    // normal WASD-movement in scene
    glm::vec3 cameraPos = particleSystem->m_camera->getCameraPos();
    glm::vec3 cameraCenter = particleSystem->m_camera->getCameraCenter();
    glm::vec3 dir = glm::normalize(cameraCenter - cameraPos);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraCenter += dir * deltaTime * 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraCenter -= dir * deltaTime * 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraCenter += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * 2.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        cameraCenter += glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime * 2.0f;
    }
    particleSystem->m_camera->setCenter(cameraCenter);


}


int main()
{
    glfwInit();
    if (glfwInit() == FALSE)
    {
        std::cout << "Could not initialize GLFW!" << std::endl;
    } else
    {
        std::cout << "GLFW " << glfwGetVersionString() << " initialized" << std::endl;
    }
    window = glfwCreateWindow(WIDTH, HEIGTH, Title, 0, 0);

    glfwSetWindowPos(window, 100, 50);
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glewInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glewExperimental = GL_TRUE;
    if (glGetString(GL_VERSION) == NULL)
    {
        std::cout << "Could not initialize OpenGL!" << std::endl;
    } else
    {
        std::cout << "OpenGL " << glGetString(GL_VERSION) << " initialized" << std::endl;
    }

    GLenum glError;
    if ((glError = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "1.OpenGL-Error: " << glError << std::endl;
    }
    glfwSwapInterval(VSync);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    srand(time(0));
    particleSystem = new ba_gaida::ParticleSystem(window, particleCount, WIDTH, HEIGTH, glm::uvec3(5));
#ifdef maxFPS
    particleSystem->m_fps->setTitle(Title);
#endif
    glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 0.f);
    glViewport(0, 0, WIDTH, HEIGTH);
    if ((glError = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "2.OpenGL-Error: " << glError << std::endl;
    }

    double time = glfwGetTime();
    double deltaTime;

    while (!glfwWindowShouldClose(window))
    {
        deltaTime = glfwGetTime() - time;
        time = glfwGetTime();
        glfwPollEvents();
        handleInput(window, deltaTime);

        particleSystem->update(deltaTime);
    }

    if ((glError = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "3.OpenGL-Error: " << glError << std::endl;
    }
    //CleanUp

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
