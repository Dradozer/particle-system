#include "Include/Engine.h"
#include "Include/CameraTrackball.h"
#include "Include/ParticleSystems/CPU/ParticleSystem_v1.h"
#include "Include/ParticleSystems/GPU/ParticleSystem_v1.h"


using ParticleSystem::Engine;
using ParticleSystem::Camera::Camera;
using ParticleSystem::Camera::CameraTrackball;

#define WIDTH 800
#define HEIGHT 600


int main()
{
    //create Engine
    Engine *myEngine = new Engine();

    //create camera
    CameraTrackball *myCamera = new CameraTrackball(glm::vec3(0.0f),
                                                    glm::vec3(0.0f, 1.0f, 0.0f),
                                                    glm::vec3(0.0f, 0.0f, 10.0f),
                                                    WIDTH,
                                                    HEIGHT);

    //basic setup
    myEngine->setWindowSize(WIDTH, HEIGHT);
    myEngine->setWindowTitle("Particle Demo");
    //myEngine->setCamera(myCamera);

    //create particle systems and add them to the engine
    auto * pSystemCPU_1 = new ParticleSystem::CPU::ParticleSystem_v1();
    pSystemCPU_1->addCamera(myCamera);
    pSystemCPU_1->activateCamera(myCamera);
    myEngine->addParticleSystem(pSystemCPU_1);


    //auto * pSystemGPU_1 = new ParticleSystem::GPU::ParticleSystem_v1();
    //myEngine->addParticleSystem(pSystemGPU_1);

    myEngine->loop();

    delete myEngine;
    delete myCamera;

    return 0;
}