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

Coach::Coach(const QMap<bool, QList<Player*>>& players, WorldMap* worldMap)
    : _players(players), _worldMap(worldMap)
{
    // Create QTimer and connects to the runCoach() slot
    _actuatorTimer = new QTimer(this);
    QObject::connect(_actuatorTimer, &QTimer::timeout, this, &Coach::runCoach);
    _actuatorTimer->start(COACH_ITERATION_INTERVAL_MS);
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

//testando
//calcular a distância entre um ponto e um segmento de reta

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

//evita robos amarelos enquanto um jogador se move em direçao a bola
void Coach::avoidObstacle(Player *player) {
    QVector2D robotPosition = player->getPosition();
    QVector2D ballPosition = getWorldMap()->ballPosition();

    //obtém a posição de todos os robôs amarelos
    QList<QVector2D> yellowPositions = getYellowPositions();
    QVector2D obstaclePosition = QVector2D (getWorldMap()->maxX(),getWorldMap()->maxY()) ;
    // Todos os robôs amarelos
    for (const auto& yellowPosition : yellowPositions) {
        //verifica se o robo amarelo está próximo do segmento de reta entre o robo e a bolaa
        if(yellowPosition.distanceToPoint(player->getPosition())<= obstaclePosition.distanceToPoint(player->getPosition())){
            obstaclePosition = yellowPosition;
        }
    }
    if (distanceToSegment(robotPosition, ballPosition, obstaclePosition) <=  ROBOT_RADIUS + ROBOT_RADIUS) {
        const QVector2D targetPoint(5.0f, 0.0f);
        //calcula a direção para desviar do robô amarelo
        // spdlog :: info("testando");
        QVector2D avoidanceDirection = (robotPosition  - obstaclePosition * 3).normalized(); // ajustar para virar 90º
        QVector2D adjustedDirection(-avoidanceDirection.y(), avoidanceDirection.x());
        //move o robô para o lado
        QVector2D newRobotPosition = obstaclePosition - adjustedDirection * (ROBOT_RADIUS + BALL_RADIUS) * -2.0f;
        spdlog :: info("({},{})", newRobotPosition.x(), newRobotPosition.y());
        //atualiza a posição do robô
        player->goTo(newRobotPosition);
        player->rotateTo(targetPoint);
    }
}
//ajustar 90º

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

    const float avoidanceDistance = ROBOT_RADIUS + BALL_RADIUS;
    const float largerAvoidanceRadius = ROBOT_RADIUS + BALL_RADIUS + 2.0f;
    const float dribbleAvoidanceDistance = ROBOT_RADIUS + ROBOT_RADIUS * 1.5f;
    const QVector2D targetPoint(5.0f, 0.0f);
    QVector2D ballPosition = getWorldMap()->ballPosition();
    std::optional<Player*> playerOpt = getPlayer(BLUE, 3);

    if (playerOpt.has_value()) {

        Player *player = playerOpt.value();
        // player->goTo(ballPosition);
        // player->rotateTo(ballPosition);
        //player->rotateTo(getWorldMap()->theirGoalCenter());
        player -> dribble(true);

        //verifica se o robô está próximo da bola
        if (player -> getPosition().distanceToPoint(ballPosition) <= (ROBOT_DIAMETER + BALL_RADIUS )){

            // player->goTo(ballPosition);
            // player->rotateTo(targetPoint);
            // player -> dribble(true);
            // player-> goTo(targetPoint);
            // spdlog :: info("testando");
                if (player->getPosition().distanceToPoint(ballPosition) <= avoidanceDistance) {
                    //verifica se há robôs amarelos na frente durante o caminho até targetPoint

                    bool obstacleDetected = false;
                    for (const auto& yellowPosition : getYellowPositions()) {
                        if (player->getPosition().distanceToPoint(yellowPosition) <= largerAvoidanceRadius) {
                        //desvia do robô amarelo com um raio maior
                        QVector2D avoidanceDirection = (yellowPosition - player->getPosition()).normalized();
                        // player->goTo(player->getPosition() + avoidanceDirection * (largerAvoidanceRadius * 2.0f));
                        obstacleDetected = true;
                        }
                    }
                    if (obstacleDetected) {
                        // spdlog :: info("testando");
                        //vai para o ponto (5.0f, 0.0f)
                        // player->goTo(targetPoint);
                        //verifica se o robô está se aproximando de um robô amarelo durante o caminhoasa
                        // for (const auto& yellowPosition : getYellowPositions()) {
                        //     if (player->getPosition().distanceToPoint(yellowPosition) <= dribbleAvoidanceDistance) {
                        //      //desvia do robô amarelo
                        //      QVector2D avoidanceDirection = (yellowPosition - player->getPosition()).normalized();
                        //      player->goTo(player->getPosition() + avoidanceDirection * dribbleAvoidanceDistance);
                        //      player->rotateTo(ballPosition);
                        //      //break; //para de verificar se um obstáculo foi detectados
                        //     }
                        // }
                        avoidObstacle(player);
                        //player->stop();
                        //player->goTo(ballPosition);

                     }

                }
                else{
                    player-> goTo(targetPoint);
                    player->rotateTo(targetPoint);
                }
        }
        else {
            //se não estiver próximo da bola, vai para a bola
            player->goTo(ballPosition);
            player->rotateTo(ballPosition);

        }
    }
}

// spdlog :: info("{}",obstacleDetected)
