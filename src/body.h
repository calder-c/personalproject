#include "definitions.h"
#include "include.h"
class RigidBody;
struct Collision {
    RigidBody* body;
    double collision_x, collision_y;
    Collision(RigidBody* body_, double collision_x_, double collision_y_) : body(body_), collision_x(collision_x_), collision_y(collision_y_){}
};
struct NewtonForce {
    // magnitude measured in newtons
    static std::pair<double, double> dirToXY(double dir) {
        double x = cos(dir * (PI / 180));
        double y = sin(dir * (PI / 180));
        if (x - trunc(x) < 0.001) {
            x = trunc(x);
        }
        if (y - trunc(y) < 0.001) {
            y = trunc(y);
        }
        return std::pair<double, double>(x, y);
    }
    int mag;
    double dirX, dirY;
    NewtonForce(int mag_, double x_, double y_) : mag(mag_), dirX(x_), dirY(y_) {
        if (dirX >= 1 && dirX <= -1 ) {
            //continue the program
        } else {
            //Direction x, y can't be more than 1 or -1 because that would mean a stronger force
            throw(std::range_error("Direction out of range"));
        }
    }
};

class RigidBody {
public:
    // Public for ease of use but ideally these should be private
    double mass, velocity, acceleration, x, y;
    float height, width;
    int id;
    sf::Angle degrees;
    sf::RectangleShape shape;
    std::vector<NewtonForce> forces;
    std::vector<Collision> activeCollisions;
    // mass measured in kg
    // velocity measured in m/s
    // acceleration measured in m/s^2
    // x, y = meters
    RigidBody(double mass_, double velocity_, double acceleration_, double x_, double y_, float width_, float height_, int id_, sf::Angle degrees_) : mass(mass_), velocity(velocity_), acceleration(acceleration_), x(x_), y(y_), width(width_), height(height_), degrees(degrees_) {
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
        std::cout << "Initialized RigidBody [id = " << id << "] with initial values:\nmass [kg]: " << this->mass << "\nvelocity [m/s]: " << velocity << "\nacceleration [m/s^2]: " << acceleration << "\nx, y: (" << x << ", " << y << ")";
    }
    void checkCollisions (std::vector<RigidBody*> & activeBodies) {
        for (auto & body : activeBodies) {
            if (this->x <= body->x+body->width/2 && this->x >= body->x-body->width/2 && this->y <= body->y+body->height/2 && this->y >= body->y-body->height/2) {
                std::cout << "RigidBody with id " << id << " is colliding with RigidBody << " << id;
                double collision_x, collision_y;
                this->activeCollisions.emplace_back(body, collision_x, collision_y);
                body->activeCollisions.emplace_back(this, collision_x, collision_y);

            }
        }
    }
    void render(sf::RenderWindow & window) {
        //update the shape that will be rendered to the x, y position of the object
        shape.setPosition(sf::Vector2f(x, y));
        shape.setRotation(degrees);
        window.draw(shape);

    }
    void update() {
        for (auto & force : forces) {

        }
    }
};
