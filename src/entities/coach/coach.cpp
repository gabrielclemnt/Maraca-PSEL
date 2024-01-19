/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "coach.h"
#define ROBOT_DIAMETER (2.0F * ROBOT_RADIUS)
#define BALL_RADIUS 0.0215
#define BALL_DIAMETER (2.0F * BALL_RADIUS)
#define ROBOT_RADIUS 0.09
#define BALL_SPEED 3.0
#define SEMICIRCLE_RADIUS 1.5

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap), contador(0), estado(0)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);

    // variavel para armazenar posição e direção da bola
    _lastBallPosition = QVector2D(0,0);
    _ballDirection = QVector2D(0,0);
    _gk_control = new gk_control(worldMap);
    _dk_control = new dk_control(worldMap);
    _ak_control = new ak_control(worldMap);

}

std::optional<Player*> Coach::getPlayer(const bool& isTeamBlue, const quint8& playerId) {
    // Get list of players
    QList<Player*> playersForColor = _players.value(isTeamBlue);

    // Iterate searching for playerId
    for(const auto& player : playersForColor) {
        if(player->getPlayerId() == playerId) {
            return player;
        }
    }

    // If could not found, just return std::nullopt (should trait later)
    return std::nullopt;
}

WorldMap* Coach::getWorldMap() {
    return _worldMap;
}

void Coach::updateDataBall() {

    // calcula a direção da bola
    _ballDirection = getWorldMap()->ballPosition() - _lastBallPosition;
    // atualização da posição da bola
    _lastBallPosition = getWorldMap()->ballPosition();


}

float distanceToSegment(const QVector2D &point, const QVector2D &segmentStart, const QVector2D &segmentEnd) {
    QVector2D segment = segmentEnd - segmentStart;
    QVector2D pointToStart = point - segmentStart;
    float t = QVector2D::dotProduct(pointToStart, segment) / segment.lengthSquared();

    if (t < 0.0f) {
        return pointToStart.length(); // ponto mais próximo é o início do segmento
    } else if (t > 1.0f) {
        return (point - segmentEnd).length(); // ponto mais próximo é o final do segmento
    } else {
        QVector2D closestPoint = segmentStart + t * segment;
        return (point - closestPoint).length(); // ponto mais próximo está no meio do segmento
    }
}

QVector2D Coach::findClosestObstacle(const QVector2D& start, const QVector2D& end, const QList<QVector2D>& obstacles) {
    QVector2D closestObstacle = QVector2D(getWorldMap()->maxX(), getWorldMap()->maxY());
    float minDistance = std::numeric_limits<float>::max(); //rastrear a menor distância encontrada durante a iteração pelos obstáculos

    for (const auto& obstacle : obstacles) {
        float distance = distanceToSegment(start, end, obstacle);

        if (distance <= (ROBOT_RADIUS + BALL_RADIUS) * 2.5f && distance < minDistance) {
            closestObstacle = obstacle;
            minDistance = distance;
        }
    }

    return closestObstacle;
}

void Coach::avoidObstacle(Player *player) {
    QVector2D robotPosition = player->getPosition();
    QVector2D ballPosition = getWorldMap()->ballPosition();
    const QVector2D targetPoint(4.5f, 0.0f);
    //obtém a posição de todos os robôs amarelos
    QList<QVector2D> yellowPositions = getYellowPositions();
    QVector2D obstaclePosition = QVector2D (getWorldMap()->maxX(),getWorldMap()->maxY()) ;

    //todos os robôs amarelos
    for (const auto& yellowPosition : yellowPositions) {

        if(distanceToSegment(yellowPosition,player->getPosition(),targetPoint) >= ROBOT_DIAMETER * 2){
            yellowPositions.removeOne(yellowPosition);
        }
    }
    // filtra os robôs que são obstáculos para o ponto desejado

    obstaclePosition = findClosestObstacle(player->getPosition(), targetPoint, yellowPositions);
    // spdlog :: info("{}",yellowPositions.length());
    if ((distanceToSegment(obstaclePosition, robotPosition, targetPoint) <= (ROBOT_DIAMETER + ROBOT_DIAMETER)) || robotPosition.distanceToPoint(obstaclePosition) <= 0.5f) {
        //const QVector2D targetPoint(4.5f, 0.0f);
        //calcula a direção para desviar do robô amarelo
        // spdlog :: info("testando");
        QVector2D avoidanceDirection = (robotPosition  - obstaclePosition * 3).normalized(); // ajustar para virar 90º
        QVector2D adjustedDirection(-avoidanceDirection.y(), avoidanceDirection.x());
        QVector2D newRobotPosition = obstaclePosition + adjustedDirection * (ROBOT_DIAMETER + 3.0F) * 2.0F; //adjustedDirection
        //atualiza a posição do robô
        player->goTo(newRobotPosition);

        //spdlog :: info("({},{})", newRobotPosition.x(), newRobotPosition.y());
    }
    else{
        player->goTo(ballPosition);
        // player->stop();
    }
    player->rotateTo(targetPoint);
}

QList<QVector2D> Coach::getYellowPositions() {
    QList<QVector2D> yellowPositions;
    for (int i = 0; i < 6; i++) {
        std::optional<Player*> yellowRobotOpt = getPlayer(YELLOW, i);

        if (yellowRobotOpt.has_value()) {
            yellowPositions.append(yellowRobotOpt.value()->getPosition());
        }
    }

    return yellowPositions;
}

void Coach::runCoach() {

    // if(contador >= 30){
    //     updateDataBall();
    //     //spdlog :: info("testando");
    //     contador = 0;
    // }

    QVector2D ballPosition = getWorldMap()->ballPosition();
    //const QVector2D pontoCerto(3.0f, 2.0f);
    //float distP2Ball = getPlayer(YELLOW,4).value()->getPosition().distanceToPoint(balloPosition);

    // getPlayer(YELLOW,0).value()-> goTo(pontoCerto);
    // getPlayer(YELLOW,0).value()-> rotateTo(balloPosition);
    // getPlayer(BLUE,2).value()->rotateTo(pontoCerto);
    // getPlayer(BLUE,2).value()->kick(3.0,false);
    //player 5 goleiro
    //player 0 defensor
    //player 4 defensor
    //player 3,1,2 atacante

    QMap<quint8, std::optional<Player *>> players;
    for (quint8 playerId = 0; playerId < 6; playerId++){
        players.insert(playerId, getPlayer(BLUE, playerId));
    }

    for (quint8 playerId : players.keys()){

        if(players.value(playerId).has_value()){

            if(playerId == 5){
                _gk_control -> setPlayer(players.value(playerId).value());
                players.value(playerId).value()->rotateTo(ballPosition);
                players.value(playerId).value()->dribble(true);

                if(getWorldMap()->isBallInsideOurPenaltyArea(ballPosition)){
                    players.value(playerId).value()->goTo(ballPosition);

                    if(players.value(playerId).value()->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS)){ //se a bola tiver dentro da area passa
                        _gk_control->pass(getPlayer(BLUE, 0).value());
                        //spdlog :: info("testando");

                    }
                }
                else {
                //defend
                _gk_control -> defend(); //metodo defende
                }

            }

            if (playerId == 0){ //defensor blue 0
                _dk_control -> setDPlayer(players.value(playerId).value());
                players.value(playerId).value()->dribble(true);

                if(ballPosition.x() <= 0.0f && ballPosition.y()>= 0.0f){
                    _dk_control->intercept0(getPlayer(BLUE, 1).value());
                }
                if(ballPosition.x()>= 0.0f){
                    _dk_control->defensePosition0();
                }

            }
            if(playerId == 4){ // defensor blue 4
                _dk_control -> setDPlayer(players.value(playerId).value());
                players.value(playerId).value()->dribble(true);

                if(ballPosition.x() <= 0.0f && ballPosition.y() <= 0.0f){
                    _dk_control->intercept4(getPlayer(BLUE, 2).value());
                }
                if(ballPosition.x()>= 0.0f){
                    _dk_control->defensePosition4();
                }



            }
            if(playerId == 1){ //atacante lateral superior blue 1
                _ak_control -> setAPlayer(players.value(playerId).value());
                players.value(playerId).value()->dribble(true);

                if(ballPosition.x()<= 0.0f){
                    _ak_control->attackposition1();
                }
                if(ballPosition.x()> 0.0f){
                    _ak_control->attack1(getPlayer(BLUE, 3).value());
                }

            }

            if(playerId == 2){
                _ak_control -> setAPlayer(players.value(playerId).value());
                players.value(playerId).value()->dribble(true);

                if(ballPosition.x()<= 0.0f){
                    _ak_control->attackposition2();
                }

                if(ballPosition.x()> 0.0f){
                    _ak_control->attack2(getPlayer(BLUE, 3).value());
                }

            }

            if(playerId == 3){

            _ak_control -> setAPlayer(players.value(playerId).value());
            players.value(playerId).value()->dribble(true);


            if(ballPosition.x() == 0.0f && ballPosition.y() == 0.0f){

               _ak_control->inicial(getPlayer(BLUE, 0).value());

            }

            if(ballPosition.x() < 0.0f && ballPosition.x() > -1.7f){

            _ak_control->attackd(getPlayer(BLUE, 1).value(),getPlayer(BLUE, 2).value());
            }

            if(ballPosition.x() > 0.0f){

                const QVector2D pontoMeia(-0.9f, 0.0f);
                players.value(playerId).value()->goTo(pontoMeia);

            }

            }


        }


    }
}

