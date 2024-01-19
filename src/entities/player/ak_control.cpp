#include "ak_control.h"
#define ROBOT_DIAMETER (2.0F * ROBOT_RADIUS)
#define BALL_RADIUS 0.0215
#define BALL_DIAMETER (2.0F * BALL_RADIUS)
#define ROBOT_RADIUS 0.09
#define BALL_SPEED 6.0

ak_control::ak_control(WorldMap *worldMap) {
    _worldMap = worldMap;
    _aPlayer = nullptr;
}

void ak_control::setAPlayer (Player *player)
{
    _aPlayer = player;
}

void ak_control::atackposition1()
{
    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        const QVector2D pontoCerto(0.2f, 1.7f);
        _aPlayer->goTo(pontoCerto);
        _aPlayer->rotateTo(ballPosition);
    }
}
