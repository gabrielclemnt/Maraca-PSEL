#ifndef AK_CONTROL_H
#define AK_CONTROL_H

#include <QMap>
#include <src/entities/player/player.h>
#include <src/entities/worldmap/worldmap.h>
#include <math.h>
#include <QVector2D>
#include <QObject>
#include <QVector>

class ak_control
{
public:
    ak_control(WorldMap *worldMap);
    void setAPlayer(Player *player);

    void atackposition1();
    void atackposition2();


private:
    Player *_aPlayer;
    WorldMap *_worldMap;
};

#endif // AK_CONTROL_H
