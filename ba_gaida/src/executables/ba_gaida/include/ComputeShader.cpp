//
// Created by Dradozer on 12.04.2019.
//

#include "ComputeShader.h"

void ba_gaida::ComputeShader::createComputeShader(GLuint &id, const char *path)
{
    Shader::attachShader(id, GL_COMPUTE_SHADER, path);
    Shader::linkShader(id);
}

void ba_gaida::ComputeShader::updateComputeShader(GLuint *id, const float deltaTime, const int particleCount)
{
    glUseProgram(id[0]);
    {
        glUniform1f(id[1],deltaTime);
        glUniform1f(id[2],particleCount);

        glDispatchCompute(particleCount/128,1,1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
    glUseProgram(0);
}

template<typename T>
void ba_gaida::ComputeShader::createSSBO(GLuint &id, int bindingID, int size, T *data)
{
    glCreateBuffers(1, &id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER,id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingID, id);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
}
