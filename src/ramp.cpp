#include "ramp.h"
#include <iostream>

void Ramp::setup() {
    mNode->setPosition(position);
}

bool Ramp::collision(const Ogre::Vector3& point) const {
    Ogre::Vector3 worldMin = position + minCorner;
    Ogre::Vector3 worldMax = position + maxCorner;

    return (point.x >= worldMin.x && point.x <= worldMax.x &&
        point.y >= worldMin.y && point.y <= worldMax.y &&
        point.z >= worldMin.z && point.z <= worldMax.z);
}

void Ramp::setPosition(const Ogre::Vector3& pos) {
    mNode->setPosition(pos);
    position = pos;
}
bool Ramp::frameStarted(const Ogre::FrameEvent& evt) {
    float dt = evt.timeSinceLastFrame;
    if (collision(player->getPosition())) {
        player->collision(State::Jumped);
    }
    //std::cout << collision(mplayerNode->getPosition()) << std::endl;
    return true;
}