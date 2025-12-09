#include "include.h"
float distance2f(sf::Vector2f p0, sf::Vector2f p1) {
    return sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
}