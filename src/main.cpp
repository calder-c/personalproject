#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    //test body for now
    RigidBody testBody(1, 0, 0, 1000, 1000, 40, 40);
    std::vector<RigidBody*> activeBodies;
    activeBodies.push_back(&testBody);
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