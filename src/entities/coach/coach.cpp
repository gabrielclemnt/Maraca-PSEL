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
#define BALL_SPEED 3.0;

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap), contador(0)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);

    // variavel para armazenar posição e direção da bola
    _lastBallPosition = QVector2D(0,0);
    _ballDirection = QVector2D(0,0);

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

int estado = 0;
int haskicked = 0;
int var= 0;

void Coach::runCoach() {

    if(contador >= 30){
        updateDataBall();
        spdlog :: info("testando");
        contador = 0;
    }

    QVector2D balloPosition = getWorldMap()->ballPosition();
    // QVector2D previousBallPosition = ballPosition;

    const QVector2D pontoCerto(3.0f, 2.0f);
    //const QVector2D pontoteste1(2.4f, 1.6f);

    // float distP0Ball = getPlayer(BLUE,2).value()->getPosition().distanceToPoint(ballPosition);
    // float distP1Ball = getPlayer(YELLOW,0).value()->getPosition().distanceToPoint(ballPosition);
    float distP2Ball = getPlayer(YELLOW,4).value()->getPosition().distanceToPoint(balloPosition);

    getPlayer(YELLOW,0).value()-> goTo(pontoCerto);
    getPlayer(YELLOW,0).value()-> rotateTo(balloPosition);
    getPlayer(BLUE,2).value()->rotateTo(pontoCerto);
    getPlayer(BLUE,2).value()->kick(3.0,false);


    if (estado == 0){ //estado inicial player0 vai para bola player2 vai para o target

        getPlayer(BLUE,2).value()->goTo(balloPosition);
        getPlayer(BLUE,2).value()->dribble(true);


        if(getPlayer(BLUE,2).value()->getPosition().distanceToPoint(balloPosition) <= (BALL_RADIUS +  ROBOT_DIAMETER)){

            getPlayer(BLUE,2).value()->kick(3.0,false);

            while(var < 10){
            getPlayer(BLUE,2).value()->kick(3.0,false);
            var++;
            }
            if(var == 10 && haskicked == 0){
            estado = 1;
            }
        }

    }

    if (estado == 1 && getPlayer(BLUE,2).value()->getPosition().distanceToPoint(balloPosition) >= (ROBOT_DIAMETER+ ROBOT_DIAMETER)){

        //spdlog :: info("testando");
        if(haskicked == 0){
            haskicked = 1;
        if (distP2Ball >= 0.3f && haskicked == 1){

            estado = 2;

        }
        }
    }

    if(estado == 2){


        //spdlog::info("ballDirection ({}, {})", _ballDirection.x(), _ballDirection.y());

        //float distanceToIntercept = redimensionarDistancia(_worldMap->ballPosition(), _worldMap->ballVelocity(), _worldMap->goalPosition());
        float distanceToIntercept = 0.8;
        QVector2D desiredPosition = _worldMap->ballPosition() + _ballDirection * distanceToIntercept;

        //spdlog :: info("testando");
        Player *p = getPlayer(YELLOW, 4).value();
        p->goTo(desiredPosition);
        p->rotateTo(_worldMap->ballPosition());
    }
    contador++;
 }



