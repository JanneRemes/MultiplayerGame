#ifndef __MENUSCREEN_SCENE_H__
#define __MENUSCREEN_SCENE_H__

#include "cocos2d.h"
#include "cocos2d/cocos/ui/UIButton.h"

class MenuScreen : public cocos2d::Layer
{
public:
    MenuScreen();

    static cocos2d::Scene* createScene();

    virtual bool init();


    void keyCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    void createMenuButtons();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);


    // implement the "static create()" method manually
    CREATE_FUNC(MenuScreen);

private:
    cocos2d::Label* m_menuLabel;

    cocos2d::ui::Button* m_localButton;
    cocos2d::ui::Button* m_onlineButton;
    cocos2d::ui::Button* m_exitButton;

    bool m_isButtonHighlighted;
};

#endif // __MENUSCREEN_SCENE_H__