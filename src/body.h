#include "definitions.h"
#include "include.h"
#include "utils.h"

class Point {
public:
    sf::Vector2f currentPos, oldPos, acc;
    sf::CircleShape shape;
    float radius;
    float e;
    float mass;
    bool shouldDraw = false;
    int lifetime;
    Point(sf::Vector2f pos_, sf::Vector2f acc_, float radius_, float e_, float mass_) : currentPos(pos_), oldPos(pos_), acc(acc_), radius(radius_), e(e_), lifetime(0), mass(mass_){
    }

    void freeze() {
        oldPos = currentPos;
        acc = sf::Vector2f(0, 0);
    }
    void update(float dt, float drag) {
        sf::Vector2f buf = currentPos;
        sf::Vector2f diffPos = (currentPos - oldPos) * (1-drag);
        currentPos = currentPos + diffPos + acc*(dt*dt);
        oldPos = buf;
    }
    void applyScreenConstraint() {
        sf::Vector2f diffPos = (currentPos - oldPos);
        if (currentPos.x < 0) {
            currentPos.x = 0;
            oldPos.x = (currentPos + diffPos*e).x;
        } else if (currentPos.x > SIM_WIDTH) {
            currentPos.x = SIM_WIDTH;
            oldPos.x = (currentPos + diffPos*e).x;
        }else if (currentPos.y < 0) {
            currentPos.y = 0;
            oldPos.y = (currentPos + diffPos*e).y;
        } else if (currentPos.y > SIM_HEIGHT) {
            currentPos.y = SIM_HEIGHT;
            oldPos.y = (currentPos + diffPos*e).y;
        }

    }
    void draw(sf::RenderWindow & window) {
        if (shouldDraw) {
            shape.setRadius(radius);
            shape.setOrigin(sf::Vector2f(radius, radius));
            shape.setPosition(currentPos);
            window.draw(shape);
        }
    }

};
struct VertexRenderer {
    std::vector<Point*> pointList;
    sf::Color fillColor;

    void draw (sf::RenderWindow & window) {
        sf::ConvexShape shape;
        shape.setPointCount(pointList.size());
        shape.setFillColor(fillColor);
        int index = 0;
        sf::Vector2f center(0,0);

        for (auto & point : pointList) {
            center+=point->currentPos;
        }
        center/=static_cast<float>(pointList.size());
        std::vector<std::pair<Point, float>> pointAngles;
        std::ranges::sort(pointAngles, compPointAngle<Point>);
        index = 0;
        for (auto & point : pointList) {
            shape.setPoint(index, point->currentPos);
            index++;
        }
        shape.setOutlineThickness(3);
        shape.setOutlineColor(gray);
        window.draw(shape);
    }
};
class LineConstraint {
public:
    Point * p0;
    Point * p1;
    float length, stiffness;
    bool shouldDraw = false;
    LineConstraint(Point* p0_, Point* p1_, float stiffness_) : p0(p0_), p1(p1_), stiffness(stiffness_) {
        length = distance2f(p0->currentPos, p1->currentPos);
    }
    LineConstraint(Point* p0_, Point* p1_, float length_, float stiffness_) : p0(p0_), p1(p1_), length(length_), stiffness(stiffness_) {}
    LineConstraint(){}
    void draw(sf::RenderWindow & window) {
        if (shouldDraw) {
            std::array<sf::Vertex, 2> line =
            {
                sf::Vertex{p0->currentPos, sf::Color::Blue,p0->currentPos },
                sf::Vertex{p1->currentPos, sf::Color::Blue,p1->currentPos }
            };
            window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
        }
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
    void update(float dt, float drag) {
        p0->update(dt, drag);
        p1->update(dt, drag);
        applyLineConstraint();
        p0->applyScreenConstraint();
        p1->applyScreenConstraint();
    }
    void checkPointCollision(Point* point) {
        float detectRadius = 3;
        float tolerance = 7;

        // Posted by Grumdrig, modified by community. See post 'Timeline' for change history
        // Retrieved 2025-12-14, License - CC BY-SA 4.0
        float l2 = (p1->currentPos-p0->currentPos).lengthSquared();  // i.e. |w-v|^2 -  avoid a sqrt
        float t = std::max(0.0f, std::min(1.0f, (point->currentPos - p0->currentPos).dot( p1->currentPos - p0->currentPos) / l2));
        sf::Vector2f closestPoint = p0->currentPos + t * (p1->currentPos - p0->currentPos);
        float distance = distance2f(point->currentPos, closestPoint);
        if (distance < detectRadius) {
            sf::Vector2f normal = (point->currentPos - closestPoint).normalized();
            float depth = tolerance-distance;

            float pim = (1.0f/point->mass);
            float p0im = (1.0f/p0->mass);
            float p1im = (1.0f/p1->mass);
            float sum = pim + p0im + p1im;
            point->currentPos += normal * depth * (pim/sum);
            p0->currentPos -= normal * depth * (p0im/sum);
            p1->currentPos -= normal * depth * (p1im/sum);
            sf::Vector2f vel = point->currentPos - point->oldPos;
            float vn = vel.dot(normal);
            if (vn < 0.0f)
                vel -= normal * vn;
            point->oldPos = point->currentPos;
            p0->oldPos = p0->currentPos;
            p1->oldPos = p1->currentPos;
            point->oldPos = point->currentPos - vel;
        }

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
    sf::Vector2f initialAcceleration;
    float sideLength;
    float rotation;
    float stiffness;
    float mass;
        Square(sf::Vector2f centerPos_, std::vector<LineConstraint>& lineList, std::vector<Point*>& objList, std::vector<VertexRenderer>& vertexList, float sideLength_, float stiffness_, float rotation_, float mass_, sf::Vector2f initialAcceleration_ = sf::Vector2f(0, 98)) {
            stiffness = stiffness_;
            centerPos = centerPos_;
            sideLength = sideLength_;
            rotation = rotation_;
            mass = mass_;
            initialAcceleration = initialAcceleration_;
            sf::Vector2f sideVector{sideLength, sideLength};
            sf::Vector2f sideHalfVector{sideLength/2, sideLength/2};
            p1 = new Point{(centerPos-sideHalfVector), initialAcceleration, 1, 1, mass};
            p2 = new Point{sf::Vector2f(centerPos.x+sideHalfVector.x, centerPos.y-sideHalfVector.y), initialAcceleration, 1, 1, mass};
            p3 = new Point{sf::Vector2f(p2->currentPos.x, p2->currentPos.y + sideLength), initialAcceleration, 1, 1, mass};
            p4 = new Point{sf::Vector2f(p1->currentPos.x, p1->currentPos.y+sideLength), initialAcceleration, 1, 1, mass};
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
            VertexRenderer newVertex{};
            newVertex.fillColor = orange1;
            newVertex.pointList.push_back(p1);
            newVertex.pointList.push_back(p2);
            newVertex.pointList.push_back(p3);
            newVertex.pointList.push_back(p4);
            vertexList.push_back(newVertex);

    }
};