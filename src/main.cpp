#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Physics Simulation Personal Project");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(false);
    sf::Clock clock;
    //sf::Time dt = sf::milliseconds(16.6667);
    const float dt = 1.0/60.0;
    float accumulator = 0.0f;
    std::vector<Point> objList{};
    objList.push_back(Point{sf::Vector2f(200, 300), sf::Vector2f(1, 98), 10});
    objList.push_back(Point{sf::Vector2f(250, 200), sf::Vector2f(1, 98), 10});
    objList.push_back(Point{sf::Vector2f(220, 100), sf::Vector2f(1, 98), 10});
    objList.push_back(Point{sf::Vector2f(210, 0), sf::Vector2f(1, 98), 10});
    objList.push_back(Point{sf::Vector2f(0, 400), sf::Vector2f(1, 98), 10});
    objList.push_back(Point{sf::Vector2f(60, 400), sf::Vector2f(61, 400), sf::Vector2f(1, 98), 10});
    while (window.isOpen())
    {
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;
        while (const std::optional event = window.pollEvent())
        {
            //checks for close event
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        while (accumulator >= dt) {
            for (auto & obj : objList) {
                obj.applyScreenConstraint();
                obj.update(dt);
            }
            accumulator -= dt;
        }
        window.clear();
        for (auto & obj : objList) {

            obj.draw(window);
        }

        window.display();


    }
}