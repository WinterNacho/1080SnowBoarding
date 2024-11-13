#include "rider.h"
#include <iostream>
#include "utils.h"

bool Rider::keyPressed(const OgreBites::KeyboardEvent& evt) {
    if (evt.keysym.sym == OgreBites::SDLK_LEFT or evt.keysym.sym == 'a')
        directionX = DirectionX::Left;

    if (evt.keysym.sym == OgreBites::SDLK_RIGHT or evt.keysym.sym == 'd')
        directionX = DirectionX::Right;

    if (evt.keysym.sym == OgreBites::SDLK_UP or evt.keysym.sym == 'w')
        directionZ = DirectionZ::Up;

    if (evt.keysym.sym == OgreBites::SDLK_DOWN or evt.keysym.sym == 's')
        directionZ = DirectionZ::Down;

    return true;
}

bool Rider::keyReleased(const OgreBites::KeyboardEvent& evt) {
    if ((evt.keysym.sym == OgreBites::SDLK_LEFT or evt.keysym.sym == 'a') and directionX == DirectionX::Left)
        directionX = DirectionX::None;

    if ((evt.keysym.sym == OgreBites::SDLK_RIGHT or evt.keysym.sym == 'd') and directionX == DirectionX::Right)
        directionX = DirectionX::None;

    if ((evt.keysym.sym == OgreBites::SDLK_UP or evt.keysym.sym == 'w') and directionZ == DirectionZ::Up)
        directionZ = DirectionZ::None;

    if ((evt.keysym.sym == OgreBites::SDLK_DOWN or evt.keysym.sym == 's') and directionZ == DirectionZ::Down)
        directionZ = DirectionZ::None;

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
        if(state != st)
            ma_sound_start(collisionSound);
        if (state == st)
            break;
        mNode->yaw(Ogre::Radian(90));
        inmuneTimer = 0;
        break;
    case State::Jumped:
        ma_sound_start(rampSound);
        ySpeed = 35.0;
        break;
    }
    state = st;
}


bool Rider::axisMoved(const OgreBites::AxisEvent& evt) {
    unsigned int axisKey = static_cast<unsigned int>(evt.axis);

    if (axisKey == 0) {
        if (evt.value < -10000)
            directionX = DirectionX::Left;
        else if (evt.value > 10000)
            directionX = DirectionX::Right;
        else
            directionX = DirectionX::None;
    }

    else if (axisKey == 1) {
        if (evt.value < -10000)
            directionZ = DirectionZ::Up; 
        else if (evt.value > 10000)
            directionZ = DirectionZ::Down;
        else
            directionZ = DirectionZ::None;
    }

    return true;
}
bool Rider::frameStarted(const Ogre::FrameEvent& evt) {
    float dt = evt.timeSinceLastFrame;

    switch (state)
    {
    case State::Stunned:
        zSpeed = 10.0;
        mNode->roll(Ogre::Radian(0.05));
        if (inmuneTimer >= 3) {
            mNode->setOrientation(Ogre::Quaternion::IDENTITY);
            state = State::Normal;
        }
        break;

    case State::Jumped:
        zSpeed = 80.0;
        mNode->pitch(Ogre::Radian(0.22));
        if (altura(riderPos.x, riderPos.z) >= riderPos.y) {
            state = State::Normal;
            mNode->setOrientation(Ogre::Quaternion::IDENTITY);
        }
        break;
        
    case State::Normal:
        zSpeed = 40.0;
        switch (directionX)
        {
        case DirectionX::Left:
            riderPos.x = std::min(20.5f, riderPos.x + xSpeed * dt);
            if (riderPos.x <  20.5f)
            mCamNode->roll(Ogre::Degree(0.2));
            break;
        case DirectionX::Right:
            riderPos.x = std::max(0.5f, riderPos.x - xSpeed * dt);
            if(riderPos.x > 0.5f)
                mCamNode->roll(Ogre::Degree(-0.2));
            break;
        }
        
        switch (directionZ) {
        case DirectionZ::Up:
            riderPos.z = riderPos.z + xSpeed * dt;
            break;
        case DirectionZ::Down:
            riderPos.z = riderPos.z - xSpeed * dt;
            break;
        }
       
    }
    riderPos.z += zSpeed * dt;
    setPosition(Ogre::Vector3(riderPos.x, std::max(altura(riderPos.x, riderPos.z), riderPos.y + ySpeed*dt), riderPos.z));
    ySpeed -= gravity * dt;
    inmuneTimer += dt;
    return true;
}
