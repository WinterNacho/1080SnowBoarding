#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "utils.h"


class Rider : public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    Rider(Ogre::SceneNode* playerNode, Ogre::SceneNode* camNode)
        : mNode(playerNode), mCamNode(camNode) {}
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
    float zSpeed = 30.0;
    float ySpeed = 0.0;
    float gravity = 50.0;
    float mRoll = 20;
    float mPitch = 0;
    Ogre::Vector3 camDisplacement = Ogre::Vector3(0, 8, -20);
    enum class Direction : char { Left, Right, Up, Down, None };

    Direction direction = Direction::None;
    State state = State::Normal;
};

