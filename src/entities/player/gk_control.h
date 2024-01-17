#ifndef GK_CONTROL_H
#define GK_CONTROL_H

#include <QMap>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <math.h>
#include <QVector2D>
#include <QObject>
#include <QVector>

class gk_control
{
public:

    gk_control(WorldMap *worldMap);

    void setPlayer(Player *player);
    void defend();
    void pass(Player *player5);

    QVector2D getGoalCircleCenter();

private:
    Player *_player;
    WorldMap *_worldMap;

};

#endif // GK_CONTROL_H
