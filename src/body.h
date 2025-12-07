#include <complex>

#include "definitions.h"
#include "include.h"
#include "utils.h"
class Point {
public:
    sf::Vector2f currentPos, oldPos, acc;
    sf::CircleShape shape;
    float radius;
    Point(sf::Vector2f currentPos_, sf::Vector2f oldPos_, sf::Vector2f acc_, float radius_) : currentPos(currentPos_), oldPos(oldPos_), acc(acc_), radius(radius_){
        shape.setRadius(radius);
    }
    void update(float dt) {
        sf::Vector2f buf = currentPos;
        currentPos = currentPos + (currentPos - oldPos) + acc*(dt*dt);
        oldPos = buf;
        shape.setPosition(currentPos);
    }
    void draw(sf::RenderWindow & window) {

        window.draw(shape);
    }
};
