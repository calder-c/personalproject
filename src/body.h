#include "definitions.h"
#include "include.h"
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
    // mass measured in kg
    // velocity measured in m/s
    // acceleration measured in m/s^2
    // x, y = meters
    RigidBody(double mass_, double velocity_, double acceleration_, double x_, double y_, float width_, float height_) : mass(mass_), velocity(velocity_), acceleration(acceleration_), x(x_), y(y_), width(width_), height(height_) {
        //this defines the shape to be able to render through SFML
        shape = sf::RectangleShape{};
        //sets color
        shape.setFillColor(orange);
        //sets width, height
        shape.setSize(sf::Vector2f(width, height));
        //sets the middle point of the rectangle
        shape.setOrigin(sf::Vector2f(width/2, height/2));
        std::cout << "Initialized RigidBody with initial values:\nmass [kg]: " << this->mass << "\nvelocity [m/s]: " << velocity << "\nacceleration [m/s^2]: " << acceleration << "\nx, y: (" << x << ", " << y << ")";
    }
    void render(sf::RenderWindow & window) {
        //update the shape that will be rendered to the x, y position of the object
        shape.setPosition(sf::Vector2f(x, y));
        window.draw(shape);
        sf::Angle angle1 = sf::degrees(1);
        shape.rotate(angle1);
    }
    // Public for ease of use but ideally these should be private
    double mass, velocity, acceleration, x, y;
    float height, width;
    sf::RectangleShape shape;
    std::vector<NewtonForce> forces;
};