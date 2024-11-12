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
    switch (st)
    {
    case State::Stunned:
        break;
    case State::Jumped:
        ySpeed = 35.0;
        break;
    }
    state = st;
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

    switch (state)
    {
    case State::Stunned:
        zSpeed = 0.0;
        break;

    case State::Jumped:
        zSpeed =80.0;
        mNode->pitch(Ogre::Radian(0.22));
        mPitch += 0.22;
        if (altura(riderPos.x, riderPos.z) >= riderPos.y) {
            state = State::Normal;
            mNode->pitch(Ogre::Radian(-mPitch));
        }
        break;
    case State::Normal:
        zSpeed = 30.0;
        switch (direction)
        {
        case Direction::Left:
            riderPos.x = std::min(20.5f, riderPos.x + xSpeed * dt);
            mCamNode->roll(Ogre::Degree(0.2));
            break;
        case Direction::Right:
            riderPos.x = std::max(0.5f, riderPos.x - xSpeed * dt);
            mCamNode->roll(Ogre::Degree(-0.2));
            break;
        case Direction::Up:
            riderPos.z = riderPos.z + xSpeed * dt;
            break;
        case Direction::Down:
            riderPos.z = riderPos.z - xSpeed * dt;
            break;
        }
        break;
    }
    riderPos.z += zSpeed * dt;
    setPosition(Ogre::Vector3(riderPos.x, std::max(altura(riderPos.x, riderPos.z), riderPos.y + ySpeed*dt), riderPos.z));
    ySpeed -= gravity * dt;

    // roll
    //mCamNode->roll(Ogre::Degree(mRoll));
    return true;
}
