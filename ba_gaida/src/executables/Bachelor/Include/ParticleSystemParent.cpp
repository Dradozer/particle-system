//
// Created by pasab on 27.03.2019.
//

#include "ParticleSystemParent.h"


unsigned int ParticleSystem::ParticleSystemParent::getParticleCount() const {
    return m_particleCount;
}

void ParticleSystem::ParticleSystemParent::addCamera(Camera::Camera* camera)
{
    //check if the given Camera is already in our list
    auto c = std::find(m_cameras.begin(), m_cameras.end(), camera);

    //if not the insert it!
    if (c == m_cameras.end())
        m_cameras.push_back(camera);
}

void ParticleSystem::ParticleSystemParent::activateCamera(const unsigned int id)
{
    //only change camera if we are in our array boundarys
    if (id < m_cameras.size())
        m_currentCamera = id;
}

void ParticleSystem::ParticleSystemParent::activateCamera(const ParticleSystem::Camera::Camera *camera)
{
    //check if the given Camera is already in our list
    auto c = std::find(m_cameras.begin(), m_cameras.end(), camera);

    //if found then get the index if it
    if (c != m_cameras.end()) {
        unsigned int idx = std::distance(m_cameras.begin(), c);
        if (idx < m_cameras.size())
            m_currentCamera = idx;
    }
}

ParticleSystem::Camera::Camera* ParticleSystem::ParticleSystemParent::getActiveCamera()
{
    return m_cameras[m_currentCamera];
}