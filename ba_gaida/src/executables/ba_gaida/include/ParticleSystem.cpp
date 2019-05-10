//
// Created by Dradozer on 03.04.2019.
//

#include "ParticleSystem.h"

ba_gaida::ParticleSystem::ParticleSystem(GLFWwindow *window, const int particleCount, const int WIDTH,
                                         const int HEIGTH, const glm::uvec3 boxSize)
{
    m_dimensions = 10;
    m_window = window;
    m_particleCount = (particleCount * 32);
    m_width = WIDTH;
    m_heigth = HEIGTH;
    m_Boxsize = boxSize;
    m_boxCenter = glm::vec3(boxSize.x / 2, boxSize.y / 2, boxSize.z / 2);
    m_camera = new ba_gaida::Camera(m_window, m_boxCenter,
                                    glm::vec3(0.f, 1.f, 0.f), m_width, m_heigth);

    //create Particle at random Position without Velocity
    m_particle = nullptr;
    m_eulerianGrid = nullptr;
    init();

    Shader::attachShader(m_renderID, GL_VERTEX_SHADER, SHADERS_PATH "/ba_gaida/vertexShader.glsl");
    Shader::attachShader(m_renderID, GL_FRAGMENT_SHADER, SHADERS_PATH "/ba_gaida/fragmentShader.glsl");
    Shader::linkShader(m_renderID);

    m_uniform_viewM = glGetUniformLocation(m_renderID, "viewMatrix");
    m_uniform_projM = glGetUniformLocation(m_renderID, "projMatrix");
    m_uniform_camPos = glGetUniformLocation(m_renderID, "cameraPos");

    SSBO::createSSBO(m_ssbo_particleId[0], 0, m_particleCount * sizeof(Particle), &m_particle[0]);
    SSBO::createSSBO(m_ssbo_particleId[1], 1, m_particleCount * sizeof(Particle), &m_particle[0]);
    SSBO::createSSBO(m_ssbo_gridId, 2, 1000 * sizeof(Grid), &m_eulerianGrid[0]);

    ComputeShader::createComputeShader(m_externalForceID[0], SHADERS_PATH "/ba_gaida/externalForcesComputeShader.glsl");
    ComputeShader::createComputeShader(m_updateForceID[0], SHADERS_PATH "/ba_gaida/updateForces.glsl");

    setUniform(m_externalForceID);
    setUniform(m_updateForceID);

#ifndef maxFPS
    // init imgui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    // set glsl version | default: 150
    const char *glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    m_imgui_once = false; //pos and resize just once, look usage
    m_imgui_applications = 0.f;
    m_imgui_clear_color = ImVec4(135 / 255.f, 206 / 255.f, 235 / 255.f, 0.f);
    m_fps = new ba_gaida::FpsCounter(m_window, 3);
#else
    m_fps = new ba_gaida::FpsCounter(m_window);
#endif
}

ba_gaida::ParticleSystem::~ParticleSystem()
{
#ifndef maxFPS
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif
    delete m_particle;
    delete m_eulerianGrid;
    delete m_camera;
    delete m_fps;
    Shader::deleteShader(m_renderID);// remember to add all programID!
    Shader::deleteShader(m_externalForceID[0]);
    Shader::deleteShader(m_updateForceID[0]);
    //delete BufferObjects
    for (int i = 0; i < 2; i++)
    {
        glDeleteBuffers(1, &m_ssbo_particleId[i]);
    }
    glDeleteBuffers(1, &m_ssbo_gridId);
}

void ba_gaida::ParticleSystem::update(const double deltaTime)
{
#ifndef maxFPS
    m_fps->resetTimestamp();
#endif
    m_camera->update();
#ifndef maxFPS
    m_fps->setTimestamp(0);
#endif
    ComputeShader::updateComputeShader(m_externalForceID, deltaTime, m_particleCount, m_dimensions);
    ComputeShader::updateComputeShader(m_updateForceID, deltaTime, m_particleCount, m_dimensions);
#ifndef maxFPS
    m_fps->setTimestamp(1);
#endif
    render();
#ifndef maxFPS
    m_fps->setTimestamp(2);
#endif
    m_fps->update(deltaTime);
}

void ba_gaida::ParticleSystem::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_renderID);

    glUniform3fv(m_uniform_camPos, 1, glm::value_ptr(m_camera->getCameraPos()));
    glUniformMatrix4fv(m_uniform_viewM, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
    glUniformMatrix4fv(m_uniform_projM, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));

    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_particleId[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, position));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_ssbo_particleId[0]);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, velocity));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_POINTS, 0, m_particleCount);

    glUseProgram(0);
#ifndef maxFPS //go to PerformanceSettings.h and #define maxFps for better performance(disables imgui and debug, enables simple FpsCounter in title)
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    { //imgui window
        ImGui::Begin("Debug | ParticleSystem");
        m_imgui_applications = 0;
        if (m_imgui_once != true)
        {
            ImGui::SetWindowPos(ImVec2(0.f, 0.f), 0); // x, y, condition
            m_imgui_once = true;
        }
        ImGui::Text("Warning! This Settings makes the ParticleSystem slower\n");
        ImGui::Text("This UI-Settings consume about 0.1 - 0.2 ms\n");
        ImGui::Text("Running with %.i Particles", m_particleCount);
        if (ImGui::CollapsingHeader("Controls"))
        {
            m_imgui_applications = m_imgui_applications + 0.8f;
            ImGui::Text("Controls:\n"
                        "LeftMouseButton: moves viewport\n"
                        "W: moves to iew-direction\n"
                        "S: moves away from view-direction\n"
                        "Spacebar: moves up in world-space\n"
                        "F: moves down in world-space\n");
            ImGui::Text("-----------------------------------------------\n"
                        "ClearColor: \n");
            ImGui::ColorEdit3("clear color", (float *) &m_imgui_clear_color);
        }
        if (ImGui::CollapsingHeader("Computingtimes"))
        {
            m_imgui_applications = m_imgui_applications + 0.5f;
            ImGui::Text("Avg. computingtime for segmenta:\n"
                        "CameraUpdate: \t%.8f ms\n"
                        "CS Gravity:   \t%.8f ms\n"
                        "Renderer:     \t%.8f ms\n",
                        m_fps->getTimestamp(0) * 1000, m_fps->getTimestamp(1) * 1000, m_fps->getTimestamp(2) * 1000);
            ImGui::Text("-----------------------------------------------");
        }

        ImGui::SetWindowSize(ImVec2(400, 170 + 150 * m_imgui_applications), 0);
        ImGui::Text("Application average %.4f ms/frame (%.i FPS)", 1000.f / m_fps->getFPS(),
                    m_fps->getFPS());
        ImGui::End();
    }
    glClearColor(m_imgui_clear_color.x, m_imgui_clear_color.y, m_imgui_clear_color.z, m_imgui_clear_color.w);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif
    glfwSwapBuffers(m_window);
}

void ba_gaida::ParticleSystem::init()
{
    initParticle();
    initGrid(m_dimensions);
}

void ba_gaida::ParticleSystem::initParticle()
{
    //init Particleposition
    std::uniform_real_distribution<float> dist_x(0.f, m_Boxsize.x);
    std::uniform_real_distribution<float> dist_y(0.f, m_Boxsize.y);
    std::uniform_real_distribution<float> dist_z(0.f, m_Boxsize.z);
    std::default_random_engine rdm;

    m_particle = new Particle[m_particleCount];

    for (int i = 0; i < m_particleCount; i++)
    {
        m_particle[i].position = glm::vec4(dist_x(rdm), dist_y(rdm), dist_z(rdm), 0.f);
        m_particle[i].velocity = glm::vec4(0.f, 0.f, 0.f, 0.f);
        if (i == m_particleCount - 1)
        {
            std::cout << "Successfully generated: \t" << m_particleCount << " Particle" << std::endl;
        }
    }
}

void ba_gaida::ParticleSystem::initGrid(const unsigned int dimensions)
{
    int i = 0;
    m_eulerianGrid = new Grid[dimensions * dimensions * dimensions];
    for (int x = 0; x < dimensions; x++)
    {
        for(int y = 0; y < dimensions; y++)
        {
            for(int z = 0; z < dimensions; z++)
            {
                m_eulerianGrid[i].id = x * dimensions * dimensions + y * dimensions + z;
                m_eulerianGrid[i].particlescount = 2;
                i++;
            }
        }
    }
}

void ba_gaida::ParticleSystem::setUniform(GLuint *id)
{
    id[1] = glGetUniformLocation(id[0], "deltaTime");
    id[2] = glGetUniformLocation(id[0], "particleCount");
    id[3] = glGetUniformLocation(id[0], "gridSize");
}