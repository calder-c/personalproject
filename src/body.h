#include "definitions.h"
#include "include.h"
class Point {
public:
    sf::Vector2f currentPos, oldPos, acc;
    sf::CircleShape shape;
    float radius;
    Point(sf::Vector2f currentPos_, sf::Vector2f oldPos_, sf::Vector2f acc_, float radius_) : currentPos(currentPos_), oldPos(oldPos_), acc(acc_), radius(radius_){
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
    }
    Point(sf::Vector2f pos_, sf::Vector2f acc_, float radius_) : currentPos(pos_), oldPos(pos_), acc(acc_), radius(radius_){
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
    }
    void freeze() {
        oldPos = currentPos;
        acc = sf::Vector2f(0, 0);
    }
    void update(float dt) {
        sf::Vector2f buf = currentPos;
        sf::Vector2f diffPos = (currentPos - oldPos);
        currentPos = currentPos + diffPos + acc*(dt*dt);
        oldPos = buf;
        shape.setPosition(currentPos);
    }
    void applyScreenConstraint() {
        sf::Vector2f diffPos = (currentPos - oldPos);
        if (currentPos.x < 0) {
            currentPos.x = 0;
            oldPos.x = (currentPos + diffPos).x;
        } else if (currentPos.x > SCREEN_WIDTH) {
            currentPos.x = SCREEN_WIDTH;
            oldPos.x = (currentPos + diffPos).x;
        }else if (currentPos.y < 0) {
            currentPos.y = 0;
            oldPos.y = (currentPos + diffPos).y;
        } else if (currentPos.y > SCREEN_HEIGHT) {
            currentPos.y = SCREEN_HEIGHT;
            oldPos.y = (currentPos + diffPos).y;
        }

    }
    void draw(sf::RenderWindow & window) {

        window.draw(shape);
    }

};

