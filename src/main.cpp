#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    //test body for now
    RigidBody testBody1(1, 0, 0, 630, 500, 40, 40, 1, sf::degrees(0));
    RigidBody testBody2(1, 0, 0, 600, 500, 40, 40, 2, sf::degrees(0));
    std::vector<RigidBody*> activeBodies;
    activeBodies.push_back(&testBody1);
    activeBodies.push_back(&testBody2);
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        for (auto & body : activeBodies) {
            //renders every active RigidBody
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
    }
}