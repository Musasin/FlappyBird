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

std::vector<Rect> Obstacle::getRects()
{
    std::vector<Rect> rects;
    
    auto pipe = this->getChildByName<Sprite*>("pipe_top");
    Size spriteSize = pipe->Sprite::getContentSize();
    Vec2 position = this->getPosition() + pipe->getPosition();
    Vec2 rectPosition = Vec2(position.x, position.y);
    rects.push_back(Rect(rectPosition, spriteSize));
    
    pipe = this->getChildByName<Sprite*>("pipe_bottom");
    spriteSize = pipe->Sprite::getContentSize();
    position = this->getPosition() + pipe->getPosition();
    rectPosition = Vec2(position.x, position.y - spriteSize.height);
    rects.push_back(Rect(rectPosition, spriteSize));
    
    
    //auto pipeTopRect = pipeTop->getBoundingBox();
    //pipeTopRect.origin += this->getPosition();
    //上記二行でもOK. BoundingBoxは親から見たRectを割り出してくれる。
    
//    Sprite* sprite = Sprite::create();
//    sprite->setTextureRect(Rect(Vec2::ZERO, spriteSize));
//    sprite->setColor(Color3B::BLACK);
//    sprite->setPosition(rectPosition);
//    this->getParent()->addChild(sprite);
    
    return rects;
}