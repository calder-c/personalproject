#include "body.h"
#include "include.h"
#include "ui.h"

int main() {
    sf::ContextSettings settings{};
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Physics Simulation Personal Project", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(false);
    sf::Clock clock;
    Point* lastClicked = new Point(sf::Vector2f(0, 0), sf::Vector2f(0, 0), -1, 0, 0);
    //sf::Time dt = sf::milliseconds(16.6667);
    const float fps = 60;
    const float dt = 1.0/fps;
    const float subtick = 100;
    enum Mode mode = PAUSED;
    float accumulator = 0.0f;
    std::string clickedButton;
    // Point* p1 = new Point{sf::Vector2f(100, 100), sf::Vector2f(110, 98), 1, 1};
    // Point* p2 = new Point{sf::Vector2f(200, 100), sf::Vector2f(0, 98), 1, 1};
    // Point* p3 = new Point{sf::Vector2f(200, 200), sf::Vector2f(0, 98), 1, 1};
    // Point* p4 = new Point{sf::Vector2f(100, 200), sf::Vector2f(0, 98), 1, 1};
    //
    std::vector<Point*> pointList{};
    std::vector<Point*> debugList{};
    std::vector<Button*> buttonList{};
    std::vector<LineConstraint> lineList{};
    std::vector<VertexRenderer> vertexList{};

    // float enforceAmount = 1;
    // lineList.push_back(LineConstraint{p1, p2, enforceAmount});
    // lineList.push_back(LineConstraint{p2, p3, enforceAmount});
    // lineList.push_back(LineConstraint{p3, p4, enforceAmount});
    // lineList.push_back(LineConstraint{p4, p1, enforceAmount});
    // lineList.push_back(LineConstraint{p3, p1, enforceAmount});
    int spacing = SCREEN_HEIGHT-SIM_HEIGHT;
    sf::Texture playTex(std::filesystem::path("../assets/play.png"));
    sf::Texture drawTex(std::filesystem::path("../assets/draw.png"));
    sf::Texture connectTex(std::filesystem::path("../assets/connect.png"));
    Button playButton = Button(sf::Vector2f(0, SIM_HEIGHT), spacing, spacing, sf::Color::Green, playTex, "play");
    Button drawButton = Button(sf::Vector2f(spacing, SIM_HEIGHT), spacing, spacing, sf::Color::White, drawTex, "draw");
    Button connectButton = Button(sf::Vector2f(spacing*2, SIM_HEIGHT), spacing, spacing, sf::Color::White, connectTex, "connect");
    buttonList.push_back(&playButton);
    buttonList.push_back(&drawButton);
    buttonList.push_back(&connectButton);
    Square s1{sf::Vector2f(400, 400), lineList, pointList, vertexList, 150, 1, 0, 1, sf::Vector2f(20, 98)};
    Square s2{sf::Vector2f(100, 100), lineList, pointList, vertexList, 150, 1, 45, 100, sf::Vector2f(0, 98)};
    Square s3{sf::Vector2f(700, 100), lineList, pointList, vertexList, 10, 1, 45, 10, sf::Vector2f(40, 98)};

    //Square s2{sf::Vector2f(400, 200), lineList, pointList, 150, 1, 0, sf::Vector2f(100, 98)};
    // Point* p1 = new Point{sf::Vector2f(100, 200), sf::Vector2f(40, 98), 1, 1, 1};
    // Point* p2 = new Point{sf::Vector2f(100, 200), sf::Vector2f(40, 98), 1, 0, 1};
    // Point* p3 = new Point{sf::Vector2f(100, 200), sf::Vector2f(40, 98), 1, 0.5, 1};
    // pointList.push_back(p1);
    // pointList.push_back(p2);
    // pointList.push_back(p3);
    while (window.isOpen())
    {
        float frameTime = clock.restart().asSeconds();
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        if (mode != SIMULATING) {
            for (auto & point : pointList) {
                float dist = distance2f(sf::Vector2f(mousePosition), point->currentPos);
                if (dist <= 10) {
                    point->shouldDraw = true;
                    if (point->radius < 10) {
                        point->radius += 0.20;
                    }

                } else {
                    point->radius = 1;
                }
            }

        }
        while (const std::optional event = window.pollEvent())
        {
            //checks for close event
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Space) {
                    if (mode == SIMULATING) {
                        clickedButton = "";
                        mode = PAUSED;
                        for (auto & point : pointList) {
                            point->shouldDraw = true;
                        }
                    } else {
                        clickedButton = "play";
                        for (auto & point : pointList) {
                            point->shouldDraw = false;
                            point->radius = 1;
                        }
                        mode = SIMULATING;
                    }
                }
            } else if (const auto* mouseClicked = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (mouseClicked->position.y > SIM_HEIGHT) {
                    for (auto & button : buttonList) {
                        if ((mouseClicked->position.x >= button->position.x && mouseClicked->position.x <= button->position.x + button->width) && (mouseClicked->position.y >= button->position.y && mouseClicked->position.y <= button->position.y + button->height)) {
                            if (button->name == "play") {
                                if (mode == SIMULATING) {
                                    clickedButton = "";
                                    mode = PAUSED;
                                    for (auto & point : pointList) {
                                        point->shouldDraw = true;
                                    }
                                } else {
                                    clickedButton = "play";
                                    for (auto & point : pointList) {
                                        point->shouldDraw = false;
                                        point->radius = 1;
                                    }
                                    mode = SIMULATING;
                                }

                            } else if (button->name == "draw") {
                                mode = DRAW;
                                clickedButton = button->name;
                            } else if (button->name == "connect") {
                                mode = CONNECTION;
                                clickedButton = button->name;
                            }
                        }
                    }
                    for (auto & button : buttonList) {
                        if (clickedButton != button->name) {
                            button->backgroundColor = sf::Color::White;
                        } else {
                            button->backgroundColor = gray;
                        }
                    }
                } else {
                    if (mode == DRAW) {
                        Point* p = new Point{sf::Vector2f(mouseClicked->position.x, mouseClicked->position.y), sf::Vector2f(0, 98), 1, 1, 1};
                        p->shouldDraw = true;
                        pointList.push_back(p);
                    } else if (mode == CONNECTION) {
                        for (auto & point : pointList) {
                            float dist = distance2f(sf::Vector2f(mousePosition), point->currentPos);
                            if (dist < point->radius) {
                                if (lastClicked->radius != -1) {
                                    auto line = LineConstraint{point, lastClicked, 1};
                                    line.shouldDraw = true;
                                    lineList.push_back(line);
                                    lastClicked = point;
                                } else {
                                    lastClicked = point;
                                }
                            }
                        }
                    }
                }
            }
        }
        if (mode == SIMULATING) {
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
                    for (auto & line : lineList) {
                        for (auto & point : pointList) {
                            if (line.p0->currentPos != point->currentPos && line.p1->currentPos != point->currentPos) {
                                line.checkPointCollision(point);
                            }

                        }
                    }
                }


                accumulator -= dt;
            }
        }

        window.clear();
        for (auto & line : lineList) {
            line.draw(window);
        }
        for (auto & vertex : vertexList) {
            vertex.draw(window);
        }
        for (auto & obj : pointList) {
            obj->draw(window);
        }
        for (auto & button : buttonList) {
            button->draw(window);
        }
        for (auto it = debugList.begin(); it != debugList.end(); it++) {
            (*it)->draw(window);
            if ((*it)->lifetime > 1) {
                debugList.erase(it);
                it--;
            } else {
                (*it)->lifetime++;
            }

        }
        window.display();


    }
}
