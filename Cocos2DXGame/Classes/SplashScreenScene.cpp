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

    if (!sprite)
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

    label = cocos2d::Label::createWithSystemFont("Splash Screen", "Arial", 32);
    label->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
    label->setColor(cocos2d::Color3B::GREEN);
    addChild(label);

    auto eventListener = EventListenerKeyboard::create();

    Director::getInstance()->getOpenGLView()->setIMEKeyboardState(true);
    eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        // If a key already exists, do nothing as it will already have a time stamp
        // Otherwise, set's the timestamp to now
        if (keys.find(keyCode) == keys.end())
        {
            keys[keyCode] = std::chrono::high_resolution_clock::now();
        }
    };

    eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event)
    {
        // Remove the key. std::map.erase() doesn't care if the key doesn't exist
        keys.erase(keyCode);
    };

    // INPUT
    //eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event)
    //{
    //    Vec2 loc = event->getCurrentTarget()->getPosition();
    //    
    //    switch (keyCode)
    //    {
    //        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
    //        case EventKeyboard::KeyCode::KEY_A:
    //            event->getCurrentTarget()->setPosition(--loc.x, loc.y);
    //            break;
    //        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    //        case EventKeyboard::KeyCode::KEY_D:
    //            event->getCurrentTarget()->setPosition(++loc.x, loc.y);
    //            break;
    //        case EventKeyboard::KeyCode::KEY_UP_ARROW:
    //        case EventKeyboard::KeyCode::KEY_W:
    //            event->getCurrentTarget()->setPosition(loc.x, ++loc.y);
    //            break;
    //        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    //        case EventKeyboard::KeyCode::KEY_S:
    //            event->getCurrentTarget()->setPosition(loc.x, --loc.y);
    //    }
    //};

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite2);

    // Let cocos know we have an update function to be called
    this->scheduleUpdate();

    return true;
}

bool SplashScreen::isKeyPressed(EventKeyboard::KeyCode code)
{
    // Check if the key is currently pressed by seeing if it's in the std::map keys
    if (keys.find(code) != keys.end())
        return true;
    return false;
}

double SplashScreen::keyPressedDuration(EventKeyboard::KeyCode code)
{
    if (!isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL))
        return 0; // Not pressed, no duration

    // Return the amount of time that has elapsed between now and when the user
    // first started holding down the key in milliseconds
    // Obviously the start time is the value we hold in our std::map keys
    return std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::high_resolution_clock::now() - keys[code]).count();
}

void SplashScreen::update(float delta)
{
    // Registers an update function that checks to see if the CTRL key is pressed
    // and displays how long, otherwise tell the user to press it
    Node::update(delta);
    if (isKeyPressed(EventKeyboard::KeyCode::KEY_CTRL))
    {
        std::stringstream ss;
        ss << "Control key has been pressed for " << keyPressedDuration(EventKeyboard::KeyCode::KEY_CTRL)
            << " ms";
        label->setString(ss.str().c_str());
    }
    else
    {
        label->setString("Press the CTRL key");
    }
}

// Because cocos2d-x requires createScene to be static, we need to make other non-pointer members static
std::map<cocos2d::EventKeyboard::KeyCode,
    std::chrono::high_resolution_clock::time_point> SplashScreen::keys;

void SplashScreen::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
