//
// Created by Dradozer on 03.04.2019.
//

#include "ParticleSystem.h"

ba_gaida::ParticleSystem::ParticleSystem(GLFWwindow *window, const int particleCount, const int WIDTH,
                                         const int HEIGTH, const glm::uvec3 boxSize)
{
    m_running = false;
    m_reset = false;
    m_gravity = true;
    m_imguiUi = true;

    m_window = window;

    m_timeMultiplyer = 1.f;
    m_buoyCoeff = 1.f;
    m_temperature = 10.f;
    m_heatFlow = -8.f;
    // values have to be negative for buoyancy
    // air 0.025
    // water 0.6089
    // concrete 0.92

    m_settings.x = 0.7f;
    m_settings.y = 10.f;
    m_settings.z = 0.2f;
    m_settings.w = 1.f;
    //float mass;
    //float restingDensity;
    //float stiffness;
    //float radius;

    m_externalForce = glm::vec3(0.f,0.f,0.f);

    m_dimensions = glm::ivec4(50);
    m_particleCount = (particleCount * 64);
    m_iterations = ceil(log(m_dimensions.x * m_dimensions.y * m_dimensions.z)/log(2))+1;
    m_step = 0;

    m_width = WIDTH;
    m_heigth = HEIGTH;
    m_boxSize = boxSize;

    m_boxCenter = glm::vec3(m_dimensions.x / 2, m_dimensions.y / 2,m_dimensions.z / 2);
    m_camera = new ba_gaida::Camera(m_window, m_boxCenter,
                                    glm::vec3(0.f, 1.f, 0.f), m_width, m_heigth);
    //create Particle at random Position without Velocity
    m_particle = nullptr;
    m_eulerianGrid = nullptr;
    m_gridBuffer = nullptr;

    init();

    Shader::attachShader(m_renderID, GL_VERTEX_SHADER, SHADERS_PATH "/ba_gaida/vertexShader.glsl");
    Shader::attachShader(m_renderID, GL_FRAGMENT_SHADER, SHADERS_PATH "/ba_gaida/fragmentShader.glsl");
    Shader::linkShader(m_renderID);

    m_uniform_viewM = glGetUniformLocation(m_renderID, "viewMatrix");
    m_uniform_projM = glGetUniformLocation(m_renderID, "projMatrix");
    m_uniform_camPos = glGetUniformLocation(m_renderID, "cameraPos");

    SSBO::createSSBO(m_ssbo_particleId[0], 0, m_particleCount * sizeof(Particle), &m_particle[0]);
    SSBO::createSSBO(m_ssbo_particleId[1], 1, m_particleCount * sizeof(Particle), &m_particle[0]);
    SSBO::createSSBO(m_ssbo_gridId, 2, m_dimensions.x * m_dimensions.y * m_dimensions.z * sizeof(Grid), &m_eulerianGrid[0]);
    SSBO::createSSBO(m_ssbo_gridBufferID, 3, m_dimensions.x * m_dimensions.y * m_dimensions.z * sizeof(int), &m_gridBuffer[0]);

    ComputeShader::createComputeShader(m_resetParticleID[0]        , SHADERS_PATH "/ba_gaida/resetParticleShader.glsl");
    ComputeShader::createComputeShader(m_resetGridID[0]            , SHADERS_PATH "/ba_gaida/resetGridShader.glsl");
    ComputeShader::createComputeShader(m_lableParticleID[0]        , SHADERS_PATH "/ba_gaida/lableParticleShader.glsl");
    ComputeShader::createComputeShader(m_prefixSumInitID[0]        , SHADERS_PATH "/ba_gaida/prefixSumInitShader.glsl");
    ComputeShader::createComputeShader(m_prefixSumCalcID[0]        , SHADERS_PATH "/ba_gaida/prefixSumCalcShader.glsl");
    ComputeShader::createComputeShader(m_prefixSumBufferID[0]      , SHADERS_PATH "/ba_gaida/prefixSumBufferShader.glsl");
    ComputeShader::createComputeShader(m_prefixSumExcludeID[0]     , SHADERS_PATH "/ba_gaida/prefixSumExcludeShader.glsl");
    ComputeShader::createComputeShader(m_rearrangingParticlesID[0] , SHADERS_PATH "/ba_gaida/rearrangingParticlesShader.glsl");
    ComputeShader::createComputeShader(m_densityID[0]              , SHADERS_PATH "/ba_gaida/densityShader.glsl");
    ComputeShader::createComputeShader(m_normalID[0]            , SHADERS_PATH "/ba_gaida/normalShader.glsl");
    ComputeShader::createComputeShader(m_calcForcesID[0]           , SHADERS_PATH "/ba_gaida/calcForcesShader.glsl");
    ComputeShader::createComputeShader(m_swapParticlesID[0]        , SHADERS_PATH "/ba_gaida/swapParticlesShader.glsl");
    ComputeShader::createComputeShader(m_updateForceID[0]          , SHADERS_PATH "/ba_gaida/updateForceShader.glsl");
    ComputeShader::createComputeShader(m_collisionID[0]            , SHADERS_PATH "/ba_gaida/collisionShader.glsl");

    setUniform(m_resetParticleID);
    setUniform(m_resetGridID);
    setUniform(m_lableParticleID);
    setUniformPrefix(m_prefixSumInitID);
    setUniformPrefix(m_prefixSumCalcID);
    setUniformPrefix(m_prefixSumBufferID);
    setUniformPrefix(m_prefixSumExcludeID);
    setUniform(m_rearrangingParticlesID);
    setUniformParticles(m_densityID);
    setUniformParticles(m_normalID);
    setUniformParticles(m_calcForcesID);
    setUniform(m_swapParticlesID);
    setUniform(m_updateForceID);
    setUniform(m_collisionID);

    initShader();

    // init imgui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    // set glsl version | default: 150
    const char *glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);
    m_imgui_once = false; //pos and resize just once, look usage
    m_imgui_applications = 0.f;
    m_fps = new ba_gaida::FpsCounter(m_window, 10);
}

ba_gaida::ParticleSystem::~ParticleSystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete m_particle;
    delete m_eulerianGrid;
    delete m_camera;
    delete m_fps;
    // remember to add all programID!
    Shader::deleteShader(m_resetParticleID[0]);
    Shader::deleteShader(m_resetGridID[0]);
    Shader::deleteShader(m_lableParticleID[0]);
    Shader::deleteShader(m_prefixSumInitID[0]);
    Shader::deleteShader(m_prefixSumCalcID[0]);
    Shader::deleteShader(m_prefixSumExcludeID[0]);
    Shader::deleteShader(m_prefixSumBufferID[0]);
    Shader::deleteShader(m_rearrangingParticlesID[0]);;
    Shader::deleteShader(m_densityID[0]);
    Shader::deleteShader(m_normalID[0]);
    Shader::deleteShader(m_calcForcesID[0]);
    Shader::deleteShader(m_swapParticlesID[0]);
    Shader::deleteShader(m_updateForceID[0]);
    Shader::deleteShader(m_collisionID[0]);
    Shader::deleteShader(m_renderID);
    //delete BufferObjects
    for (int i = 0; i < 2; i++)
    {
        glDeleteBuffers(1, &m_ssbo_particleId[i]);
    }
    glDeleteBuffers(1, &m_ssbo_gridId);
    glDeleteBuffers(1, &m_ssbo_gridId);
}

void ba_gaida::ParticleSystem::update(double deltaTime)
{
    if(m_reset == true){
        ComputeShader::updateComputeShaderP64(m_resetParticleID, m_particleCount);
        m_reset = m_running = false;
        m_externalForce = glm::vec3(0.f);
    }
    deltaTime = deltaTime * m_timeMultiplyer * m_running * 0.5;
    m_fps->resetTimestamp();

    if(!(ImGui::IsAnyWindowFocused()||ImGui::IsAnyWindowHovered())){
        m_camera->m_hovered = false;
    }
    m_camera->update();
    m_fps->setTimestamp(0);

    ComputeShader::updateComputeShaderD(m_resetGridID, m_dimensions);
    m_fps->setTimestamp(1);

    ComputeShader::updateComputeShaderP64(m_lableParticleID, m_particleCount);
    m_fps->setTimestamp(2);
    ComputeShader::updateComputeShaderD(m_prefixSumInitID, m_dimensions);
    m_step = 1;
    for(int i = 1; i < m_iterations; i++){
        ComputeShader::updateComputeShaderPrefix(m_prefixSumCalcID, m_dimensions,m_step);
        ComputeShader::updateComputeShaderD(m_prefixSumBufferID, m_dimensions);
        m_step *= 2;
    }
//    ComputeShader::updateComputeShaderD(m_prefixSumExcludeID, m_dimensions);
    m_fps->setTimestamp(3);

    ComputeShader::updateComputeShaderP64(m_rearrangingParticlesID, m_particleCount);
    m_fps->setTimestamp(4);

    m_Forces = glm::vec4(glm::vec3(m_gravityV4.x * m_gravity,m_gravityV4.y * m_gravity,m_gravityV4.z * m_gravity)+ m_externalForce,0.f);

    ComputeShader::updateComputeShaderParticle(m_densityID, deltaTime, m_particleCount, m_settings, m_Forces, m_buoyCoeff, m_heatFlow);
    ComputeShader::updateComputeShaderParticle(m_normalID, deltaTime, m_particleCount, m_settings , m_Forces, m_buoyCoeff, m_heatFlow);

    m_fps->setTimestamp(5);

    ComputeShader::updateComputeShaderParticle(m_calcForcesID, deltaTime, m_particleCount, m_settings, m_Forces, m_buoyCoeff, m_heatFlow);

    m_fps->setTimestamp(6);

    ComputeShader::updateComputeShaderP64(m_swapParticlesID, m_particleCount);

    ComputeShader::updateComputeShaderP64DT(m_updateForceID,deltaTime ,m_particleCount);

    m_fps->setTimestamp(7);

    ComputeShader::updateComputeShaderP64DT(m_collisionID, deltaTime,m_particleCount);
    m_fps->setTimestamp(8);

    render();
    m_fps->setTimestamp(9);

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
if(m_imguiUi == true){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    { //imgui window
        ImGui::Begin("Debug | ParticleSystem");
        m_imgui_applications = 1.17f;

        if (m_imgui_once != true)
        {
            ImGui::SetWindowPos(ImVec2(0.f, 0.f), 0); // x, y, condition
            m_imgui_once = true;
        }

        ImGui::Text("Particles: %.i\n", m_particleCount);
        ImGui::Text("Gridsize: %.ix%.ix%.i \n", m_dimensions.x, m_dimensions.y, m_dimensions.z);
        bool run;
        if (m_running != true)
        {
            run = ImGui::Button("Play");
        } else
        {
            run = ImGui::Button("Stop");
        }
        if (run == true && m_running == true)
        {
            m_running = false;
        } else if (run == true && m_running != true)
        {
            m_running = true;
        }

        ImGui::SameLine(300.f, -1);
        m_reset = ImGui::Button("Reset");

        if (ImGui::CollapsingHeader("Settings"))
        {
            m_imgui_applications = m_imgui_applications + 1.95f;
            ImGui::SliderFloat("Time", &m_timeMultiplyer, 0.01f, 2.f);
            ImGui::Text("ParticleSettings");
            ImGui::SliderFloat("Mass", &m_settings.x, 0.1f, 2.f);
            ImGui::SliderFloat("BuoyCoeff", &m_buoyCoeff, 1.f, 20.f);
            ImGui::SliderFloat("HeatFlow", &m_heatFlow, -10.f, 10.f);
            ImGui::SliderFloat("RestDensity", &m_settings.y, -20.f, 20.f);
            ImGui::SliderFloat("Stiffness", &m_settings.z, -1.f, 10.f);
            ImGui::SliderFloat("Radius", &m_settings.w, 0.5f, 5.f);
            ImGui::Text("Gravity: %.4f\n", m_gravity * m_gravityV4.y);
            ImGui::SameLine(125, -1.f);
            ImGui::Checkbox("", &m_gravity);
            ImGui::Text("ExternalForces:\n");
            ImGui::SliderFloat("X", &m_externalForce.x, -20.f, 20.f);
            ImGui::SliderFloat("Y", &m_externalForce.y, -20.f, 20.f);
            ImGui::SliderFloat("Z", &m_externalForce.z, -20.f, 20.f);
        }

        if (ImGui::CollapsingHeader("Controls"))
        {
            m_imgui_applications = m_imgui_applications + 0.9f;
            ImGui::Text("Controls:\n"
                        "LeftMouseButton: moves viewport\n"
                        "W: moves to view-direction\n"
                        "S: moves away from view-direction\n"
                        "F: moves down in world-space\n"
                        "V: moves up in world-space\n"
                        "H: Hide UI\n"
                        "Spacebar: Start/Stop\n"
                        "R: Reset\n"
                        "ScrollWheel: zoom to center\n");
        }

        if (ImGui::CollapsingHeader("Computingtimes"))
        {
            m_imgui_applications = m_imgui_applications + 1.1f;
            ImGui::Text("Avg. computingtime for segment:\n"
                        "CameraUpdate: \t%.8f ms\n"
                        "CS ResetGrid: \t%.8f ms\n"
                        "CS Label:     \t%.8f ms\n"
                        "CS PrefixSum: \t%.8f ms\n"
                        "CS rearr.Prt: \t%.8f ms\n"
                        "CS preForces:   \t%.8f ms\n"
                        "CS Forces:  \t%.8f ms\n"
                        "CS Swapping:  \t%.8f ms\n"
                        "CS Collision: \t%.8f ms\n"
                        "Renderer:     \t%.8f ms\n",
                        m_fps->getTimestamp(0) * 1000, m_fps->getTimestamp(1) * 1000, m_fps->getTimestamp(2) * 1000,
                        m_fps->getTimestamp(3) * 1000, m_fps->getTimestamp(4) * 1000, m_fps->getTimestamp(5) * 1000,
                        m_fps->getTimestamp(6) * 1000, m_fps->getTimestamp(7) * 1000, m_fps->getTimestamp(8) * 1000,
                        m_fps->getTimestamp(9) * 1000);
            ImGui::Text("-----------------------------------------------");
        }

        ImGui::SetWindowSize(ImVec2(350, 150 * m_imgui_applications), 0);
        if (m_running == true)
        {
            ImGui::Text("Application average %.4f ms/frame (%.i FPS)", 1000.f / m_fps->getFPS(),
                        m_fps->getFPS());
        }
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
    glfwSwapBuffers(m_window);
}

void ba_gaida::ParticleSystem::init()
{
    initParticle();
    initGrid();
}

void ba_gaida::ParticleSystem::initParticle()
{
    //init Particleposition
    std::uniform_real_distribution<float> pos_x(m_dimensions.x/2 - m_boxSize.x,m_dimensions.x/2 + m_boxSize.x);
    std::uniform_real_distribution<float> pos_y(m_dimensions.y/10 - m_boxSize.y,m_dimensions.y/10 + m_boxSize.y);
    std::uniform_real_distribution<float> pos_z(m_dimensions.z/2 - m_boxSize.z,m_dimensions.z/2 + m_boxSize.z);
//    std::uniform_real_distribution<float> vel_x(-10.f,10.f);
//    std::uniform_real_distribution<float> vel_y(-10.f,10.f);
//    std::uniform_real_distribution<float> vel_z(-10.f,10.f);
    std::uniform_real_distribution<float> vel_x(-2.f,2.f);
    std::uniform_real_distribution<float> vel_y(-5.f,5.f);
    std::uniform_real_distribution<float> vel_z(-2.f,2.f);
    std::default_random_engine rdm;

    m_particle = new Particle[m_particleCount];

    for (int i = 0; i < m_particleCount; i++)
    {
        m_particle[i].position = glm::vec4(pos_x(rdm), pos_y(rdm), pos_z(rdm), 0.f);
        while(glm::distance(glm::vec3(m_particle[i].position.x,m_particle[i].position.y,m_particle[i].position.z),glm::vec3(m_dimensions.x/2,m_dimensions.y/10,m_dimensions.z/2 ))> 1.f)
        {
            m_particle[i].position = glm::vec4(pos_x(rdm), pos_y(rdm), pos_z(rdm), 0.f);
        }
        m_particle[i].startPosition = glm::vec4(m_particle[i].position.x, m_particle[i].position.y, m_particle[i].position.z, m_temperature);


//        m_particle[i].velocity = glm::vec4(vel_x(rdm), vel_y(rdm), vel_z(rdm), 0.f);
        m_particle[i].velocity = glm::vec4(0.f);

        m_particle[i].temperature = m_temperature;

        m_particle[i].vorticity = glm::vec4(0.f);

        if (i == m_particleCount - 1)
        {
            std::cout << "Successfully generated: \t" << m_particleCount << " Particle" << std::endl;
        }
    }
}

void ba_gaida::ParticleSystem::initGrid()
{
    int i = 0;
    m_gridBuffer = new int[m_dimensions.x * m_dimensions.y * m_dimensions.z];
    m_eulerianGrid = new Grid[m_dimensions.x * m_dimensions.y * m_dimensions.z];
    std::cout << "Successfully generated: \t" << m_dimensions.x << " x " << m_dimensions.y << " x"  << m_dimensions.z << " Grid" << std::endl;
    std::cout << "Running with " << m_iterations << " Itterations per Frame" << std::endl;
    for (int x = 0; x < m_dimensions.x; x++)
    {
        for(int y = 0; y < m_dimensions.y; y++)
        {
            for(int z = 0; z < m_dimensions.z; z++)
            {
                m_eulerianGrid[i].id = x * m_dimensions.x * m_dimensions.x + y * m_dimensions.y + z;
                m_eulerianGrid[i].particlescount = 0;
                m_eulerianGrid[i].previousSortOutPut = 0;
                m_eulerianGrid[i].currentSortOutPut = 0;
                m_gridBuffer[i] = 0;
                i++;
            }
        }
    }
}

void ba_gaida::ParticleSystem::initShader(){
    ComputeShader::initComputeShader(m_resetParticleID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_resetGridID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_lableParticleID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_prefixSumInitID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_prefixSumCalcID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_prefixSumExcludeID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_prefixSumBufferID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_rearrangingParticlesID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_densityID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_normalID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_calcForcesID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_swapParticlesID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_updateForceID, m_particleCount, m_dimensions);
    ComputeShader::initComputeShader(m_collisionID, m_particleCount, m_dimensions);
}

void ba_gaida::ParticleSystem::setUniform(GLuint *id)
{
    id[1] = glGetUniformLocation(id[0], "deltaTime");
    id[2] = glGetUniformLocation(id[0], "particleCount");
    id[3] = glGetUniformLocation(id[0], "gridSize");
}

void ba_gaida::ParticleSystem::setUniformPrefix(GLuint *id)
{
    id[1] = glGetUniformLocation(id[0], "deltaTime");
    id[2] = glGetUniformLocation(id[0], "step");
    id[3] = glGetUniformLocation(id[0], "gridSize");
}

void ba_gaida::ParticleSystem::setUniformParticles(GLuint *id)
{
    id[1] = glGetUniformLocation(id[0], "deltaTime");
    id[2] = glGetUniformLocation(id[0], "particleCount");
    id[3] = glGetUniformLocation(id[0], "gridSize");
    id[4] = glGetUniformLocation(id[0], "particleSettings");
    id[5] = glGetUniformLocation(id[0], "externalForce");
    id[6] = glGetUniformLocation(id[0], "buoyCoeff");
    id[7] = glGetUniformLocation(id[0], "heatFlow");
}

void ba_gaida::ParticleSystem::hideUi()
{
    m_imguiUi = !m_imguiUi;
}

void ba_gaida::ParticleSystem::runStop()
{
    m_running = !m_running;
}

void ba_gaida::ParticleSystem::reset()
{
    m_reset = !m_reset;
}