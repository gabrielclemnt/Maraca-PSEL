#include "gk_control.h"
#define ROBOT_DIAMETER (2.0F * ROBOT_RADIUS)
#define BALL_RADIUS 0.0215
#define BALL_DIAMETER (2.0F * BALL_RADIUS)
#define ROBOT_RADIUS 0.09
#define BALL_SPEED 3.0


gk_control::gk_control(WorldMap *worldMap)
{
    _worldMap = worldMap;
    _player = nullptr;
}

void gk_control::setPlayer (Player *player)
{
    _player = player;
}

void gk_control::defend()
{
    if(_player != nullptr){

        // obtém a posição do centro do gol
        QVector2D goalCenter = _worldMap->ourGoalCenter();
        // obtém a posição da bola
        QVector2D ballPosition = _worldMap->ballPosition();

        // calcula o vetor do centro do gol até a bola
        QVector2D toBall = ballPosition - goalCenter;
        toBall.normalize();
        // multiplica o vetor por um valor para definir a distância do goleiro à bola
        QVector2D targetPosition = goalCenter + toBall * 0.5;

        // move o goleiro para a posição de defesa
        _player->goTo(targetPosition);
    }
}

void gk_control::pass(Player *player5)
{
    //faz o passe para um player especifico
    if(_player != nullptr){
            QVector2D ballPosition = _worldMap->ballPosition();
            QVector2D playerBLUE0Position = player5 -> getPosition();

            QVector2D targetDirection = playerBLUE0Position - ballPosition; //calcula a direção da bola até o alvo
            _player->rotateTo(playerBLUE0Position);

            float playerOrientation = _player->getOrientation();

            float targetDirectionAngle = atan2(targetDirection.y(), targetDirection.x());

            if(_player->getPosition().distanceToPoint(ballPosition) <= (ROBOT_RADIUS + BALL_RADIUS) &&  abs(playerOrientation - targetDirectionAngle) < 0.05){
                // Chuta a bola
                _player->kick(4,false);
            }
        }
}

