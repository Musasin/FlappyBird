#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "CharacterReader.h"
#include "ObstacleReader.h"
#include "Character.h"
#include "Obstacle.h"
#include "Constants.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    CSLoader* instance = CSLoader::getInstance();
    instance->registReaderObject("CharacterReader", (ObjectFactory::Instance) CharacterReader::getInstance);
    instance->registReaderObject("ObstacleReader", (ObjectFactory::Instance) ObstacleReader::getInstance);
    
    auto rootNode = CSLoader::createNode("MainScene.csb");
    
    Size size = Director::getInstance()->getVisibleSize();
    rootNode->setContentSize(size);
    ui::Helper::doLayout(rootNode);
    
    this->background = rootNode->getChildByName("back");
    background->getChildByName("ground")->setLocalZOrder(1);
    background->getChildByName("character")->setLocalZOrder(2);
    this->character = this->background->getChildByName<Character*>("character");
    
    addChild(rootNode);
    
    return true;
}

#pragma mark - protected

void MainScene::onEnter()
{
    Layer::onEnter();
    this->setupTouchHandling();
    this->scheduleUpdate();
    
    triggerReady();
}

void MainScene::update(float dt)
{
    
    for(auto obstacle : this->obstacles)
    {
        obstacle->moveLeft(dt * OBSTACLE_SPEED);
    }
    
    Rect characterRect = this->character->getRect();
    
    for(auto obstacle : this->obstacles)
    {
        auto obstacleRects = obstacle->getRects();
        
        for(Rect obstacleRect : obstacleRects)
        {   
            bool hit = characterRect.intersectsRect(obstacleRect);
            
            if(hit)
            {
                CCLOG("HIT!");
                triggerGameOver();
            }
            else
                CCLOG("NOT HIT");
        }
    }
    
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        switch(this->state)
        {
        case State::Ready:
            this->triggerPlaying();
            this->character->jump();
        break;
        case State::Playing:
            this->character->jump();
            break;
        case State::GameOver:
            auto nextGameScene = MainScene::createScene();
            auto crossFade = TransitionFade::create(0.5f, nextGameScene);
            Director::getInstance()->replaceScene(crossFade);
            break;
        //default:
        //    break;
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainScene::createObstacle(float dt)
{
    Obstacle* obstacle = dynamic_cast<Obstacle*>(CSLoader::createNode("Obstacle.csb"));
    this->background->addChild(obstacle);
    
    float y = OBSTACLE_MIN_Y + (CCRANDOM_0_1() * (OBSTACLE_MAX_Y - OBSTACLE_MIN_Y));
    obstacle->setPosition(Vec2(OBSTACLE_INIT_X, y));
    this->obstacles.pushBack(obstacle);
    
    if(this->obstacles.size() > OBSTACLE_LIMIT)
    {
        this->obstacles.front()->removeFromParent();
        this->obstacles.erase(this->obstacles.begin());
    }
    
}

void MainScene::triggerReady()
{
    this->state = State::Ready;
    this->character->stopPlay();
}
void MainScene::triggerPlaying()
{
    this->state = State::Playing;
    this->character->startPlay();
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle), OBSTACLE_TIME_SPAN);
}
void MainScene::triggerGameOver()
{
    this->state = State::GameOver;
    this->unscheduleAllCallbacks();
    this->unschedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle));
}



