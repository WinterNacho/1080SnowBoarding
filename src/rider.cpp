#include "rider.h"
#include <iostream>
#include "utils.h"

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
    if ((evt.keysym.sym == OgreBites::SDLK_LEFT or evt.keysym.sym == 'a') and direction == Direction::Left)
        direction = Direction::None;

    if ((evt.keysym.sym == OgreBites::SDLK_RIGHT or evt.keysym.sym == 'd') and direction == Direction::Right)
        direction = Direction::None;

    if ((evt.keysym.sym == OgreBites::SDLK_UP or evt.keysym.sym == 'w') and direction == Direction::Up)
        direction = Direction::None;

    if ((evt.keysym.sym == OgreBites::SDLK_DOWN or evt.keysym.sym == 's') and direction == Direction::Down)
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

Ogre::Vector3 Rider::getPosition() {
    return riderPos;
}

void Rider::collision(State st) {
    if (st == State::Stunned) {}
    else if (st== State::Jumped){
        state = st;

    }
}


bool Rider::axisMoved(const OgreBites::AxisEvent& evt) {
    unsigned int axisKey = static_cast<unsigned int>(evt.axis);

    // only checking one specific axis
    if (axisKey != 0)
        return true;

    if (evt.value < -10000)
        direction = Direction::Left;

    else if (evt.value > 10000)
        direction = Direction::Right;

    else
        direction = Direction::None;

    return true;
}

bool Rider::frameStarted(const Ogre::FrameEvent& evt) {
    float dt = evt.timeSinceLastFrame;
    //std::cout << "Player: " << riderPos.x << " " << riderPos.y << " " << riderPos.z << std::endl;
    if (state == State::Stunned) {
        direction = Direction::None;
        zSpeed = 0.0;
    }
    else if (state == State::Jumped) {
        zSpeed = 45.0;
        if(altura(riderPos.x, riderPos.z) >= riderPos.y){
            state = State::Normal;
        }
        else
            setPosition(Ogre::Vector3(riderPos.x, riderPos.y - 0.1 * dt, riderPos.z + zSpeed * dt));
    }
    else if (state==State::Normal) {
        zSpeed = 30.0;
        setPosition(Ogre::Vector3(riderPos.x, altura(riderPos.x, riderPos.z + zSpeed * dt), riderPos.z + zSpeed * dt));
        switch (direction)
        {
        case Direction::Left:
            setPosition(Ogre::Vector3(std::min(20.5f, riderPos.x + xSpeed * dt), riderPos.y, riderPos.z));
            break;
        case Direction::Right:
            setPosition(Ogre::Vector3(std::max(0.5f, riderPos.x - xSpeed * dt), riderPos.y, riderPos.z));
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
    }
    
    return true;
}