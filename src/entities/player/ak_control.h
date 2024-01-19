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

    void attackposition1();
    void attackposition2();
    void attack1(Player *player3);
    void attack2(Player *player3);
    void attack3(Player *player3);
    void attackd();
    void attacka();
    void inicial();


    //talvez implemente desvio
    QList<QVector2D> getYellowPositions();
    void avoidObstacle(Player *player);
    QVector2D findClosestObstacle(const QVector2D& start, const QVector2D& end, const QList<QVector2D>& obstacles);


private:
    Player *_aPlayer;
    WorldMap *_worldMap;
};

#endif // AK_CONTROL_H
