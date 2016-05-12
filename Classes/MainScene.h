#ifndef __MAIN_SCENE_SCENE_H__
#define __MAIN_SCENE_SCENE_H__

#include "cocos2d.h"

#include "Obstacle.h"

class Character;

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init() override;
    void onEnter() override;
    void update(float dt) override;
    

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
    
private:
    Character* character;
    cocos2d::Vector<Obstacle*> obstacles;
    cocos2d::Node* background;
    
    void setupTouchHandling();
    void createObstacle(float dt);
    
};

#endif // __MAIN_SCENE_SCENE_H__
