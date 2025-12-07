#include <complex>

#include "definitions.h"
#include "include.h"
#include "utils.h"


class Point {
public:
    sf::Vector2f pos, vel, acc;
    sf::CircleShape shape;
    float radius;
    Point(sf::Vector2f pos_, sf::Vector2f vel_, sf::Vector2f acc_, float radius_) : pos(pos_), vel(vel_), acc(acc_), radius(radius_){
        shape.setRadius(radius);
    }
    void update(float dt) {
        vel = vel + acc * dt;
        pos = pos + vel * dt;
        shape.setPosition(pos);
    }
    void draw(sf::RenderWindow & window) {

        window.draw(shape);
    }
};
