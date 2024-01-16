﻿/***
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


#include "worldmap.h"

WorldMap::WorldMap(const bool& isPlayingLeft) : _isPlayingLeft(isPlayingLeft)
{

}

bool WorldMap::playingLeftSide() const {
    return _isPlayingLeft;
}

float WorldMap::minX() const {
    return -length()/2.0f;
}

float WorldMap::maxX() const {
    return length()/2.0f;
}

float WorldMap::minY() const {
    return -width()/2.0f;
}

float WorldMap::maxY() const {
    return width()/2.0f;
}

float WorldMap::length() const {
    return _field.field().field_length() / 1000.0f;
}

float WorldMap::width() const {
    return _field.field().field_width() / 1000.0f;
}

float WorldMap::goalDepth() const {
    return _field.field().goal_depth() / 1000.0f;
}

float WorldMap::goalWidth() const {
    return _field.field().goal_width() / 1000.0f;
}

float WorldMap::penaltyDepth() const {
    return 2.0f;
}

float WorldMap::penaltyWidth() const {
    return 1.0f;
}

float WorldMap::penaltyMarkDistanceFromGoal() const {
    return 0.1125f;
}

float WorldMap::centerRadius() const {
    return 0.25f;
}

QVector2D WorldMap::ourGoalCenter() const {
    if(playingLeftSide()) {
        return QVector2D(-length()/2.0f, 0.0f);
    }
    else {
        return QVector2D(length()/2.0f, 0.0f);
    }
}

QVector2D WorldMap::ourGoalLeftPost() const {
    QVector2D goalCenter = ourGoalCenter();
    float offset = (playingLeftSide() ? -goalWidth()/2.0f : goalWidth()/2.0f);
    return QVector2D(goalCenter.x(), goalCenter.y() + offset);
}

QVector2D WorldMap::ourGoalRightPost() const {
    QVector2D goalCenter = ourGoalCenter();
    float offset = (playingLeftSide() ? goalWidth()/2.0f : -goalWidth()/2.0f);
    return QVector2D(goalCenter.x(), goalCenter.y() + offset);
}

QVector2D WorldMap::ourPenaltyMark() const {
    QVector2D goalCenter = theirGoalCenter();
    float offset = (playingLeftSide() ? -penaltyMarkDistanceFromGoal() : penaltyMarkDistanceFromGoal());
    return QVector2D(goalCenter.x() - offset, goalCenter.y());
}

QVector2D WorldMap::theirGoalCenter() const {
    // Take our goal center and just reflect the x coordinate
    QVector2D ourCenter = ourGoalCenter();
    return QVector2D(-ourCenter.x(), ourCenter.y());
}

QVector2D WorldMap::theirGoalLeftPost() const {
    QVector2D goalCenter = theirGoalCenter();
    float offset = (playingLeftSide() ? goalWidth()/2.0f : -goalWidth()/2.0f);
    return QVector2D(goalCenter.x(), goalCenter.y() + offset);
}

QVector2D WorldMap::theirGoalRightPost() const {
    QVector2D goalCenter = theirGoalCenter();
    float offset = (playingLeftSide() ? -goalWidth()/2.0f : goalWidth()/2.0f);
    return QVector2D(goalCenter.x(), goalCenter.y() + offset);
}

QVector2D WorldMap::theirPenaltyMark() const {
    QVector2D goalCenter = ourGoalCenter();
    float offset = (playingLeftSide() ? penaltyMarkDistanceFromGoal() : -penaltyMarkDistanceFromGoal());
    return QVector2D(goalCenter.x() - offset, goalCenter.y());
}

QVector2D WorldMap::ballPosition() const {
    return QVector2D(_ball.x() / 1000.0f, _ball.y() / 1000.0f);
}

bool WorldMap::isBallInsideOurPenaltyArea(QVector2D position) const{

    if(!playingLeftSide()){
        QVector2D topLeft(ourGoalCenter().x() - penaltyWidth(), penaltyDepth() / 2);
        QVector2D bottomRight(ourGoalCenter().x(), -(penaltyDepth() / 2));

        if(position.x() >= topLeft.x() && position.x() <= bottomRight.x()){
            if(position.y() <= topLeft.y() && position.y() >= bottomRight.y()){
                return true;
            }
        }
        return false;
    } else{
        QVector2D topLeft(ourGoalCenter().x(), penaltyDepth() / 2);
        QVector2D bottomRight(ourGoalCenter().x() + penaltyWidth(), -(penaltyDepth() / 2));

        if(position.x() >= topLeft.x() && position.x()<= bottomRight.x()){
            if(position.y() <= topLeft.y() && position.y() >= bottomRight.y()){
                return true;
            }
        }
        return false;
    }
}

bool WorldMap :: isBallInsideTheirPenaltyArea(QVector2D position) const{

    if(playingLeftSide()){
        QVector2D topLeft(theirGoalCenter().x() - penaltyWidth(), penaltyDepth() / 2);
        QVector2D bottomRight(theirGoalCenter().x(), -(penaltyDepth() / 2));

        if(position.x() >= topLeft.x() && position.x() <= bottomRight.x()){
            if(position.y() <= topLeft.y() && position.y() >= bottomRight.y()){
                return true;
            }
        }
        return false;
    } else{
        QVector2D topLeft(theirGoalCenter().x(), penaltyDepth() / 2);
        QVector2D bottomRight(theirGoalCenter().x() + penaltyWidth(), -(penaltyDepth() / 2));

        if(position.x() >= topLeft.x() && position.x()<= bottomRight.x()){
            if(position.y() <= topLeft.y() && position.y() >= bottomRight.y()){
                return true;
            }
        }
        return false;
    }
}

void WorldMap::updateBallDetection(const SSL_DetectionBall &ball)
{
    _ball = ball;
}

void WorldMap::updateFieldDetection(const SSL_GeometryData &field)
{
    _field = field;
}

