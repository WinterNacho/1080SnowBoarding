#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "utils.h"
#include "miniaudio.h"

class Rider : public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    Rider(Ogre::SceneNode* playerNode, Ogre::SceneNode* camNode, ma_sound* collision, ma_sound* ramp)
        : mNode(playerNode), mCamNode(camNode), collisionSound(collision),  rampSound(ramp) {}
    virtual ~Rider() {}

    void setup();
    void setPosition(const Ogre::Vector3& pos);
    Ogre::Vector3 getPosition();

    void collision(State st);
    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool keyReleased(const OgreBites::KeyboardEvent& evt);
    bool axisMoved(const OgreBites::AxisEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt);
private:
    Ogre::SceneNode* mNode;
    Ogre::SceneNode* mCamNode;
    Ogre::Vector3 riderPos;
    float xSpeed = 20.0;
    float zSpeed;
    float ySpeed = 0.0;
    float gravity = 50.0;
    float inmuneTimer = 0;
    Ogre::Vector3 camDisplacement = Ogre::Vector3(0, 8, -20);
    enum class DirectionX : char { Left, Right, None };
    enum class DirectionZ : char { Up, Down, None };
    DirectionX directionX = DirectionX::None;
    DirectionZ directionZ = DirectionZ::None;
    State state = State::Normal;

    //audio

    ma_sound* collisionSound;
    ma_sound* rampSound;
};

