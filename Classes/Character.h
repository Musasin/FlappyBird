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
    
protected:
    cocostudio::timeline::ActionTimeline* timeline;
};

#endif /* defined(__Bird__Character__) */