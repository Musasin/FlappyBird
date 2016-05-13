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
    
    this->groundA = background->getChildByName("groundA");
    this->groundA->setLocalZOrder(1);
    this->groundB = background->getChildByName("groundB");
    this->groundB->setLocalZOrder(1);
                           
    background->getChildByName("character")->setLocalZOrder(2);
    this->character = this->background->getChildByName<Character*>("character");
    
    this->scoreLabel = this->background->getChildByName<ui::TextBMFont*>("scoreLabel");
    this->scoreLabel->setLocalZOrder(3);
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
    
    Rect characterRect = this->character->getRect();
    
    switch(this->state)
    {
        case State::Ready:
            moveGround(dt);
            break;
        case State::Playing:
        {
            float moveDistance = dt * OBSTACLE_SPEED;
            moveGround(dt);
            
            for(auto obstacle : this->obstacles)
            {
                obstacle->moveLeft(dt * OBSTACLE_SPEED);
            }
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
            
            float characterX = this->character->getPositionX();
            for(auto obstacle : this->obstacles)
            {
                float currentX = obstacle->getPositionX();
                if(currentX + moveDistance > characterX && currentX <= characterX) {
                    setScore(this->score + 1);
                }
            }
            break;
        }
        case State::GameOver:
            break;
    }
    
    if(characterRect.origin.y <= groundA->getPosition().y)
    {
        this->character->setPositionY(groundA->getPosition().y + (this->character->getChildByName("bird")->getContentSize().height / 2));
        //this->character->setPositionY(100);
        CCLOG("HIT!");
        triggerGameOver();
        character->stopPlay();
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
    this->setScore(0);
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
    //this->unscheduleAllCallbacks();
    this->unschedule(CC_SCHEDULE_SELECTOR(MainScene::createObstacle));
}

void MainScene::moveGround(float dt)
{
    groundA->setPosition(groundA->getPosition() - Vec2(dt * OBSTACLE_SPEED, 0));
    groundB->setPosition(groundB->getPosition() - Vec2(dt * OBSTACLE_SPEED, 0));
    
    //ラムダ式による実装
    auto groundScrollCheck = [&](Node* ground, Node* baddyGround) {
        if(ground->getPositionX() < 0) {
            ground->setPosition(baddyGround->getPosition() + Vec2(baddyGround->getContentSize().width, 0.0f));
        }
    };
    groundScrollCheck(this->groundA, this->groundB);
    groundScrollCheck(this->groundB, this->groundA);
    
}

void MainScene::setScore(int score)
{
    this->score = score;
    this->scoreLabel->setString(std::to_string(score));
}

