#ifndef __ENUMERATIONS_H__
#define __ENUMERATIONS_H__

struct GameState
{
    enum State
    {
        GS_RUNNING,
        GS_PAUSED,
        GS_FINISHED
    };

    enum Type
    {
        GT_ONLINE,
        GT_LOCAL
    };
};

struct BallState
{
    enum State
    {
        BS_IN_PLAY,
        BS_ON_PAD
    };
};

struct PlayerState
{
    // TODO enums
};
#endif // __ENUMERATIONS_H__