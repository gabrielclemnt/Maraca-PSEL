#ifndef DK_CONTROL_H
#define DK_CONTROL_H

#include <QMap>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <math.h>
#include <QVector2D>
#include <QObject>
#include <QVector>

class dk_control
{
public:
    dk_control(WorldMap *worldMap);

    void setDPlayer(Player *player);
    void intercept0(Player *Player3);
    void defensePass0(Player *defensePlayer);
    void defensePosition0();
    void intercept4(Player *Player2);
    void defensePass4(Player *defensePlayer);
    void defensePosition4();


private:
    Player *_dPlayer;
    WorldMap *_worldMap;

};

#endif // DK_CONTROL_H
