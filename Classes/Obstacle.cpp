//
//  Obstacle.cpp
//  SushiNeko
//
//  Created by Eiji Ito on 2016/04/26.
//
//

#include "Obstacle.h"
#include "Constants.h"
#include <math.h>

using namespace cocos2d;

#pragma mark - public

bool Obstacle::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    this->timeline = CSLoader::createTimeline("Obstacle.csb");
    this->timeline->retain();
    
    return true;
}

void Obstacle::moveLeft(float distance)
{
    this->setPosition(this->getPosition() + Vec2(-distance, 0));
}

void Obstacle::onEnter()
{
    Node::onEnter();
    
    this->scheduleUpdate();
}

void Obstacle::update(float dt)
{
}