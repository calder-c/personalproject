#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    //test body for now
    RigidBody testBody1(1, 0, 0, 850, 500, 40, 40, 1, sf::degrees(0));
    RigidBody testBody2(1, 0, 0, 590, 500, 40, 40, 2, sf::degrees(0));
    std::vector<RigidBody*> activeBodies;
    activeBodies.push_back(&testBody1);
    activeBodies.push_back(&testBody2);
    sf::Clock deltaClock;
    sf::Time dt;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        for (auto & body : activeBodies) {
            //renders every active RigidBody
            testBody1.degreesRotation += sf::degrees(10*dt.asSeconds());
            testBody1.x -= 10*dt.asSeconds();
            body->update();
            body->checkCollisions(activeBodies);
            body->render(window);

        }

        while (const std::optional event = window.pollEvent())
        {
            //checks for close event
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        //displays the frame
        window.display();
        dt = deltaClock.restart();
    }
}