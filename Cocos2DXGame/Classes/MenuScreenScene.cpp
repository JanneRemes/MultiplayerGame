#include "MenuScreenScene.h"
#include "GameScene.h"
#include "enumerations.h"

USING_NS_CC;

MenuScreen::MenuScreen():
m_menuLabel(),
m_localButton(),
m_onlineButton(),
m_exitButton(),
m_isButtonHighlighted(false)
{
}

Scene* MenuScreen::createScene()
{
    auto scene = Scene::create();
    if (scene)
        CCLOG("Scene creation succesful!");
    auto layer = MenuScreen::create();
    if (layer)
        CCLOG("Layer creation succesful!");
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool MenuScreen::init()
{
    if (!Layer::init())
    {
        return false;
    }

    m_menuLabel = cocos2d::Label::createWithSystemFont("MAIN MENU", "Arial", 50);
    m_menuLabel->enableBold();
    m_menuLabel->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMaxY() - 100);
    m_menuLabel->setColor(Color3B::GREEN);
    this->addChild(m_menuLabel);

    createMenuButtons();
    
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyReleased = CC_CALLBACK_2(MenuScreen::keyCallback, this);

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void MenuScreen::keyCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW || keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW
        || keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_S
        || keyCode == EventKeyboard::KeyCode::KEY_ENTER)
    {
        if (!m_isButtonHighlighted)
        {
            m_localButton->setHighlighted(true);
            m_isButtonHighlighted = true;
        }
        else
        {
            if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW
                || keyCode == EventKeyboard::KeyCode::KEY_S)
            {
                if (m_localButton->isHighlighted())
                {
                    m_localButton->setHighlighted(false);
                    m_onlineButton->setHighlighted(true);
                }
                else if (m_onlineButton->isHighlighted())
                {
                    m_onlineButton->setHighlighted(false);
                    m_exitButton->setHighlighted(true);
                }
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW
                || keyCode == EventKeyboard::KeyCode::KEY_W)
            {
                if (m_onlineButton->isHighlighted())
                {
                    m_onlineButton->setHighlighted(false);
                    m_localButton->setHighlighted(true);
                }
                else if (m_exitButton->isHighlighted())
                {
                    m_exitButton->setHighlighted(false);
                    m_onlineButton->setHighlighted(true);
                }
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
            {
                if (m_localButton->isHighlighted())
                {
                    // Start local game
                    CCLOG("LOCAL GAME SELECTED");
                    GameScene::setGameType(GameState::GT_LOCAL);
                    auto scene = GameScene::createScene();
                    Director::getInstance()->replaceScene(scene);
                }
                else if (m_onlineButton->isHighlighted())
                {
                    // Go to multiplayer menu
                    CCLOG("ONLINE GAME SELECTED");
                    GameScene::setGameType(GameState::GT_ONLINE);
                }
                else if (m_exitButton->isHighlighted())
                {
                    // Close the game
                    CCLOG("QUIT GAME SELECTED");
                    Director::getInstance()->end();
                }
            }
        }
        CCLOG("Key with keycode %d released", keyCode);
    }
}

void MenuScreen::createMenuButtons()
{
    // LOCAL GAME
    m_localButton = ui::Button::create();
    m_localButton->loadTextures("images/MenuButtonDefault.png", "images/MenuButtonHighlighted.png");
    m_localButton->setPosition(Point(this->getBoundingBox().getMidX(), 
                                     this->getBoundingBox().getMaxY() * 0.6));
    m_localButton->setTitleText("LOCAL GAME");
    m_localButton->setTitleFontSize(22);
    m_localButton->setTitleColor(Color3B::WHITE);
    m_localButton->setPressedActionEnabled(true);

    // ONLINE GAME
    m_onlineButton = ui::Button::create();
    m_onlineButton->loadTextures("images/MenuButtonDefault.png", "images/MenuButtonHighlighted.png");
    m_onlineButton->setPosition(Point(this->getBoundingBox().getMidX(),
                                      this->getBoundingBox().getMaxY() * 0.4));
    m_onlineButton->setTitleText("ONLINE GAME");
    m_onlineButton->setTitleFontSize(22);
    m_onlineButton->setTitleColor(Color3B::WHITE);
    m_onlineButton->setPressedActionEnabled(true);

    // QUIT GAME
    m_exitButton = ui::Button::create();
    m_exitButton->loadTextures("images/MenuButtonDefault.png", "images/MenuButtonHighlighted.png");
    m_exitButton->setPosition(Point(this->getBoundingBox().getMidX(), 
                                    this->getBoundingBox().getMaxY() * 0.2));
    m_exitButton->setTitleText("QUIT");
    m_exitButton->setTitleFontSize(22);
    m_exitButton->setTitleColor(Color3B::WHITE);
    m_exitButton->setPressedActionEnabled(true);

    this->addChild(m_localButton, 0);
    this->addChild(m_onlineButton, 0);
    this->addChild(m_exitButton, 0);
}

void MenuScreen::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}