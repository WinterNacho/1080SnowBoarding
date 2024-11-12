#include "tree.h"
#include <iostream>

void Tree::setup() {
    mNode->setPosition(position);
}

bool Tree::collision(const Ogre::Vector3& point) const {
    Ogre::Vector3 worldMin = position + minCorner;
    Ogre::Vector3 worldMax = position + maxCorner;

    return (point.x >= worldMin.x && point.x <= worldMax.x &&
        point.y >= worldMin.y && point.y <= worldMax.y &&
        point.z >= worldMin.z && point.z <= worldMax.z);
}

void Tree::setPosition(const Ogre::Vector3& pos) {
    mNode->setPosition(pos);
    position = pos;
}
bool Tree::frameStarted(const Ogre::FrameEvent& evt) {
    float dt = evt.timeSinceLastFrame;
    if (collision(player->getPosition())) {
        player->collision(State::Stunned);
    }
    return true;
}