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
    RigidBody(double mass_, double velocity_, double acceleration_, double x_, double y_, double width_, double height_) : mass(mass_), velocity(velocity_), acceleration(acceleration_), x(x_), y(y_), width(width_), height(height_)
    {std::cout << "Initialized RigidBody with initial values:\nmass [kg]: " << this->mass << "\nvelocity [m/s]: " << velocity << "\nacceleration [m/s^2]: " << acceleration << "\nx, y: (" << x << ", " << y << ")";}
    void render(sf::RenderWindow & window) {

    }
private:
    double mass, velocity, acceleration, height, width, x, y;
    std::vector<NewtonForce> forces;
};