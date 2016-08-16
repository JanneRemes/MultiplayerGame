#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "cocos2d.h"

class GameApp : public cocos2d::Layer
{
public:
    GameApp();
    ~GameApp();

    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameApp);

    void update(float) override;


private:
    cocos2d::Sprite* sprite;
};

#endif // __GAME_APP_H__