#pragma once

#include "Ogre.h"
#include "OgreApplicationContext.h"

class Rider : public OgreBites::InputListener, public Ogre::FrameListener
{
public:
    Rider(Ogre::SceneNode* playerNode, Ogre::SceneNode* camNode)
        : mNode(playerNode), mCamNode(camNode) {}
    virtual ~Rider() {}

    void setup();
    void setPosition(const Ogre::Vector3& pos);
    bool isColliding(const Ogre::Vector3& entityPos);

    bool keyPressed(const OgreBites::KeyboardEvent& evt);
    bool keyReleased(const OgreBites::KeyboardEvent& evt);
    bool axisMoved(const OgreBites::AxisEvent& evt);
    bool frameStarted(const Ogre::FrameEvent& evt);
private:
    Ogre::SceneNode* mNode;
    Ogre::SceneNode* mCamNode;
    Ogre::Vector3 riderPos;
    float xSpeed = 25.0;
    float gravity = 5.0;
    Ogre::Vector3 camDisplacement = Ogre::Vector3(0, 10, -40);
    enum class Direction : char { Left, Right, Up, Down, None };
    Direction direction = Direction::None;
};

