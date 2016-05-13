//
//  Obstacle.h
//  FlappyBird
//
//  Created by Eiji Ito on 2016/05/12.
//
//
#ifndef __Bird__Obstacle__
#define __Bird__Obstacle__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Obstacle : public cocos2d::Node
{
public:
    CREATE_FUNC(Obstacle);
    
    bool init() override;
    void moveLeft(float distance);
    
    void onEnter() override;
    void update(float dt) override;
    std::vector<cocos2d::Rect> getRects();
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
};

#endif /* defined(__Bird__Obstacle__) */