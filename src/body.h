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
    Point() {
        radius=1;
        e=1;
        currentPos = sf::Vector2f(0,0);
        oldPos = sf::Vector2f(0,0);
        acc = sf::Vector2f(0,0);
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
        shape.setPosition(currentPos);
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
    LineConstraint(){}
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
class Square {
public:
    LineConstraint l1, l2, l3, l4, l5;
    Point* p1;
    Point* p2;
    Point* p3;
    Point* p4;
    sf::Vector2f centerPos;
    float sideLength;
    float rotation;
    float stiffness;
        Square(sf::Vector2f centerPos_, std::vector<LineConstraint>& lineList, std::vector<Point*>& objList, float sideLength_, float stiffness_, float rotation_) {
            stiffness = stiffness_;
            centerPos = centerPos_;
            sideLength = sideLength_;
            rotation = rotation_;
            sf::Vector2f sideVector{sideLength, sideLength};
            sf::Vector2f sideHalfVector{sideLength/2, sideLength/2};
            p1 = new Point{(centerPos-sideHalfVector), (centerPos-sideHalfVector), sf::Vector2f(0,0), 1, 1};
            p2 = new Point{sf::Vector2f(centerPos.x+sideHalfVector.x, centerPos.y-sideHalfVector.y), sf::Vector2f(centerPos.x+sideHalfVector.x, centerPos.y-sideHalfVector.y), sf::Vector2f(0,0), 1, 1};
            p3 = new Point{sf::Vector2f(p2->currentPos.x, p2->currentPos.y + sideLength), sf::Vector2f(p2->currentPos.x, p2->currentPos.y + sideLength), sf::Vector2f(0, 0), 1, 1};
            p4 = new Point{sf::Vector2f(p1->currentPos.x, p1->currentPos.y+sideLength), sf::Vector2f(p1->currentPos.x, p1->currentPos.y+sideLength), sf::Vector2f(0, 0), 1, 1};
            p1->currentPos = rotateAroundPoint(p1->currentPos, centerPos, rotation);
            p1->oldPos = p1->currentPos;
            p2->currentPos = rotateAroundPoint(p2->currentPos, centerPos, rotation);
            p2->oldPos = p2->currentPos;
            p3->currentPos = rotateAroundPoint(p3->currentPos, centerPos, rotation);
            p3->oldPos = p3->currentPos;
            p4->currentPos = rotateAroundPoint(p4->currentPos, centerPos, rotation);
            p4->oldPos = p4->currentPos;
            l1 = LineConstraint{p1, p2, stiffness};
            l2 = LineConstraint{p2, p3, stiffness};
            l3 = LineConstraint{p3, p4, stiffness};
            l4 = LineConstraint{p4, p1, stiffness};
            l5 = LineConstraint{p3, p1, stiffness};
            objList.push_back(p1);
            objList.push_back(p2);
            objList.push_back(p3);
            objList.push_back(p4);
            lineList.push_back(l1);
            lineList.push_back(l2);
            lineList.push_back(l3);
            lineList.push_back(l4);
            lineList.push_back(l5);



        // p1.currentPos = centerPos - sideHalfVector;
        // p1.oldPos = p1.currentPos;

        // p2.currentPos = p1.currentPos + sideVector;
        // p2.oldPos = p2.currentPos;

        // p3.currentPos.y = p2.currentPos.y + sideLength;
        // p3.currentPos.x = p2.currentPos.x;
        // p3.oldPos = p3.currentPos;

        // p4.currentPos.y = p1.currentPos.y + sideLength;
        // p4.oldPos = p4.currentPos;


    }
};