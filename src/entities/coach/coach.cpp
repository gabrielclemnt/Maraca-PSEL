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

void Coach::runCoach() {

    QVector2D ballPosition = getWorldMap() -> ballPosition();
    std::optional playerOpt = getPlayer(BLUE,3);
    if(playerOpt.has_value()){ //verifica se playerOpt tem um valor player: será o ponteiro para o jogador
        Player *player = playerOpt.value();
        if (player -> getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS)){ //verifica se player está próximo da bola
            player -> goTo(ballPosition); //move o jogador até a bola
            player -> rotateTo(getWorldMap()->ourGoalCenter()); //faz o jogador girar até o centro do gol
            player -> dribble(false);
            player -> kick (8.0f, true);
        } else if (player -> getPosition().distanceToPoint(ballPosition) <= ((2* BALL_DIAMETER) + (2 * ROBOT_DIAMETER))){ //verifica se o jogador está numa posição intermediária entre a bola e o gol
            QVector2D behindBall = ballPosition + ((ballPosition - getWorldMap() -> theirGoalCenter()).normalized()*(ROBOT_RADIUS + BALL_RADIUS)); //calcula uma posição atrás da bola entre a bola e o centro do gol
            player -> goTo(behindBall); //move o jogador para a posição calculada da bola
            player -> rotateTo(getWorldMap()->theirGoalCenter()); //Faz o jogador girar na direção do centro do gol adversário.

        } else { //Se o jogador não estiver nem perto da bola nem em uma posição intermediária.
            if (ballPosition.x() > 0.0f){ //Verifica se a bola está do lado direito do campo.
                player -> goTo(QVector2D(4.0f, 0.0f));
                player -> rotateTo(ballPosition);
            } else{
                player->goTo(ballPosition);
                player->rotateTo(ballPosition);
            }
        }
    }
}
