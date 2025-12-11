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
    bool simulating = false;
    float accumulator = 0.0f;

    // Point* p1 = new Point{sf::Vector2f(100, 100), sf::Vector2f(110, 98), 1, 1};
    // Point* p2 = new Point{sf::Vector2f(200, 100), sf::Vector2f(0, 98), 1, 1};
    // Point* p3 = new Point{sf::Vector2f(200, 200), sf::Vector2f(0, 98), 1, 1};
    // Point* p4 = new Point{sf::Vector2f(100, 200), sf::Vector2f(0, 98), 1, 1};
    //
    std::vector<Point*> pointList{};
    std::vector<LineConstraint> lineList{};
    // float enforceAmount = 1;
    // lineList.push_back(LineConstraint{p1, p2, enforceAmount});
    // lineList.push_back(LineConstraint{p2, p3, enforceAmount});
    // lineList.push_back(LineConstraint{p3, p4, enforceAmount});
    // lineList.push_back(LineConstraint{p4, p1, enforceAmount});
    // lineList.push_back(LineConstraint{p3, p1, enforceAmount});

    Square s1{sf::Vector2f(400, 400), lineList, pointList, 150, 0, 0};
    Point* p1 = new Point{sf::Vector2f(100, 200), sf::Vector2f(0, 98), 1, 1};
    pointList.push_back(p1);

    while (window.isOpen())
    {
        float frameTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            //checks for close event
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    simulating = !simulating;
                }
            }
        }
        if (simulating) {
            accumulator += frameTime;
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