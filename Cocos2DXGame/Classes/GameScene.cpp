#include "GameScene.h"


GameType GameScene::m_type = GT_ONLINE;
GameState GameScene::m_state = GS_RUNNING;

GameScene::GameScene():
m_sprite()
m_playerManager(nullptr)
{
}

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    if (scene)
        CCLOG("Scene creation succesful!");
    auto layer = GameScene::create();
    if (layer)
        CCLOG("Layer creation succesful!");
    scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    if (GT_LOCAL == m_type)
    {
        if (!initLocalGame())
        {
            CCLOGERROR("Local game init failed!");
            return false;
        }
        else
        {
            CCLOG("Local game init succesfull!");
        }
    }

    else if (GT_ONLINE == m_type)
    {
        if (!initOnlineGame())
        {
            CCLOGERROR("Online game init failed!");
            return false;
        }
        else
        {
            CCLOG("Online game init succesfull!");
        }
    }

    this->scheduleUpdate();

    return true;
}

bool GameScene::initLocalGame()
{
    // TODO: Player & Ball classes
    m_playerManager = new PlayerManager();
    GameScene* scene = this;
    m_playerManager->initLocalPlayers(scene);

    return true;
}

bool GameScene::initOnlineGame()
{
    return false;
}

void GameScene::update(float delta)
{
    // TODO: separate into local and online
    auto position = m_sprite->getPosition();
    position.x -= 250 * delta;

    if (position.x < 0 - (m_sprite->getBoundingBox().size.width / 2))
    {
        position.x = this->getBoundingBox().getMaxX() + m_sprite->getBoundingBox().size.width / 2;
    }

    m_sprite->setPosition(position);
}