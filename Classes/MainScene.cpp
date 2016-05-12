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
    
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle), OBSTACLE_TIME_SPAN);
}

void MainScene::update(float dt)
{
    for(auto obstacle : this->obstacles)
    {
        obstacle->moveLeft(dt * OBSTACLE_SPEED);
    }
}

void MainScene::setupTouchHandling()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        //this->createObstacle();
        this->character->jump();
        //Vec2 touchLocation = this->convertTouchToNodeSpace(touch);
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



