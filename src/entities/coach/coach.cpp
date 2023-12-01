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
    QVector2D ballPosition = getWorldMap()->ballPosition();
    std::optional<Player*> playerOpt = getPlayer(BLUE, 3);

    if (playerOpt.has_value()) {
        Player *player = playerOpt.value();

        static bool hasKicked = false;  // Adicionando variável para rastrear se o robô já chutou

        if (!hasKicked && player->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS)) {
            player->goTo(ballPosition);
            player->rotateTo(getWorldMap()->ourGoalCenter());
            player->dribble(false);
            player->kick(8.0f, true);

            hasKicked = true;  // Marcar que o robô já chutou
        } else if (hasKicked && player->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS)) {
            player->stop();  // Parar o robô após o chute
        } else if (player->getPosition().distanceToPoint(ballPosition) <= ((2 * BALL_DIAMETER) + (2 * ROBOT_DIAMETER))) {
            QVector2D behindBall = ballPosition + ((ballPosition - getWorldMap()->theirGoalCenter()).normalized() * (ROBOT_RADIUS + BALL_RADIUS));
            player->goTo(behindBall);
            player->rotateTo(getWorldMap()->theirGoalCenter());
        } else {
            if (ballPosition.x() > 0.0f) {
                player->goTo(QVector2D(4.0f, 0.0f));
                player->rotateTo(ballPosition);
            } else {
                player->goTo(ballPosition);
                player->rotateTo(ballPosition);
            }
        }
    }
}
