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
        _aPlayer->rotateTo(ballPosition); //
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

void ak_control ::inicial(Player *player0){

     if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();

         if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) * 8){
            _aPlayer->rotateTo(ballPosition);
            _aPlayer->goTo(ballPosition);
            _aPlayer->kick(4,false);
         }

         if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS)){
             QVector2D ballPosition = _worldMap->ballPosition();
             QVector2D playerBLUE0Position = player0 -> getPosition();

             QVector2D targetDirection = playerBLUE0Position - ballPosition; //calcula a direção da bola até o alvo
             _aPlayer->rotateTo(playerBLUE0Position);

             float playerOrientation = _aPlayer->getOrientation();

             float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

             if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                 // Chuta a bola
                 _aPlayer->kick(4,false);
             }
         }

    }

}

void ak_control::attackd(Player *Player1, Player *Player2){

    if(_aPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        _aPlayer->dribble(true);

        if(ballPosition.x() < 0.0f && ballPosition.x() >= -1.7f && ballPosition.y() <= 3.0f && ballPosition.y() >= 0.0f){


            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) *20){
                _aPlayer->goTo(ballPosition);
                _aPlayer->rotateTo(ballPosition);
            }

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<= (ROBOT_RADIUS + BALL_RADIUS)){

                QVector2D playerBLUE1Position = Player1 -> getPosition();

                QVector2D targetDirection = playerBLUE1Position  - ballPosition; //calcula a direção da bola até o alvo
                _aPlayer->rotateTo(playerBLUE1Position );

                float playerOrientation = _aPlayer->getOrientation();

                float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _aPlayer->kick(5,false);
                }
            }
        }

        if(ballPosition.x() < 0.0f && ballPosition.x() >= -1.7f && ballPosition.y() < 0.0f && ballPosition.y() >= -3.0f){


            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<(ROBOT_RADIUS + BALL_RADIUS) *20){
                _aPlayer->goTo(ballPosition);
                _aPlayer->rotateTo(ballPosition);
            }

            if(_aPlayer->getPosition().distanceToPoint(ballPosition)<= (ROBOT_RADIUS + BALL_RADIUS)){

                QVector2D playerBLUE2Position = Player2 -> getPosition();

                QVector2D targetDirection = playerBLUE2Position  - ballPosition; //calcula a direção da bola até o alvo
                _aPlayer->rotateTo(playerBLUE2Position );

                float playerOrientation = _aPlayer->getOrientation();

                float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                if(_aPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _aPlayer->kick(5,false);
                }
            }
        }
    }

}

