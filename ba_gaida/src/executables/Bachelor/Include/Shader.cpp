//
// Created by pasab on 29.03.2019.
//

#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "Shader.h"


void ParticleSystem::Shader::AttachShader(GLuint &program, GLenum type, const char *path) {
    GLuint shader = LoadShader(type, path);

    if (glIsProgram(program) == false) {
        program = glCreateProgram();
    }

    glAttachShader(program, shader);
}

void ParticleSystem::Shader::LinkProgram(GLuint program) {
    glLinkProgram(program);
    ValidateProgram(program);
    glValidateProgram(program);
    ValidateProgram(program);
}

void ParticleSystem::Shader::DeleteShaders(GLuint program) {
    GLint maxCount = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &maxCount);

    if (maxCount != 0) {
        GLuint *shaders = new GLuint[maxCount];
        glGetAttachedShaders(program, maxCount, 0, shaders);
        for (int i = 0; i < maxCount; i++) {
            glDeleteShader(shaders[i]);
        }
        delete[] shaders;

    }

    glGetError();
}

GLuint ParticleSystem::Shader::LoadShader(GLenum type, const char *path) {
    std::string fileContent;
    std::string line;

    //open file and "parse" input
    std::ifstream file(path);
    if (file.is_open()) {
        while (!file.eof()) {
            getline(file, line);
            fileContent += line + "\n";
        }
        file.close();
        std::cout << "SUCCESS: Opened file " << path << std::endl;
    } else
        std::cout << "ERROR: Unable to open file " << path << std::endl;

    const char* source = fileContent.c_str();
    const GLint source_size = strlen(source);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &source_size);
    glCompileShader(shader);
    ValidateShader(shader, path);

    return shader;
}

void ParticleSystem::Shader::ValidateShader(GLuint shader, const char *path) {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        GLint errorLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errorLogLength);

        char *log = new char[errorLogLength];
        glGetShaderInfoLog(shader, errorLogLength, 0, log);

        std::cout << "Error when compiling the shader: " << path << std::endl;
        std::cout << log << std::endl;

        delete[] log;

        FAIL_GRACEFULLY
    }
}

void ParticleSystem::Shader::ValidateProgram(GLuint program) {
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE) {
        GLint errorLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorLogLength);

        char *log = new char[errorLogLength];
        glGetProgramInfoLog(program, errorLogLength, 0, log);

        std::cout << "Error when linking the shaders" << std::endl;
        std::cout << log << std::endl;

        delete[] log;

        FAIL_GRACEFULLY
    }
}