#include "SplashScreenScene.h"

USING_NS_CC;

Scene* SplashScreen::createScene()
{
    auto scene = Scene::create();
    if (scene)
        CCLOG("Scene creation succesful!");
    auto layer = SplashScreen::create();
    if (layer)
        CCLOG("Layer creation succesful!");
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool SplashScreen::init()
{
    if ( !Layer::init() )
    {
        return false;
    }    
    
    auto sprite = Sprite::create("images/peacock.jpg");
    auto sprite2 = Sprite::create("images/HelloWorld.png");

    if(!sprite)
        CCLOG("Texture 1 loading failed!");
    if (!sprite2)
        CCLOG("Texture 2 loading failed!");

    // Screen origin is at the bottom left corner
    // Sprite default anchor point is at the middle of texture
    sprite->setPosition(512, 384);

    sprite2->setScale(0.5f);
    sprite2->setPosition(512, 130);

    this->addChild(sprite, 0);
    this->addChild(sprite2, 0);
    
    return true;
}


void SplashScreen::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
