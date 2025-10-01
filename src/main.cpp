#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    RigidBody testBody(1, 0, 0, 100, 100, 400, 400);
    std::vector<RigidBody> activeBodies;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        for (auto & body : activeBodies) {
            body.render(window);
        }
        while (const std::optional event = window.pollEvent())
        {

            if (event->is<sf::Event::Closed>())
                window.close();
        }
    }
}