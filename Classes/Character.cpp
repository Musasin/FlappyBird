//
//  Character.cpp
//  SushiNeko
//
//  Created by Eiji Ito on 2016/04/26.
//
//

#include "Character.h"
#include <math.h>

using namespace cocos2d;

#pragma mark - public

bool Character::init()
{
    if ( !Node::init() )
    {
        return false;
    }
    
    this->timeline = CSLoader::createTimeline("Character.csb");
    this->timeline->retain();
    
    this->velocity = 0;
    
    this->state = birdState::STOP;
    
    return true;
}

void Character::onEnter()
{
    Node::onEnter();
    
    this->scheduleUpdate();
}

void Character::update(float dt)
{
    if(this->state != birdState::STOP)
        this->velocity += ACCEL * dt;
    this->velocity = MAX(this->velocity, -500);
    this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
}

void Character::jump()
{
    this->state = birdState::JUMP;
    this->velocity = 250;
}