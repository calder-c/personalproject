#include "definitions.h"
#include "include.h"
#include "utils.h"
class RigidBody;
struct Collision {
    RigidBody* body1;
    RigidBody* body2;
    int collisionId;
    double collision_x, collision_y;
    Collision(RigidBody* body1_, RigidBody* body2_, double collision_x_, double collision_y_, int collisionId_) : body1(body1_), body2(body2_), collision_x(collision_x_), collision_y(collision_y_), collisionId(collisionId_) {
    }
};

class RigidBody {
public:
    // Public for ease of use but ideally these should be private
    double mass, x, y;
    sf::Vector2<double> velocity, acceleration;
    float height, width;
    int id;
    sf::Angle degreesRotation;
    sf::RectangleShape shape;
    std::vector<sf::Vector2<double>> forces;
    // mass measured in kg
    // velocity measured in m/s
    // acceleration measured in m/s^2
    // x, y = meters
    RigidBody(double mass_, sf::Vector2<double> velocity_, sf::Vector2<double> acceleration_, double x_, double y_, float width_, float height_, int id_, sf::Angle degrees_) : mass(mass_), velocity(velocity_), acceleration(acceleration_), x(x_), y(y_), width(width_), height(height_), degreesRotation(degrees_) {
        //this defines the shape to be able to render through SFML
        srand(time(NULL));
        shape = sf::RectangleShape{};
        //sets color
        shape.setFillColor(orange1);
        shape.setOutlineThickness(3.0f);
        shape.setOutlineColor(orange2);
        //sets width, height
        shape.setSize(sf::Vector2f(width, height));
        //sets the middle point of the rectangle
        shape.setOrigin(sf::Vector2f(width/2, height/2));
        //if id=0 it sets a random id
        if (id_ == 0) {
            this->id = rand() % 10000000;
        } else {
            this->id = id_;
        }

    }
    void checkCollisions (std::vector<RigidBody*> & activeBodies, std::vector<Collision> & collisionsList) {
        for (auto & body : activeBodies) {
            if (body->id != this->id) {
                sf::FloatRect boundingBox1 = this->shape.getGlobalBounds();
                sf::FloatRect boundingBox2 = body->shape.getGlobalBounds();
                if (std::optional intersection = boundingBox1.findIntersection(boundingBox2)) {

                    double collision_x, collision_y;
                    sf::Vector2<float> collisionPoint = intersection->getCenter();
                    collision_x = collisionPoint.x;
                    collision_y = collisionPoint.y;
                    bool detectedDuplicate = false;
                    int uniqueId = generateUniqueId(this->id, body->id);
                    for (auto & collision : collisionsList) {
                        if (collision.collisionId == uniqueId) {
                            detectedDuplicate = true;
                            break;
                        }
                    }
                    if (!detectedDuplicate) {

                        std::cout << uniqueId << "\n";
                        std::cout << this->id << " Registered collision (" << collision_x << ", " << collision_y << ") with " << body->id << "\n";
                        collisionsList.emplace_back(this, body, collision_x, collision_y, uniqueId);
                        this->shape.setFillColor(debugRed);
                        body->shape.setFillColor(debugRed);
                    }


                } else {
                    const int uniqueId = generateUniqueId(this->id, body->id);
                    for (auto it = collisionsList.begin(); it != collisionsList.end();) {

                        // Remove even elements
                        if (uniqueId == it->collisionId) {
                            it = collisionsList.erase(it);
                            std::cout << "Stopped colliding \n";
                        }else {
                            it++;
                        }

                    }
                    this->shape.setFillColor(debugGreen);
                    body->shape.setFillColor(debugGreen);

                }
            }

        }
    }
    void updatePosition(double dt) {
        velocity += acceleration;
        x += velocity.x * dt;
        y += velocity.y * dt;
    }
    void render(sf::RenderWindow & window) {
        window.draw(shape);
    }
    void update() {
        for (auto & force : forces) {

        }
        shape.setPosition(sf::Vector2f(x, y));
        shape.setRotation(degreesRotation);
    }
};

class PhysicsEvaluator {
    double gravity;
public:
    PhysicsEvaluator(double gravity_) : gravity(gravity_){}
    PhysicsEvaluator() {
        gravity = earth_g;
    }
    void evaluateCollision(Collision* collision) {
        sf::Vector2<double> m1 = collision->body1->velocity * collision->body1->mass;
        sf::Vector2<double> m2 = collision->body2->velocity * collision->body2->mass;
        sf::Vector2<double> normal;
    }
};