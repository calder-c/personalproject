#include "include.h"
class DebugPoint {
    public:
        double x, y;
        sf::CircleShape point;
        DebugPoint(double x_, double y_, double radius) : x(x_), y(y_) {
            point = sf::CircleShape();
            point.setPosition(sf::Vector2f(x, y));
            point.setRadius(radius);
            point.setOrigin(sf::Vector2f(radius, radius));
            point.setFillColor(sf::Color::Blue);
        }
        void render(sf::RenderWindow & window) {
                window.draw(point);
        }


};
int generateUniqueId(int n1, int n2) {
    if (n1 < n2) {
        const int buf = n2;
        n2 = n1;
        n1 = buf;
    }
    return (n1+n2) * (n1+n2+1) /2 + n2;
}