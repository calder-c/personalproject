#include "definitions.h"
#include "include.h"
#include "utils.h"

class Point {
public:
    sf::Vector2f currentPos, oldPos, acc;
    sf::CircleShape shape;
    float radius;
    float e;
    Point(sf::Vector2f currentPos_, sf::Vector2f oldPos_, sf::Vector2f acc_, float radius_, float e_) : currentPos(currentPos_), oldPos(oldPos_), acc(acc_), radius(radius_), e(e_){
        shape.setRadius(radius);
        shape.setOrigin(sf::Vector2f(radius, radius));
    }
    Point(sf::Vector2f pos_, sf::Vector2f acc_, float radius_, float e_) : currentPos(pos_), oldPos(pos_), acc(acc_), radius(radius_), e(e_){
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
            oldPos.x = (currentPos + diffPos*e).x;
        } else if (currentPos.x > SCREEN_WIDTH) {
            currentPos.x = SCREEN_WIDTH;
            oldPos.x = (currentPos + diffPos*e).x;
        }else if (currentPos.y < 0) {
            currentPos.y = 0;
            oldPos.y = (currentPos + diffPos*e).y;
        } else if (currentPos.y > SCREEN_HEIGHT) {
            currentPos.y = SCREEN_HEIGHT;
            oldPos.y = (currentPos + diffPos*e).y;
        }

    }
    void draw(sf::RenderWindow & window) {

        window.draw(shape);
    }

};
class LineConstraint {
public:
    Point * p0;
    Point * p1;
    float length, stiffness;
    LineConstraint(Point* p0_, Point* p1_, float stiffness_) : p0(p0_), p1(p1_), stiffness(stiffness_) {
        length = distance2f(p0->currentPos, p1->currentPos);
    }
    LineConstraint(Point* p0_, Point* p1_, float length_, float stiffness_) : p0(p0_), p1(p1_), length(length_), stiffness(stiffness_) {}
    void draw(sf::RenderWindow & window) {
        std::array<sf::Vertex, 2> line =
        {
            sf::Vertex{p0->currentPos, sf::Color::Blue,p0->currentPos },
            sf::Vertex{p1->currentPos, sf::Color::Blue,p1->currentPos }
        };
        window.draw(p0->shape);
        window.draw(p1->shape);
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);

    }
    void applyLineConstraint() {
        float dx = p1->currentPos.x - p0->currentPos.x;
        float dy = p1->currentPos.y - p0->currentPos.y;
        float dist = sqrt(dx*dx + dy*dy);
        float diff = length - dist;
        float percent = (diff/dist)/2;
        float offset_x = dx*percent*stiffness;
        float offset_y = dy*percent*stiffness;
        p0->currentPos.x -= offset_x;
        p0->currentPos.y -= offset_y;
        p1->currentPos.x += offset_x;
        p1->currentPos.y += offset_y;
    }
    void update(float dt) {
        p0->update(dt);
        p1->update(dt);
        applyLineConstraint();
        p0->applyScreenConstraint();
        p1->applyScreenConstraint();





    }
};
