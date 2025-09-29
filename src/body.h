#include "definitions.h"
#include "include.h"
struct NewtonForce {
    int mag; double dir;
    std::pair<double, double> dirToXY() const {
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
    NewtonForce(int mag_, double dir_) : mag(mag_), dir(dir_) {
        if (dir >= -180 && dir <= 180 ) {
            //continue the program
        } else {
            //Degrees can't be more than -180 / 180
            throw(std::range_error("Degrees out of range"));
        }
    }
};
class RigidBody {
public:
    // mass measured in kg
    // velocity measured in m/s
    // acceleration measured in m/s^2
    RigidBody(double mass_, double velocity_, double acceleration_, double x_, double y_, double width_, double height_) : mass(mass_), velocity(velocity_), acceleration(acceleration_), x(x_), y(y_), width(width_), height(height_)
    {std::cout << "Initialized RigidBody with initial values:\nmass [kg]: " << this->mass << "\nvelocity [m/s]: " << velocity << "\nacceleration [m/s^2]: " << acceleration << "\nx, y: (" << x << ", " << y << ")";}
private:
    double mass, velocity, acceleration, height, width, x, y;
    std::vector<NewtonForce> forces;
};