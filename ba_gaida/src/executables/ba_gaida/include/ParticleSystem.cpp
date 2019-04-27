//
// Created by Dradozer on 03.04.2019.
//

#include "ParticleSystem.h"

ba_gaida::ParticleSystem::ParticleSystem(GLFWwindow *window, const int particleCount, const int WIDTH,
                                         const int HEIGTH, const glm::uvec3 boxSize)
{
    m_window = window;
    m_particleCount = particleCount * 128;
    m_Boxsize = boxSize;
    m_boxCenter = glm::vec3(boxSize.x / 2, boxSize.y / 2, boxSize.z / 2);
    m_camera = new ba_gaida::Camera(m_boxCenter,
                                    glm::vec3(0.0f, 1.0f, 0.0f), WIDTH, HEIGTH);

    //create Particle at random Position without Velocity
    m_particle_pos = NULL;
    m_particle_vel = NULL;
    init();

    Shader::attachShader(m_renderID, GL_VERTEX_SHADER, SHADERS_PATH "/ba_gaida/vertexShader.glsl");
    Shader::attachShader(m_renderID, GL_FRAGMENT_SHADER, SHADERS_PATH "/ba_gaida/fragmentShader.glsl");
    Shader::linkShader(m_renderID);

    m_uniform_viewM = glGetUniformLocation(m_renderID, "viewMatrix");
    m_uniform_projM = glGetUniformLocation(m_renderID, "projMatrix");
    m_uniform_camPos = glGetUniformLocation(m_renderID, "cameraPos");

    SSBO::createSSBO(m_ssbo_pos_id[0], 0, m_particleCount * sizeof(glm::vec4), &m_particle_pos[0]);
    SSBO::createSSBO(m_ssbo_vel_id[0], 1, m_particleCount * sizeof(glm::vec4), &m_particle_vel[0]);

    ComputeShader::createComputeShader(m_externalForceID[0], SHADERS_PATH "/ba_gaida/externalForcesComputeShader.glsl");

    setUniform(m_externalForceID, m_particleCount);

#ifndef maxFPS
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    m_clear_color = ImVec4(135 / 255.f, 206 / 255.f, 235 / 255.f, 0.f);
#else
    m_fps = new ba_gaida::FpsCounter(window);
#endif
}

ba_gaida::ParticleSystem::~ParticleSystem()
{
#ifndef maxFPS
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif


    delete m_camera;
    delete m_fps;
    Shader::deleteShader(m_renderID);// remember to add all programID!
    //delete BufferObjects
    for (int i = 0; i < 2; i++)
    {
        glDeleteBuffers(1, &m_ssbo_pos_id[i]);
        glDeleteBuffers(1, &m_ssbo_vel_id[i]);
    }

}

void ba_gaida::ParticleSystem::update(const double deltaTime)
{
    ComputeShader::updateComputeShader(m_externalForceID, deltaTime, m_particleCount);

    m_camera->update(m_window);
#ifdef maxFPS
    m_fps->update(deltaTime);
#endif
}

void ba_gaida::ParticleSystem::render(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_renderID);

    glUniform3fv(m_uniform_camPos, 1, glm::value_ptr(m_camera->getCameraPos()));
    glUniformMatrix4fv(m_uniform_viewM, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
    glUniformMatrix4fv(m_uniform_projM, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));

    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_pos_id[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_vel_id[0]);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, m_particleCount);

    glUseProgram(0);

#ifndef maxFPS
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    bool show_demo_window = false;
    { //imgui window
        static float f = 0.0f;

        ImGui::Begin("Debug| ParticleSystem");

        ImGui::Text("Warning! This makes the ParticleSystem slower");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&m_clear_color); // Edit 3 floats representing a color


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, m_clear_color.w);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

    glfwSwapBuffers(window);

}

void ba_gaida::ParticleSystem::init()
{
    //init Particleposition
    std::uniform_real_distribution<float> dist_x(0.0f, m_Boxsize.x);
    std::uniform_real_distribution<float> dist_y(0.0f, m_Boxsize.y);
    std::uniform_real_distribution<float> dist_z(0.0f, m_Boxsize.z);
    std::default_random_engine rdm;

    m_particle_pos = new glm::vec4[m_particleCount];
    m_particle_vel = new glm::vec4[m_particleCount];

    for (int i = 0; i < m_particleCount; i++)
    {
        m_particle_pos[i] = glm::vec4(dist_x(rdm), dist_y(rdm), dist_z(rdm), 0.0f);
//        std::cout << "Pos[" << i << "]:( " << m_particle_pos[i].x << ", " << m_particle_pos[i].y << ", "
//                  << m_particle_pos[i].z << ")" << std::endl;
        m_particle_vel[i] = glm::vec4(0.0f);
        if (i == m_particleCount - 1)
        {
            std::cout << "Successfully generated: \t" << m_particleCount << " Particle" << std::endl;
        }
    }
}

void ba_gaida::ParticleSystem::setUniform(GLuint *id, const int particleCount)
{
    id[1]= glGetUniformLocation(id[0], "deltaTime");
    id[2]= glGetUniformLocation(id[0], "particleCount");
}


