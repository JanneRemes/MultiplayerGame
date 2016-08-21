#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "enumerations.h"

class GameScene : public cocos2d::Layer
{
public:
    GameScene();
    ~GameScene();

    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameScene);

    void update(float) override;

    GameState m_state;

private:
    cocos2d::Sprite* sprite;
};

#endif // __GAME_SCENE_H__