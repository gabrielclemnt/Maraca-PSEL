#include "dk_control.h"
#define ROBOT_DIAMETER (2.0F * ROBOT_RADIUS)
#define BALL_RADIUS 0.0215
#define BALL_DIAMETER (2.0F * BALL_RADIUS)
#define ROBOT_RADIUS 0.09
#define BALL_SPEED 6.0

dk_control::dk_control(WorldMap *worldMap) {

    _worldMap = worldMap;
    _dPlayer = nullptr;
}

void dk_control::setDPlayer (Player *player)
{
    _dPlayer = player;
}

void dk_control::defensePass0(Player *atacplayer)
{
    //faz o passe para um player especifico
    if(_dPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        QVector2D playerBLUE0Position = atacplayer -> getPosition();

        QVector2D targetDirection = playerBLUE0Position - ballPosition; //calcula a direção da bola até o alvo
        _dPlayer->rotateTo(playerBLUE0Position);

        float playerOrientation = _dPlayer->getOrientation();

        float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

        if(_dPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
            // Chuta a bola
            _dPlayer->kick(4,false);
        }
    }
}

void dk_control::intercept0(Player *Player3)
{
    if(_dPlayer != nullptr){

        QVector2D ballPosition = _worldMap->ballPosition();
        _dPlayer->dribble(true);
        //const QVector2D pontoCerto(-2.5f, 0.5f);

        if(ballPosition.x() <= 0.0f && ballPosition.y() >=0.0 && _dPlayer->getPosition().distanceToPoint(ballPosition)>(ROBOT_RADIUS + BALL_RADIUS)* 5){
            // obtém a posição do centro do gol
            QVector2D goalCenter = _worldMap->ourGoalCenter();

            // calcula o vetor do centro do gol até a bola
            QVector2D toBall = ballPosition - goalCenter;
            toBall.normalize();
            // multiplica o vetor por um valor para definir a distância da defesa à bola
            QVector2D targetPosition = goalCenter + toBall * 2;

            // move a defesa para a posição de defesa
            _dPlayer->goTo(targetPosition);
            _dPlayer->rotateTo(ballPosition);

        }
        if(_dPlayer->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS) * 5 && _dPlayer->getPosition().distanceToPoint(ballPosition)>(ROBOT_RADIUS + BALL_RADIUS)){
            _dPlayer->goTo(ballPosition);
            _dPlayer->rotateTo(ballPosition);
        }
        else{

            if(_dPlayer->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS)){
                QVector2D ballPosition = _worldMap->ballPosition();
                QVector2D playerBLUE0Position = Player3 -> getPosition();

                QVector2D targetDirection = playerBLUE0Position - ballPosition; //calcula a direção da bola até o alvo
                _dPlayer->rotateTo(playerBLUE0Position);

                float playerOrientation = _dPlayer->getOrientation();

                float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                if(_dPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _dPlayer->kick(5,false);
                }
            }
        }
    }
}

void dk_control::intercept4(Player *Player2)
{
    if(_dPlayer != nullptr){

        QVector2D ballPosition = _worldMap->ballPosition();
        _dPlayer->dribble(true);
        //const QVector2D pontoCerto(-2.5f, 0.5f);

        if(ballPosition.x() <= 0.0f && ballPosition.y() <= 0.0 && _dPlayer->getPosition().distanceToPoint(ballPosition)>(ROBOT_RADIUS + BALL_RADIUS) *5 ) {
            // obtém a posição do centro do gol
            QVector2D goalCenter = _worldMap->ourGoalCenter();

            // calcula o vetor do centro do gol até a bola
            QVector2D toBall = ballPosition - goalCenter;
            toBall.normalize();
            // multiplica o vetor por um valor para definir a distância da defesa à bola
            QVector2D targetPosition = goalCenter + toBall * 2;

            // move a defesa para a posição de defesa
            _dPlayer->goTo(targetPosition);
            _dPlayer->rotateTo(ballPosition);

        }
        if(_dPlayer->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS) * 5 && _dPlayer->getPosition().distanceToPoint(ballPosition)>(ROBOT_RADIUS + BALL_RADIUS)){
            _dPlayer->goTo(ballPosition);
            _dPlayer->rotateTo(ballPosition);

        }
        else{

                if(_dPlayer->getPosition().distanceToPoint(ballPosition)<=(ROBOT_RADIUS + BALL_RADIUS)){
                    QVector2D ballPosition = _worldMap->ballPosition();
                    QVector2D playerBLUE0Position = Player2 -> getPosition();

                    QVector2D targetDirection = playerBLUE0Position - ballPosition; //calcula a direção da bola até o alvo
                    _dPlayer->rotateTo(playerBLUE0Position);

                    float playerOrientation = _dPlayer->getOrientation();

                    float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

                    if(_dPlayer->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.02){
                    // Chuta a bola
                    _dPlayer->kick(5,false);
                    }
                }
        }
    }
}

void dk_control::defensePosition0()
{
    if(_dPlayer != nullptr){
      QVector2D ballPosition = _worldMap->ballPosition();
      const QVector2D pontoCerto(-2.5f, 0.8f);
      _dPlayer->goTo(pontoCerto);
      _dPlayer->rotateTo(ballPosition);
    }
}

void dk_control::defensePosition4()
{
    if(_dPlayer != nullptr){
        QVector2D ballPosition = _worldMap->ballPosition();
        const QVector2D pontoCerto(-2.5f, -0.8f);
        _dPlayer->goTo(pontoCerto);
        _dPlayer->rotateTo(ballPosition);
    }
}


