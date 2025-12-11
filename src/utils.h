#include "include.h"
float distance2f(sf::Vector2f p0, sf::Vector2f p1) {
    return sqrt((p1.x - p0.x)*(p1.x - p0.x) + (p1.y - p0.y)*(p1.y - p0.y));
}
//Source: https://stackoverflow.com/questions/620745/c-rotating-a-vector-around-a-certain-point
sf::Vector2f rotateAroundPoint(sf::Vector2f pos, sf::Vector2f point, float degAngle) {
    float x = pos.x;
    float y = pos.y;
    float x_origin = point.x;
    float y_origin = point.y;
    float radAngle = degAngle * PI/180;
    double cs = cos(radAngle);
    double sn = sin(radAngle);

    double translated_x = x - x_origin;
    double translated_y = y - y_origin;

    double result_x = translated_x * cs - translated_y * sn;
    double result_y = translated_x * sn + translated_y * cs;

    result_x += x_origin;
    result_y += y_origin;
    return sf::Vector2f(result_x, result_y);
}
