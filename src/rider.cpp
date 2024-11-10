#include "rider.h"
#include <iostream>

bool Rider::keyPressed(const OgreBites::KeyboardEvent& evt) {
    if (evt.keysym.sym == OgreBites::SDLK_LEFT or evt.keysym.sym == 'a')
        direction = Direction::Left;

    if (evt.keysym.sym == OgreBites::SDLK_RIGHT or evt.keysym.sym == 'd')
        direction = Direction::Right;

    if (evt.keysym.sym == OgreBites::SDLK_UP or evt.keysym.sym == 'w')
        direction = Direction::Up;

    if (evt.keysym.sym == OgreBites::SDLK_DOWN or evt.keysym.sym == 's')
        direction = Direction::Down;

    return true;
}

bool Rider::keyReleased(const OgreBites::KeyboardEvent& evt) {
    //if ((evt.keysym.sym == OgreBites::SDLK_LEFT or evt.keysym.sym == 'a') and direction == Direction::Left)
    if (evt.keysym.sym == 'a' and direction == Direction::Left)
        direction = Direction::None;

    //if ((evt.keysym.sym == OgreBites::SDLK_RIGHT or evt.keysym.sym == 'd') and direction == Direction::Right)
    if (evt.keysym.sym == 'd' and direction == Direction::Right)
        direction = Direction::None;
    if (evt.keysym.sym == 'w' and direction == Direction::Up)
        direction = Direction::None;

    if (evt.keysym.sym == 's' and direction == Direction::Down)
        direction = Direction::None;

    return true;
}

void Rider::setup() {
	
}
void Rider::setPosition(const Ogre::Vector3& pos) {
	mNode->setPosition(pos);
    riderPos = pos;
    mCamNode->setPosition(riderPos + camDisplacement);
}
bool Rider::isColliding(const Ogre::Vector3& entityPos) { return false; }


bool Rider::axisMoved(const OgreBites::AxisEvent& evt) {
    unsigned int axisKey = static_cast<unsigned int>(evt.axis);
    // Control del eje horizontal (0) y vertical (1)
    if (axisKey == 0) {
        if (evt.value < -10000)
            direction = Direction::Left;
        else if (evt.value > 10000)
            direction = Direction::Right;
        else
            direction = Direction::None;  // Neutral en el eje horizontal
    }
    else if (axisKey == 1) { // Manejo del eje vertical
        if (evt.value < -10000)
            direction = Direction::Up;
        else if (evt.value > 10000)
            direction = Direction::Down;
        else
            direction = Direction::None;  // Neutral en el eje vertical
    }


    return true;
}

bool Rider::frameStarted(const Ogre::FrameEvent& evt) {
    float dt = evt.timeSinceLastFrame;
    std::cout << "Player: " << riderPos.x << " " << riderPos.y << " " << riderPos.z << std::endl;
    std::cout << "Cam :   " << mCamNode->getPosition().x << " " << mCamNode->getPosition().y << " " << mCamNode->getPosition().z << std::endl;
    setPosition(Ogre::Vector3(riderPos.x, riderPos.y, riderPos.z + gravity * dt));
    switch (direction)
    {
    case Direction::Left:
        setPosition(Ogre::Vector3(riderPos.x + xSpeed * dt, riderPos.y, riderPos.z));
        break;
    case Direction::Right:
        setPosition(Ogre::Vector3(riderPos.x - xSpeed * dt, riderPos.y, riderPos.z));
        break;
    case Direction::Up:
        setPosition(Ogre::Vector3(riderPos.x, riderPos.y, riderPos.z + xSpeed * dt));
        break;
    case Direction::Down:
        setPosition(Ogre::Vector3(riderPos.x, riderPos.y, riderPos.z - xSpeed * dt));
        break;
    case Direction::None:
    default:
        break;
    }
    return true;
}