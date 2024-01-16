#include "gk_control.h"

gk_control::gk_control(WorldMap *worldMap)
{
    _worldMap = worldMap;
    _player = nullptr;
}

void gk_control::setPlayer (Player *player)
{
    _player = player;
}

void gk_control::defend()
{
    if(_player != nullptr){
        _player->goTo(_worldMap->ourGoalCenter());
        _player->rotateTo(_worldMap->ballPosition());
    }
}

void gk_control::pass(QVector2D target)
{
  //faz o passe para um player especifico
    if(_player != nullptr){
       _player ->goTo(target);//
    }
}

