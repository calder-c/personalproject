#include "body.h"
#include "include.h"
#include "ui.h"
#define PRECISE true
int main() {
    sf::ContextSettings settings{};
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Physics Simulation Personal Project", sf::Style::Default, sf::State::Windowed, settings);
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(false);
    sf::Clock clock;
    sf::Texture bgTexture(std::filesystem::path("../assets/background.png"));
    sf::RectangleShape bgSprite;
    bgSprite.setTexture(&bgTexture);
    auto bgTextureSize = bgTexture.getSize();
    bgSprite.setPosition(sf::Vector2f(0, 0));
    bgSprite.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    Point* lastPointClicked = nullptr;
    sf::Vector2f lastClickPos = sf::Vector2f(-1, -1);
    //sf::Time dt = sf::milliseconds(16.6667);
    const float fps = 60;
    const float dt = 1.0/fps;
    const float subtick = 10;
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
    sf::Texture squareTex(std::filesystem::path("../assets/square.png"));
    Button playButton = Button(sf::Vector2f(0, SIM_HEIGHT), spacing, spacing, sf::Color::White, playTex, "play");
    Button drawButton = Button(sf::Vector2f(spacing, SIM_HEIGHT), spacing, spacing, sf::Color::White, drawTex, "draw");
    Button connectButton = Button(sf::Vector2f(spacing*2, SIM_HEIGHT), spacing, spacing, sf::Color::White, connectTex, "connect");
    Button squareButton = Button(sf::Vector2f(spacing*3, SIM_HEIGHT), spacing, spacing, sf::Color::White, squareTex, "square");
    buttonList.push_back(&playButton);
    buttonList.push_back(&drawButton);
    buttonList.push_back(&connectButton);
    buttonList.push_back(&squareButton);
    // Square s1{sf::Vector2f(400, 400), lineList, pointList, vertexList, 150, 1, 0, 10, sf::Vector2f(10, 98)};
    // Square s2{sf::Vector2f(100, 100), lineList, pointList, vertexList, 150, 1, 35, 10, sf::Vector2f(0, 98)};
    // Square s3{sf::Vector2f(700, 100), lineList, pointList, vertexList, 30, 1, 25, 10, sf::Vector2f(40, 98)};
    // Square s4{sf::Vector2f(40, 400), lineList, pointList, vertexList, 40, 1, 15, 10, sf::Vector2f(40, 98)};
    //
    // Square s1{sf::Vector2f(100, 100), lineList, pointList, vertexList, 150, 1, 0, 10, sf::Vector2f(100, 98)};
    // Square s2{sf::Vector2f(150, 300), lineList, pointList, vertexList, 150, 1, 35, 10, sf::Vector2f(0, 98)};
    // Square s3{sf::Vector2f(700, 100), lineList, pointList, vertexList, 30, 1, 25, 2, sf::Vector2f(40, 98)};
    // Square s4{sf::Vector2f(40, 400), lineList, pointList, vertexList, 40, 1, 15, 3, sf::Vector2f(40, 98)};
    while (window.isOpen())
    {
        float frameTime = clock.restart().asSeconds();
        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        if (mode != SIMULATING) {
            for (auto & point : pointList) {
                point->shouldDraw = true;
                float dist = distance2f(sf::Vector2f(mousePosition), point->currentPos);
                if (dist <= 10) {

                    if (point->radius < 10) {
                        point->radius += 0.20;
                    }

                } else {
                    point->radius = 1;

                }
            }

        } else {
            for (auto & point : pointList) {
                point->shouldDraw = false;
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
                    } else {
                        clickedButton = "play";
                        mode = SIMULATING;
                    }
                    for (auto & button : buttonList) {
                        if (clickedButton == button->name) {
                            button->backgroundColor = gray;
                        } else {
                            button->backgroundColor = sf::Color::White;
                        }
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
                                } else {
                                    clickedButton = "play";
                                    mode = SIMULATING;
                                }
                            } else if (button->name == "draw") {
                                mode = DRAW;
                                clickedButton = button->name;
                            } else if (button->name == "connect") {
                                mode = CONNECTION;
                                clickedButton = button->name;
                            } else if (button->name == "square") {
                                mode = SQUARE;
                                clickedButton = button->name;
                            }
                        }
                    }
                    for (auto & button : buttonList) {
                        if (clickedButton == button->name) {
                            button->backgroundColor = gray;
                        } else {
                            button->backgroundColor = sf::Color::White;
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
                                if (lastPointClicked) {
                                    auto line = LineConstraint{point, lastPointClicked, 1};
                                    line.shouldDraw = true;
                                    lineList.push_back(line);
                                    lastPointClicked = nullptr;
                                } else {
                                    lastPointClicked = point;
                                }
                            }
                        }
                    } else if (mode == SQUARE) {
                        if (lastClickPos.x != -1) {

                            float sideLength;
                            float angle;
                            float kg;
                            float iniAccelX;
                            float iniAccelY;

                            sf::Vector2f centerPos;
                            // std::cout << lastClickPos.x << "LastClickPos \n";
                            // std::cout << mousePosition.x << "MousePos \n";
                            if (lastClickPos.x < mousePosition.x) {
                                sideLength = (mousePosition.x - lastClickPos.x);
                                centerPos = mousePosition-(mousePosition - lastClickPos)*0.5f;
                            }
                            else if (lastClickPos.x > mousePosition.x) {
                                sideLength = (lastClickPos.x - mousePosition.x);
                                centerPos = lastClickPos+(lastClickPos - mousePosition)*0.5f;
                            } else {

                            }
                            angle = getUserInput<float>("Enter Angle (degrees): ", "0");
                            kg = getUserInput<float>("Enter Mass (kg): ", "1");
                            iniAccelX = getUserInput<float>("Enter Initial Acceleration X: ", "0");
                            iniAccelY = getUserInput<float>("Enter Initial Acceleration Y: ", "98");
                            Square* newSquare = new Square(centerPos, lineList, pointList, vertexList, sideLength, 1, angle, kg, sf::Vector2f(iniAccelX, iniAccelY));
                            lastClickPos.x = -1;
                        } else {
                            lastClickPos = mousePosition;
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

        for (int i = 0; i < subtick; i++) {

            for (auto & line : lineList) {
                line.applyLineConstraint();
            }

            for (auto & line : lineList) {
                for (auto & point : pointList) {

                    if (line.p0->currentPos != point->currentPos && line.p1->currentPos != point->currentPos) {
                        #if (PRECISE)
                            sf::Vector2f motion = point->currentPos - point->oldPos;
                            float motionLength = std::sqrt(motion.x*motion.x + motion.y*motion.y);

                            int steps = std::max(1, int(std::ceil(motionLength)));

                            sf::Vector2f stepMotion = motion / float(steps);

                            sf::Vector2f interpPos = point->oldPos;
                            for (int s = 0; s < steps; ++s) {
                                interpPos += stepMotion;
                                sf::Vector2f originalPos = point->currentPos;
                                point->currentPos = interpPos;
                                line.checkPointCollision(point);
                            }
                        #else

                            line.checkPointCollision(point);


                        #endif


                    }
                }
            }
        }

        accumulator -= dt;
    }
}


        window.clear();
        window.draw(bgSprite);
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
