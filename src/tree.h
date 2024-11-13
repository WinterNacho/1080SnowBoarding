#pragma once

#include "Ogre.h"
#include "utils.h"
#include "rider.h"
class Tree : public Ogre::FrameListener {
public:
    Tree(Ogre::SceneNode* Node, Rider* playerRider, Ogre::Vector3 pos)
        : mNode(Node), player(playerRider), position(pos) {};
    void setup();
    void setPosition(const Ogre::Vector3& pos);
    bool collision(const Ogre::Vector3& point) const;
    bool frameStarted(const Ogre::FrameEvent& evt);

private:
    Ogre::SceneNode* mNode;
    Rider* player;
    Ogre::Vector3 position;
    Ogre::Vector3 minCorner = Ogre::Vector3(-3, -5, -3);
    Ogre::Vector3 maxCorner = Ogre::Vector3(3, 5, 3);

};

