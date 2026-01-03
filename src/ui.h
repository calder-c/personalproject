#include "include.h"
class Button {
public:
    bool shouldDraw;
    sf::Color backgroundColor;
    sf::Sprite sprite;
    sf::RectangleShape shape;
    sf::Vector2f position;
    std::string name;
    float width, height;
    std::function<void()> function;
    Button(sf::Vector2f position_, float width_, float height_, sf::Color backgroundColor_, sf::Texture & image_, std::string name_) : backgroundColor(
        backgroundColor_), sprite(sf::Sprite{image_}), width(width_), height(height_), position(position_), name(name_){

        shape.setPosition(position_);
        shape.setSize(sf::Vector2f(width, height));
        sprite.setPosition(position_);
        auto size = image_.getSize();
        sprite.setScale(sf::Vector2f(width/size.x, height/size.y));
        shouldDraw = true;
    }
    void draw(sf::RenderWindow & window) {
        if (shouldDraw) {
            shape.setFillColor(backgroundColor);
            window.draw(shape);
            window.draw(sprite);
        }
    }
};