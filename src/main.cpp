#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Physics Simulation Personal Project");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(false);
    sf::Clock clock;
    //sf::Time dt = sf::milliseconds(16.6667);
    const float fps = 60;
    const float dt = 1.0/fps;
    const float subtick = 100;
    float accumulator = 0.0f;
    Point* p1 = new Point{sf::Vector2f(10, 20), sf::Vector2f(0, 98), 1, 1};
    Point* p2 = new Point{sf::Vector2f(30, 50), sf::Vector2f(0, 98), 1, 1};
    Point* p3 = new Point{sf::Vector2f(70, 60), sf::Vector2f(0, 98), 1, 1};
    Point* p4 = new Point{sf::Vector2f(70, 60), sf::Vector2f(10, 98), 1, 1};
    std::vector<Point*> pointList{p1, p2, p3, p4};
    std::vector<LineConstraint> lineList{};
    lineList.push_back(LineConstraint{p1, p2, float(1)});
    lineList.push_back(LineConstraint{p2, p3, float(1)});
    lineList.push_back(LineConstraint{p3, p1, float(1)});
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
            for (auto & obj : pointList) {
                obj->applyScreenConstraint();
                obj->update(dt);

            }
            for (int i=0; i<subtick; i++) {
                for (auto & line : lineList) {
                    line.applyLineConstraint();
                }
            }
            accumulator -= dt;
        }
        window.clear();
        for (auto & obj : pointList) {
            obj->draw(window);
        }
        for (auto & line : lineList) {
            line.draw(window);
        }
        window.display();


    }
}