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
    sf::Texture bgTexture(std::filesystem::path("../assets/background.png"));
    sf::RectangleShape bgSprite;
    bgSprite.setTexture(&bgTexture);
    auto bgTextureSize = bgTexture.getSize();
    bgSprite.setPosition(sf::Vector2f(0, 0));
    bgSprite.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
    Point* lastPointClicked = nullptr;
    sf::Vector2f lastClickPos = sf::Vector2f(-1, -1);
    const float fps = 60;
    const float dt = 1.0/fps;
    float subtick = 20;
    float drag=0;
    enum Mode mode = PAUSED;
    float accumulator = 0.0f;
    bool precise = true;
    std::string clickedButton;
    std::vector<Point*> pointList{};
    std::vector<Point*> pointClickedList{};
    std::vector<Point*> debugList{};
    std::vector<Button*> buttonList{};
    std::vector<LineConstraint> lineList{};
    std::vector<VertexRenderer> vertexList{};
    int spacing = SCREEN_HEIGHT-SIM_HEIGHT;
    sf::Texture playTex(std::filesystem::path("../assets/play.png"));
    sf::Texture drawTex(std::filesystem::path("../assets/draw.png"));
    sf::Texture connectTex(std::filesystem::path("../assets/connect.png"));
    sf::Texture squareTex(std::filesystem::path("../assets/square.png"));
    sf::Texture vertexTex(std::filesystem::path("../assets/vertex.png"));
    sf::Texture dragTex(std::filesystem::path("../assets/drag.png"));
    sf::Texture subtickTex(std::filesystem::path("../assets/subtick.png"));
    Button playButton = Button(sf::Vector2f(0, SIM_HEIGHT), spacing, spacing, sf::Color::White, playTex, "play");
    Button drawButton = Button(sf::Vector2f(spacing, SIM_HEIGHT), spacing, spacing, sf::Color::White, drawTex, "draw");
    Button connectButton = Button(sf::Vector2f(spacing*2, SIM_HEIGHT), spacing, spacing, sf::Color::White, connectTex, "connect");
    Button squareButton = Button(sf::Vector2f(spacing*3, SIM_HEIGHT), spacing, spacing, sf::Color::White, squareTex, "square");
    Button vertexButton = Button(sf::Vector2f(spacing*4, SIM_HEIGHT), spacing, spacing, sf::Color::White, vertexTex, "vertex");
    Button dragButton = Button(sf::Vector2f(spacing*5, SIM_HEIGHT), spacing, spacing, sf::Color::White, dragTex, "drag");
    Button subtickButton = Button(sf::Vector2f(spacing*6, SIM_HEIGHT), spacing, spacing, sf::Color::White, subtickTex, "subtick");
    buttonList.push_back(&playButton);
    buttonList.push_back(&drawButton);
    buttonList.push_back(&connectButton);
    buttonList.push_back(&squareButton);
    buttonList.push_back(&vertexButton);
    buttonList.push_back(&dragButton);
    buttonList.push_back(&subtickButton);
    while (window.isOpen()) {
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
                } else if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
                    if (mode == VERTEX) {
                        VertexRenderer vertexShape;
                        vertexShape.fillColor = orange1;
                        int stiffness = 1;
                        float kg = std::min(getUserInput<float>("Enter Mass (1-10kg): ", "1"), 10.0f);
                        kg = std::max(1.0f, kg);
                        float iniAccelX = getUserInput<float>("Enter Initial Acceleration X: ", "0");
                        float iniAccelY = getUserInput<float>("Enter Initial Acceleration Y: ", "98");
                        for (auto & point : pointClickedList) {
                            point->mass = kg;
                            point->acc = sf::Vector2f(iniAccelX, iniAccelY);
                            vertexShape.pointList.push_back(point);
                            for (auto point2 : pointClickedList) {
                                if (point2->currentPos != point->currentPos) {
                                    LineConstraint l = LineConstraint{point, point2, 1.0f};
                                    lineList.push_back(l);
                                }
                            }

                        }
                        vertexList.push_back(vertexShape);
                        pointClickedList = {};
                    }
                }
            } else if (const auto* mouseClicked = event->getIf<sf::Event::MouseButtonReleased>()) {
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
                            } else if (button->name == "vertex") {
                                mode = VERTEX;
                                clickedButton = button->name;
                            } else if (button->name == "drag") {
                                drag = getUserInput<float>("Enter Drag (0-1): ", "0");
                            } else if (button->name == "subtick") {
                                subtick = getUserInput<float>("Enter amount of subticks: ", "20");
                            }
                        }
                    }
                    for (auto & button : buttonList) {
                        if (clickedButton == button->name) {
                            button->backgroundColor = gray;
                        } else if (button->name != "precise") {
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

                            float sideLength, angle, kg, iniAccelX, iniAccelY, e;

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
                            e = getUserInput<float>("Enter Coefficient of Restitution (0-1): ", "1");
                            iniAccelX = getUserInput<float>("Enter Initial Acceleration X: ", "0");
                            iniAccelY = getUserInput<float>("Enter Initial Acceleration Y: ", "98");
                            Square* newSquare = new Square(centerPos, lineList, pointList, vertexList, sideLength, 1, angle, kg, sf::Vector2f(iniAccelX, iniAccelY), e);
                            lastClickPos.x = -1;
                        } else {
                            lastClickPos = mousePosition;
                        }
                    }else if (mode == CONNECTION) {
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
                    } else if (mode == VERTEX) {
                        for (auto & point : pointList) {
                            float dist = distance2f(sf::Vector2f(mousePosition), point->currentPos);
                            if (dist < point->radius) {
                                pointClickedList.push_back(point);
                                point->shape.setFillColor(sf::Color::Blue);
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
                        obj->update(dt, drag);
                    }

                    for (int i = 0; i < subtick; i++) {

                        for (auto & line : lineList) {
                            line.applyLineConstraint();
                        }

                        for (auto & line : lineList) {
                            for (auto & point : pointList) {

                                if (line.p0->currentPos != point->currentPos && line.p1->currentPos != point->currentPos) {
                                    if (precise) {
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
                                    } else {
                                        line.checkPointCollision(point);
                                    }
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
