#ifndef __SPLASHSCREEN_SCENE_H__
#define __SPLASHSCREEN_SCENE_H__

#include "cocos2d.h"
#include "AppDelegate.h"

class SplashScreen : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    virtual void update(float delta) override;

    // INPUT
    bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
    double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SplashScreen);

private:
    static std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;
    cocos2d::Label* label;
};

#endif // __SPLASHSCREEN_SCENE_H__
