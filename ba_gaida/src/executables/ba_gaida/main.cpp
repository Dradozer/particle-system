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

#include "include/FpsCounter.h"
#include "include/ParticleSystem.h"
#include "include/Objects/CVK_Sphere.h"
#include "include/Objects/CVK_Cube.h"

//particleCount is multiplied by 128, keep it between 64 and 256 for now
#define particleCount   32

#define WIDTH 1024
#define HEIGTH 768
#define Title "ba_gaida"

ba_gaida::ParticleSystem *particleSystem;
GLFWwindow *window;

void resizeCallback(GLFWwindow *window, int w, int h)
{
    particleSystem->m_camera->updateWidthHeight(w, h);
    glViewport(0, 0, w, h);
}

void setAllUniforms(float v[6])
{
    particleSystem->setVariables(0, v[0]); // lin vis
    particleSystem->setVariables(1, v[1]); // quad vis
    particleSystem->setVariables(2, v[2]); // rest dens
    particleSystem->setVariables(3, v[3]); // k
    particleSystem->setVariables(4, v[4]); // near k
//	particleSystem->setVariables(5, v[5]); // g
    particleSystem->setVariables(6, v[6]); // red
    particleSystem->setVariables(7, v[7]); // green
    particleSystem->setVariables(8, v[8]); // blue
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // modes
//	if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
//		setAllUniforms(honey);
//		std::cout << "Parameters set to Honey." << std::endl;
//	}
}

void handleInput(GLFWwindow *w, float deltaTime)
{
    // normal WASD-movement in scene
    glm::vec3 cameraPos = particleSystem->m_camera->getCameraPos();
    glm::vec3 cameraCenter = particleSystem->m_camera->getCameraCenter();
    glm::vec3 dir = glm::normalize(cameraCenter - cameraPos);
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraCenter += dir * deltaTime * 2.0f;
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraCenter -= dir * deltaTime * 2.0f;
    }
    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraCenter += glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime * 2.0f;
    }
    if (glfwGetKey(w, GLFW_KEY_F) == GLFW_PRESS)
    {
        cameraCenter += glm::vec3(0.0f, -1.0f, 0.0f) * deltaTime * 2.0f;
    }
    particleSystem->m_camera->setCenter(cameraCenter);
}


int main()
{
    if (!glfwInit())
    {
        std::cout << "Could not initialize GLFW!" << std::endl;
    } else
    {
        std::cout << "GLFW  initialized" << std::endl;
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

    //VSync, parameter must be 0 or 1, 0 -> disabled , 1 -> enabled
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ba_gaida::FpsCounter();
    ba_gaida::FpsCounter::m_window = window;
    ba_gaida::FpsCounter::m_title = Title;
    ba_gaida::Camera camera = ba_gaida::Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                                               glm::vec3(0.0f, 5.0f, 0.0f), WIDTH, HEIGTH);
    srand(time(0));
    particleSystem = new ba_gaida::ParticleSystem(window, particleCount, &camera, glm::uvec3(5));
    glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 0.f);
    glViewport(0, 0, WIDTH, HEIGTH);

    double time = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double deltaTime = glfwGetTime() - time;
        time = glfwGetTime();

        particleSystem->update(deltaTime);

        handleInput(window, deltaTime);

        particleSystem->m_camera->update(window, 0);
        particleSystem->render(window);

        ba_gaida::FpsCounter::update(deltaTime);

        glfwPollEvents();
    }
    //CleanUp
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
