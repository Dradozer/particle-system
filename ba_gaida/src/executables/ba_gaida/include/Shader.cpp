//
// Created by Dradozer on 08.04.2019.
//

#include "Shader.h"

void ba_gaida::Shader::attachShader(GLuint &program, GLenum type, const char *path)
{
    GLuint shader = loadShader(type, path);

    if (glIsProgram(program) == false)
    {
        program = glCreateProgram();
    }
    glAttachShader(program, shader);
}

void ba_gaida::Shader::linkShader(GLuint program)
{
    glLinkProgram(program);
    validateProgram(program);
    glValidateProgram(program);
    validateProgram(program);
}

GLuint ba_gaida::Shader::loadShader(GLenum type, const char *path)
{
    std::string fileContent;
    std::string line;

    std::ifstream file(path);
    if (file.is_open())
    {
        while (!file.eof())
        {
            getline(file, line);
            fileContent += line + "\n";
        }
        file.close();
        std::cout << green << "Successfully loaded: " << path << white << std::endl;
    } else
    {
        std::cout << red << "ERROR: Loading-Error with " << path << white << std::endl;
    }
    const char *source = fileContent.c_str();
    const GLint sourceSize = strlen(source);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, &sourceSize);
    glCompileShader(shader);
    validateShader(shader, path);

    return shader;
}

void ba_gaida::Shader::validateShader(GLuint shader, const char *path)
{
    GLint status = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *strLog = new GLchar[logLength + 1];
        glGetShaderInfoLog(shader, logLength, NULL, strLog);

        std::cout << red << "ERROR: Compile-Error with " << path << white << std::endl;
        std::cout << red << strLog << white << std::endl;

        delete[] strLog;

    } else if (status == GL_TRUE)
    {
        std::cout << green << "Successfully compiled: " << path << white << std::endl;
    }
}

void ba_gaida::Shader::validateProgram(GLuint program)
{
    GLint status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        GLchar *strLog = new GLchar[logLength + 1];
        glGetProgramInfoLog(program, logLength, NULL, strLog);

        std::cout << red << "ERROR: Linking-Error" << white << std::endl;
        std::cout << red << strLog << white << std::endl;

        delete[] strLog;

        deleteShader(program);

        FAIL_PROGRAM //program shuts down
    }
}

void ba_gaida::Shader::deleteShader(GLuint program)
{
    GLint shaderCount = 0;
    glGetProgramiv(program, GL_ATTACHED_SHADERS, &shaderCount);

    if (shaderCount != 0)
    {
        GLuint *shader = new GLuint[shaderCount];
        glGetAttachedShaders(program, shaderCount, NULL, shader);
        for(int i = 0; i < shaderCount; i++)
        {
            deleteShader(shader[i]);
        }
        delete [] shader;
    }
    glGetError();
}
