//
// Created by Dradozer on 03.04.2019.
//

#include "ParticleSystem.h"

ba_gaida::ParticleSystem::ParticleSystem(GLFWwindow *window, const int particleCount, const int WIDTH,
                                         const int HEIGTH, const glm::uvec3 boxSize)
{
    m_window = window;
    m_particleCount = particleCount * 128;
    m_width = WIDTH;
    m_heigth = HEIGTH;
    m_Boxsize = boxSize;
    m_boxCenter = glm::vec3(boxSize.x / 2, boxSize.y / 2, boxSize.z / 2);
    m_camera = new ba_gaida::Camera(m_window, m_boxCenter,
                                    glm::vec3(0.f, 1.f, 0.f), m_width, m_heigth);

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
    // init imgui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    // set glsl version | default: 150
    const char *glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    m_imgui_once = false; //pos and resize just once, look usage

    m_imgui_clear_color = ImVec4(135 / 255.f, 206 / 255.f, 235 / 255.f, 0.f);
    for(int i = 0; i <= (sizeof(m_timeStamps)/ sizeof(*m_timeStamps)); i++)
        m_timeStamps[i] = 0.f;
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

    delete m_camera;
#ifdef maxFPS
    delete m_fps;
#endif
    Shader::deleteShader(m_renderID);// remember to add all programID!
    //delete BufferObjects
    for (int i = 0; i < 2; i++)
    {
        glDeleteBuffers(1, &m_ssbo_pos_id[i]);
        glDeleteBuffers(1, &m_ssbo_vel_id[i]);
    }

}
#ifndef maxFPS
template <class T> const T& max (const T& a, const T& b) {
    return (a<b)?b:a;     // or: return comp(a,b)?b:a; for version (2)
}
#endif

void ba_gaida::ParticleSystem::update(const double deltaTime)
{
#ifndef maxFPS
    resetTime();
#endif
    m_camera->update();
#ifndef maxFPS
    m_timeStamps[0] = max(m_timeStamps[0], getTimeStamp());
#endif
    ComputeShader::updateComputeShader(m_externalForceID, deltaTime, m_particleCount);
#ifndef maxFPS
    m_timeStamps[1] = max(m_timeStamps[1], getTimeStamp());
#endif
#ifdef maxFPS
    m_fps->update(deltaTime);
#endif

    render();
#ifndef maxFPS
    m_timeStamps[2] = max(m_timeStamps[2], getTimeStamp());
#endif
}

void ba_gaida::ParticleSystem::render()
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

#ifndef maxFPS //go to PerformanceSettings.h and #define maxFps for better performance(disables imgui and debug, enables simple FpsCounter in title)
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    { //imgui window
        ImGui::Begin("Debug | ParticleSystem");
        if (m_imgui_once != true)
        {
            ImGui::SetWindowPos(ImVec2(0.f, 0.f), 0); // x, y, condition
            m_imgui_once = true;
        }

        ImGui::Text("Warning! This Settings makes the ParticleSystem slower\n");
        ImGui::Text("Change Settings in the PerformanceSettings.h \nand compile again\n");
        ImGui::Text("Running with %.i Particles", m_particleCount);
        if (ImGui::CollapsingHeader("Controls"))
        {
            ImGui::SetWindowSize(ImVec2(400, 300), 0);
            ImGui::Text("Controls:\n"
                        "LeftMouseButton: moves viewport\n"
                        "W: moves to iew-direction\n"
                        "S: moves away from view-direction\n"
                        "Spacebar: moves up in world-space\n"
                        "F: moves down in world-space\n");
            ImGui::Text("-----------------------------------------------\n"
                        "ClearColor: \n");
            ImGui::ColorEdit3("clear color", (float *) &m_imgui_clear_color);
            ImGui::Text("-----------------------------------------------");
        } else if (ImGui::CollapsingHeader("Computingtimes"))
        {
            ImGui::SetWindowSize(ImVec2(400, 300), 0);
            ImGui::Text("Max computingtime for segment:\n"
                        "CameraUpdate: %.4f ms\n"
                        "CS Gravity: %.4f ms\n"
                        "Renderer: %.4f ms\n",
                        m_timeStamps[0] * 1000, m_timeStamps[1] * 1000, m_timeStamps[2] * 1000);
            ImGui::Text("-----------------------------------------------");
        } else
        {
            ImGui::SetWindowSize(ImVec2(400, 170), 0);
        }
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
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
    //init Particleposition
    std::uniform_real_distribution<float> dist_x(0.f, m_Boxsize.x);
    std::uniform_real_distribution<float> dist_y(0.f, m_Boxsize.y);
    std::uniform_real_distribution<float> dist_z(0.f, m_Boxsize.z);
    std::default_random_engine rdm;

    m_particle_pos = new glm::vec4[m_particleCount];
    m_particle_vel = new glm::vec4[m_particleCount];

    for (int i = 0; i < m_particleCount; i++)
    {
        m_particle_pos[i] = glm::vec4(dist_x(rdm), dist_y(rdm), dist_z(rdm), 0.f);
//        std::cout << "Pos[" << i << "]:( " << m_particle_pos[i].x << ", " << m_particle_pos[i].y << ", "
//                  << m_particle_pos[i].z << ")" << std::endl;
        m_particle_vel[i] = glm::vec4(0.f);
        if (i == m_particleCount - 1)
        {
            std::cout << "Successfully generated: \t" << m_particleCount << " Particle" << std::endl;
        }
    }
}

void ba_gaida::ParticleSystem::setUniform(GLuint *id, const int particleCount)
{
    id[1] = glGetUniformLocation(id[0], "deltaTime");
    id[2] = glGetUniformLocation(id[0], "particleCount");

}

#ifndef maxFPS
void ba_gaida::ParticleSystem::resetTime()
{
    m_startTimer = glfwGetTime();
}

float ba_gaida::ParticleSystem::getTimeStamp()
{
    m_usedTime = glfwGetTime() - m_startTimer;
    m_startTimer = glfwGetTime();
    return m_usedTime;
}
#endif