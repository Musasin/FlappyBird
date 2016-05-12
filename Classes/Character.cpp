//
//  Character.cpp
//  SushiNeko
//
//  Created by Eiji Ito on 2016/04/26.
//
//

#include "Character.h"
#include "Constants.h"
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
    this->isPlaying = false;
    
    return true;
}

void Character::onEnter()
{
    Node::onEnter();
    
    this->scheduleUpdate();
}

void Character::update(float dt)
{
    if(isPlaying)
    {
        this->velocity += GRAVITY_ACCEL * dt;
        this->velocity = MAX(this->velocity, -500);
        this->setPosition(this->getPosition() + Vec2(0, this->velocity * dt));
    }
}

void Character::jump()
{
    this->velocity = JUMP_SPEED;
    
    this->stopAllActions();
    this->runAction(this->timeline);
    
    this->timeline->play("fly", false);
    
}

Rect Character::getRect()
{
    Size spriteSize = this->getChildByName("bird")->Sprite::getContentSize() * 0.8;
    Vec2 rectPosition = this->getPosition() - (spriteSize / 2);
//
//    Sprite* sprite = Sprite::create();
//    sprite->setTextureRect(Rect(rectPosition, spriteSize));
//    sprite->setColor(Color3B::BLACK);
//    this->addChild(sprite);
    
    return Rect(rectPosition, spriteSize);
}


void Character::setIsPlaying(bool isPlaying)
{
    this->isPlaying = isPlaying;
}
void Character::startPlay()
{
    this->isPlaying = true;
}
void Character::stopPlay()
{
    this->isPlaying = false;
}