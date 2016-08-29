#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "enumerations.h"

#include "PlayerManager.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    GameScene();

    static cocos2d::Scene* createScene();
    virtual bool init() override;

    bool initLocalGame();
    bool initOnlineGame();

    void update(float) override;
    
    static void GameScene::setGameType(GameType type)
    {
        m_type = type;
    }

    static void GameScene::setGameState(GameState state)
    {
        m_state = state;
    }

    CREATE_FUNC(GameScene);

private:
    Sprite* m_sprite;
    PlayerManager* m_playerManager;

    static GameType m_type;
    static GameState m_state;
};

#endif // __GAME_SCENE_H__