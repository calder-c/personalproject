#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    sf::Clock deltaClock;
    sf::Time dt = sf::milliseconds(16.6667);
    Point obj1{sf::Vector2f(1, 1), sf::Vector2f(1.5, 1.5), sf::Vector2f(30.5, 30.5), 100};
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        obj1.update(dt.asSeconds());

        while (const std::optional event = window.pollEvent())
        {
            //checks for close event
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        obj1.draw(window);
        window.display();
        dt = deltaClock.restart();

    }
}