#include "ak_control.h"
#define ROBOT_DIAMETER (2.0F * ROBOT_RADIUS)
#define BALL_RADIUS 0.0215
#define BALL_DIAMETER (2.0F * BALL_RADIUS)
#define ROBOT_RADIUS 0.09
#define BALL_SPEED 6.0

ak_control::ak_control(WorldMap *worldMap) {
    _worldMap = worldMap;
    _aPlayer = nullptr;
}

void ak_control::setAPlayer (Player *player)
{
    _aPlayer = player;
}

void ak_control::attackposition1()
{
    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        const QVector2D pontoCerto(0.2f, 1.7f);
        _aPlayer->goTo(pontoCerto);
        _aPlayer->rotateTo(ballPosition);
    }
}

void ak_control::attackposition2()
{
    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        const QVector2D pontoCerto(0.2f, -1.7f);
        _aPlayer->goTo(pontoCerto);
        _aPlayer->rotateTo(ballPosition);
    }
}


void ak_control::attack1(Player *Player3){

    if(_aPlayer != nullptr){
    QVector2D ballPosition = _worldMap->ballPosition();
    _aPlayer->dribble(true);

    if(ballPosition.x() > 0.0f && ballPosition.y() >= 0.0){

        _aPlayer->rotateTo(ballPosition);

        if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) *6){
            _aPlayer->goTo(ballPosition);
        }

        if(_aPlayer->getPosition().distanceToPoint(ballPosition)<= (ROBOT_RADIUS + BALL_RADIUS)){
            QVector2D ballPosition = _worldMap->ballPosition();
            const QVector2D golposition(4.4f,0.4f);

            QVector2D targetDirection = golposition - ballPosition; //calcula a direção da bola até o alvo
            _aPlayer->rotateTo(golposition);

            float playerOrientation = _aPlayer->getOrientation();

            float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

            if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                // Chuta a bola
                _aPlayer->kick(6,false);
            }
        }
    }

    }

}

void ak_control::attack2(Player *Player3){

    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        _aPlayer->dribble(true);

        if(ballPosition.x() > 0.0f && ballPosition.y() <= 0.0){

            _aPlayer->rotateTo(ballPosition);

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) *6){
                _aPlayer->goTo(ballPosition);
            }

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<= (ROBOT_RADIUS + BALL_RADIUS)){
                QVector2D ballPosition = _worldMap->ballPosition();
                const QVector2D golposition(4.4f,-0.4f);

                QVector2D targetDirection = golposition - ballPosition; //calcula a direção da bola até o alvo
                _aPlayer->rotateTo(golposition);

                float playerOrientation = _aPlayer->getOrientation();

                float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _aPlayer->kick(6,false);
                }
            }
        }

    }

}

void ak_control ::inicial(){

     if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();

         if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) * 8){
             _aPlayer->goTo(ballPosition);
         }

         if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS)){
             _aPlayer->kick(6.0f, false);
         }

    }

}

void ak_control::attack3(Player *Player3){

    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        _aPlayer->dribble(true);

        if(ballPosition.x() > 0.0f && ballPosition.y() <= 0.0){

            _aPlayer->rotateTo(ballPosition);

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) *6){
                _aPlayer->goTo(ballPosition);
            }

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<= (ROBOT_RADIUS + BALL_RADIUS)){
                QVector2D ballPosition = _worldMap->ballPosition();
                const QVector2D golposition(4.4f,-0.4f);

                QVector2D targetDirection = golposition - ballPosition; //calcula a direção da bola até o alvo
                _aPlayer->rotateTo(golposition);

                float playerOrientation = _aPlayer->getOrientation();

                float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _aPlayer->kick(6,false);
                }
            }
        }

    }

}

