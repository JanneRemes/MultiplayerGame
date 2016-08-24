#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "enumerations.h"


class GameScene : public cocos2d::Layer
{
public:
    GameScene();

    static cocos2d::Scene* createScene();
    virtual bool init() override;

    bool initLocalGame();
    bool initOnlineGame();

    void update(float) override;
    
    static void GameScene::setGameType(GameState::Type type)
    {
        m_type = type;
    }

    static void GameScene::setGameState(GameState::State state)
    {
        m_state = state;
    }

    CREATE_FUNC(GameScene);

private:
    cocos2d::Sprite* m_sprite;

    static GameState::Type m_type;
    static GameState::State m_state;
};

#endif // __GAME_SCENE_H__