#include "body.h"
#include "include.h"
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Physics Simulation Personal Project");
    //test body for now
    RigidBody testBody1(1, sf::Vector2<float>{10, 1}, sf::Vector2<float>{1, 1}, 850, 500, 40, 40, 1, sf::degrees(0), 0, 0);
    RigidBody testBody2(1, sf::Vector2<float>{10, 1}, sf::Vector2<float>{100, 1}, 590, 500, 40, 40, 2, sf::degrees(0), 0, 0);
    std::vector<RigidBody*> activeBodies;
    std::vector<DebugPoint*> debugPoints;
    activeBodies.push_back(&testBody1);
    activeBodies.push_back(&testBody2);
    std::vector<Collision> collisionsList{};
    PhysicsEvaluator physicsEvaluator{};
    sf::Clock deltaClock;
    sf::Time dt = sf::milliseconds(16.6667);
    bool firstFrame = true;
    while (window.isOpen())
    {
        window.clear(sf::Color::Black);
        if (!firstFrame) {
            for (auto & body : activeBodies) {
                body->checkCollisions(activeBodies, collisionsList);
            }
            for (auto & collision : collisionsList) {
                auto* newPoint = new DebugPoint(collision.collision_x, collision.collision_y, 5);
                debugPoints.push_back(newPoint);
                collision.body1->freeze();
                collision.body2->freeze();
                physicsEvaluator.evaluateCollisionVelocity(&collision);

            }
        }

        for (auto & body : activeBodies) {
            body->updatePosition(dt.asSeconds());
            body->update();
            body->render(window);
        }
        for (auto & point : debugPoints) {
            point->render(window);
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
        if (firstFrame) {
            firstFrame = false;
        }
    }
}