//
//  Character.hpp
//  SushiNeko
//
//  Created by Eiji Ito on 2016/04/26.
//
//
#ifndef __Bird__Character__
#define __Bird__Character__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"

class Character : public cocos2d::Node
{
public:
    CREATE_FUNC(Character);
    
    bool init() override;
    
    void onEnter() override;
    void update(float dt) override;
    void jump();
    cocos2d::Rect getRect();
    
    void setIsPlaying(bool isPlaying);
    void startPlay();
    void stopPlay();
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
    
    float velocity;
    bool isPlaying;
};

#endif /* defined(__Bird__Character__) */