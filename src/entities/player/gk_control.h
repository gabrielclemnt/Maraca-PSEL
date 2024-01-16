#ifndef GK_CONTROL_H
#define GK_CONTROL_H

#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>

class gk_control
{
public:

    gk_control(WorldMap *worldMap);

    void setPlayer(Player *player);
    void defend();
    void pass(QVector2D target);

    QVector2D getGoalCircleCenter();

private:
    Player *_player;
    WorldMap *_worldMap;

};

#endif // GK_CONTROL_H
