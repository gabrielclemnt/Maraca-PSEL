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

QVector2D Coach::calculaSemiCircle(const QVector2D& ballPosition, float radius) {

    //pega o centro do gol
    QVector2D goalCenter = getWorldMap()->ourGoalCenter();

    //calcula o angulo do semi circulo
    float angle = atan2(ballPosition.y() - goalCenter.y(), ballPosition.x() - goalCenter.x());

    // calcula o x e y do target position
    float x = radius * cos(angle);
    float y = radius * sin(angle);

    // retorna target position
    return QVector2D(goalCenter.x() + x, goalCenter.y() + y);

}

int haskicked = 0;
int var= 0;
int gol = 0;

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

                if(players.value(playerId).value()->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS)){ //se a bola tiver dentro da area passa
                    _gk_control->pass(getPlayer(BLUE, 0).value());
                    //spdlog :: info("testando");

            } else {
                //defend
                _gk_control -> defend(); //metodo defende
            }

        } else if (playerId == 0){
            players.value(playerId).value()->rotateTo(ballPosition);
        } else{

        }
    }
  }
}

