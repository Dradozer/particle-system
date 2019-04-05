//
// Created by pasab on 27.03.2019.
//

#include <random>
#include "ParticleSystem_v1.h"

ParticleSystem::CPU::ParticleSystem_v1::ParticleSystem_v1()
{
    m_particleCount = 1000;
    m_origin = glm::vec3(0.0f);
    this->Init();
}

ParticleSystem::CPU::ParticleSystem_v1::ParticleSystem_v1(const unsigned int nCount)
{
    m_particleCount = nCount;
    m_origin = glm::vec3(0.0f);
    this->Init();
}

ParticleSystem::CPU::ParticleSystem_v1::ParticleSystem_v1(const unsigned int nCount, glm::vec3 &origin)
{
    m_particleCount = nCount;
    m_origin = origin;
    this->Init();
}

ParticleSystem::CPU::ParticleSystem_v1::~ParticleSystem_v1()
{
    //cleanup
    delete m_particlePos;
    delete m_particleVel;
    delete m_indexList;

    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_indexListBuffer);
    glDeleteBuffers(1, &m_vertexArrayObject);

    Shader::DeleteShaders(m_shaderProgram);
}

void ParticleSystem::CPU::ParticleSystem_v1::update(GLFWwindow *window, const double deltaTime)
{
    //update the camera
    glUseProgram(m_shaderProgram);
    ParticleSystem::Camera::Camera *camera = this->getActiveCamera();
    camera->update(window, deltaTime, m_uniformViewMat, m_uniformProjMat);
    glUseProgram(0);


    //update particles

}

void ParticleSystem::CPU::ParticleSystem_v1::render()
{
    glUseProgram(m_shaderProgram);

    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_POINTS, m_particleCount, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
}

void ParticleSystem::CPU::ParticleSystem_v1::Init()
{
    m_particlePos = new glm::vec4[m_particleCount];
    m_particleVel = new glm::vec4[m_particleCount];
    m_indexList   = new unsigned int[m_particleCount];

    //initialize particles with random values
    std::random_device rd;
    std::default_random_engine e2(rd());
    std::uniform_real_distribution<> dist(-1.0f, 1.0f);

    for (size_t i = 0; i < m_particleCount; i++)
    {
        m_particlePos[i] = glm::vec4(dist(e2), dist(e2), dist(e2), 0.0f);
        m_indexList[i] = i;
    }

    //generate vertex buffers
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_particleCount * sizeof(glm::vec4), &m_particlePos[0], GL_STATIC_DRAW);

    //generate index list
    glGenBuffers(1, &m_indexListBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexListBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_particleCount * sizeof(unsigned int), &m_indexList[0], GL_STATIC_DRAW);

    glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexListBuffer);
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

    //initialize the shaders
    Shader::AttachShader(m_shaderProgram, GL_VERTEX_SHADER, SHADERS_PATH "/Bachelor/vertexShader.glsl");
    Shader::AttachShader(m_shaderProgram, GL_FRAGMENT_SHADER, SHADERS_PATH "/Bachelor/fragmentShader.glsl");
    Shader::LinkProgram(m_shaderProgram);

    //locate the uniforms
    int uniformViewMat = glGetUniformLocation(m_shaderProgram, "viewMatrix");
    int uniformProjMat = glGetUniformLocation(m_shaderProgram, "projMatrix");

    if (uniformProjMat < 0 || uniformViewMat < 0)
        std::cout << "Could not find Uniform Locations!" << std::endl;

    m_uniformProjMat = uniformProjMat;
    m_uniformViewMat = uniformViewMat;

}