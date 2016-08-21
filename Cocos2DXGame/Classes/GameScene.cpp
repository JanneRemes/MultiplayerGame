#include "GameScene.h"

USING_NS_CC;

GameScene::GameScene():
m_state()
{

}

GameScene::~GameScene()
{
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    sprite = Sprite::create("images/purple.png");
    sprite->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
    this->addChild(sprite, 0);

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

void GameScene::update(float delta)
{
    auto position = sprite->getPosition();
    position.x -= 250 * delta;

    if (position.x < 0 - (sprite->getBoundingBox().size.width / 2))
    {
        position.x = this->getBoundingBox().getMaxX() + sprite->getBoundingBox().size.width / 2;
    }

    sprite->setPosition(position);
}