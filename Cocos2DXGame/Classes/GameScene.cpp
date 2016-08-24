#include "GameScene.h"

USING_NS_CC;

GameState::Type GameScene::m_type = GameState::GT_ONLINE;
GameState::State GameScene::m_state = GameState::GS_RUNNING;

GameScene::GameScene():
m_sprite()
{
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    if (scene)
        CCLOG("Scene creation succesful!");
    auto layer = GameScene::create();
    if (layer)
        CCLOG("Layer creation succesful!");
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    if (GameState::GT_LOCAL == m_type)
    {
        if (!initLocalGame())
        {
            CCLOGERROR("Local game init failed!");
            return false;
        }
        else
        {
            CCLOG("Local game init succesfull!");
        }
    }

    else if (GameState::GT_ONLINE == m_type)
    {
        if (!initOnlineGame())
        {
            CCLOGERROR("Online game init failed!");
            return false;
        }
        else
        {
            CCLOG("Online game init succesfull!");
        }
    }

    //auto listener = cocos2d::EventListenerKeyboard::create();
    //listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)->void
    //{
    //    // This is where our different actions are going to be implemented
    //    auto action = cocos2d::MoveTo::create(2, cocos2d::Vec2(0, 0));
    //    sprite->runAction(action);
    //};

    //this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    this->scheduleUpdate();

    return true;
}

bool GameScene::initLocalGame()
{
    // TODO: Player & Ball classes

    m_sprite = Sprite::create("images/purple.png");
    
    if (!m_sprite)
        return false;

    m_sprite->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
    this->addChild(m_sprite, 0);

    return true;
}

bool GameScene::initOnlineGame()
{
    return false;
}

void GameScene::update(float delta)
{
    // TODO: separate into local and online
    auto position = m_sprite->getPosition();
    position.x -= 250 * delta;

    if (position.x < 0 - (m_sprite->getBoundingBox().size.width / 2))
    {
        position.x = this->getBoundingBox().getMaxX() + m_sprite->getBoundingBox().size.width / 2;
    }

    m_sprite->setPosition(position);
}