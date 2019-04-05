//
// Created by Dradozer on 03.04.2019.
//

#include "ParticleSystem.h"

ba_gaida::ParticleSystem::ParticleSystem(const GLFWwindow *window, const int particleCount, Camera *camera)
{
    m_particleCount = particleCount;
    m_Boxsize = 10.0f;
    m_camera = camera;
}

ba_gaida::ParticleSystem::~ParticleSystem()
{
    delete m_camera;
}

void ba_gaida::ParticleSystem::update(const double deltaTime)
{
    //todo executeComputeShader
}

void ba_gaida::ParticleSystem::render(GLFWwindow *window)
{
    //todo überarbeiten

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    glUseProgram(m_rID);
//
//    glUniform3fv(m_uniform_camPos, 1, glm::value_ptr(m_camera->getCameraPos()));
//    glUniformMatrix4fv(m_uniform_viewM, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
//    glUniformMatrix4fv(m_uniform_projM, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_vel[0]);
//    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(1);
//
//    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_pos[0]);
//    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
//    glEnableVertexAttribArray(0);
//
//
//    glDrawArrays(GL_POINTS, 0, m_particleCount);

    glUseProgram(0);

    glfwSwapBuffers(window);

}

void ba_gaida::ParticleSystem::setVariables(const int index, float value)
{

}

void ba_gaida::ParticleSystem::init()
{

}

