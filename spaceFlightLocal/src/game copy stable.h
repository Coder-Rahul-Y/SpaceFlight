#ifndef Gme_H
#define Gme_h

//add sprites
//learn to add multiplayer
//add instant win in multiplayer top level

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <sstream>

class highscore
{
public:
    char name[50];
    int score;

    void setName()
    {
        // std::cin.ignore(1,'\n');
        std::cout << "Enter your name : ";
        std::cin.getline(name, 50);
    }

    void setScore(int a)
    {
        score = a;
    }

    char *getName()
    {
        return name;
    }

    int getScore()
    {
        return score;
    }

} hs, hs1;

enum Type
{
    normal = 0,
    repair,
    danger,
    power,
    superDanger,
    boss,
    points,
};

enum lvl
{
    probabilityNum = 0,
    speedNum
};

struct obstacle
{
    sf::Vector2f pos;
    sf::Vector2f size;
    float speed = 0.f;
    int type;
    sf::Color color;
    sf::Texture *tex = NULL;
    int points = 0;
    int health = 0;
};

class Level
{
    //use level.type[level][probabilityNum] OR level.type[level][speedNum]
public:
    int normal[4][2] = {{60, 3}, {60, 3}, {60, 3}, {60, 3}};
    int repair[4][2] = {{4, 7}, {4, 7}, {4, 7}, {4, 7}};
    int danger[4][2] = {{26, 3}, {36, 3}, {44, 3}, {60, 3}};
    int power[4][2] = {{1, 9}, {1, 9}, {1, 9}, {1, 9}};
    int superdanger[4][2] = {{9, 8}, {13, 8}, {18, 8}, {22, 8}};
};

class Game
{
private:
    bool running = true, leftPressed = false, healthFlash = false, invincible = false, firstInitLevel = false;
    sf::RenderWindow *window;
    int windowHeight = 600, windowWidth = 800;
    sf::Event ev;
    sf::RectangleShape obstacleRect, healthRect, lvlPointsRect;
    sf::Vector2f ShipSize;
    float pointsRectY = windowHeight, healthRectApproachY = -1.f, healthRectY = 0.f;

    //sprites and textures
    sf::Texture shiptexture, dangertexture, normaltexture, sdangertexture, powertexture, repairtexture;
    sf::Sprite sship, normalSprite, dangerSprite, sdangerSprite, powerSprite, repairSprite;

    int obsNo = 0;
    obstacle obstacles[10];

    Level lvl;
    int level = 1;
    int lvlSpeed = 0;
    int points = 0, health = 15, maxHealth = 15, pointsAtLevelup = 0, pointsForLevelUp = 1000;

    float spawnTime = 12.f, timer = 0.f, KeyTimer = 0.f, flashTimer = 0.f, invinciTimer = 0.f;

    float dir = 0.f;
    // int loops = 0, firstpress = 0;

    //text
    sf::Font font;
    sf::Text uiText;

    //probibility
    int pNormal, pRepair, pDanger, pPower, pSuperDanger,
        sNormal, sRepair, sDanger, sPower, sSuperDanger;

    int all;

    //functions
    void initObstacle(int num)
    {

        points++;
        int probability = rand() % all;

        //Normal
        if (probability < pNormal)
        {
            obstacles[num].type = normal;
            obstacles[num].speed = sNormal;
            obstacles[num].color = sf::Color::Blue;
            obstacles[num].tex = &normaltexture;
            obstacles[num].size = sf::Vector2f(40.f, 40.f);
            obstacles[num].points = 10;
            obstacles[num].health = 0;
        }

        //Repair
        else if (probability < pNormal + pRepair)
        {
            obstacles[num].type = repair;
            obstacles[num].speed = sRepair;
            obstacles[num].color = sf::Color::Yellow;
            obstacles[num].tex = &repairtexture;
            obstacles[num].size = sf::Vector2f(20.f, 20.f);
            obstacles[num].points = 50;
            obstacles[num].health = +5;
        }
        //Danger
        else if (probability < pNormal + pRepair + pDanger)
        {
            obstacles[num].type = danger;
            obstacles[num].speed = sDanger;
            obstacles[num].color = sf::Color::Red;
            obstacles[num].tex = &dangertexture;
            obstacles[num].size = sf::Vector2f(30.f, 30.f);
            obstacles[num].points = 0;
            obstacles[num].health = -1;
        }
        //Power
        else if (probability < pNormal + pRepair + pDanger + pPower)
        {
            obstacles[num].type = power;
            obstacles[num].speed = sPower;
            obstacles[num].color = sf::Color::Green;
            obstacles[num].tex = &powertexture;
            obstacles[num].size = sf::Vector2f(25.f, 25.f);
            obstacles[num].points = 100;
            obstacles[num].health = 0;
        }
        //super danger
        else if (probability < pNormal + pRepair + pDanger + pPower + pSuperDanger)
        {
            obstacles[num].type = superDanger;
            obstacles[num].speed = sSuperDanger;
            obstacles[num].color = sf::Color::Red;
            obstacles[num].tex = &sdangertexture;
            obstacles[num].size = sf::Vector2f(50.f, 50.f);
            obstacles[num].points = 0;
            obstacles[num].health = -4;
        }

        obstacles[num].pos.x = rand() % int(windowWidth - obstacles[num].size.x);
        obstacles[num].pos.y = -obstacles[num].size.y;
    }

    void initLevel(int x)
    {
        int level = x - 1;
        lvlSpeed = level * 2;

        sNormal = lvl.normal[level][speedNum] + lvlSpeed,
        sRepair = lvl.repair[level][speedNum] + lvlSpeed,
        sDanger = lvl.danger[level][speedNum] + lvlSpeed,
        sPower = lvl.power[level][speedNum] + lvlSpeed,
        sSuperDanger = lvl.superdanger[level][speedNum] + lvlSpeed;

        pNormal = lvl.normal[level][probabilityNum],
        pRepair = lvl.repair[level][probabilityNum],
        pDanger = lvl.danger[level][probabilityNum],
        pPower = lvl.power[level][probabilityNum],
        pSuperDanger = lvl.superdanger[level][probabilityNum];

        all = pNormal + pRepair + pDanger + pPower + pSuperDanger;

        //change speed of all objects after level change
        if (firstInitLevel == true)
        {
            for (int i = 0; i < 10; i++)
            {
                switch (obstacles[i].type)
                {
                case normal:
                    obstacles[i].speed = sNormal;
                    break;
                case repair:
                    obstacles[i].speed = sRepair;
                    break;
                case danger:
                    obstacles[i].speed = sDanger;
                    break;
                case power:
                    obstacles[i].speed = sPower;
                    break;
                case superDanger:
                    obstacles[i].speed = sSuperDanger;
                    break;
                }
            }
        }
        else
            firstInitLevel = true;
    }

public:
    Game()
    {
        window = new sf::RenderWindow(sf::VideoMode(windowWidth + 20 + 10, windowHeight), "window", sf::Style::Titlebar | sf::Style::Close);
        window->setFramerateLimit(80);

        ShipSize.x = 50.f;
        ShipSize.y = 50.f;

        // sship.setSize(ShipSize);
        sship.setPosition(ShipSize.x * 8, ShipSize.y * 11);
        // sship.setFillColor(sf::Color::White);

        obstacleRect.setSize(sf::Vector2f(30.f, 30.f));
        obstacleRect.setFillColor(sf::Color::Black);

        healthRect.setSize(sf::Vector2f(20.f, windowHeight));
        healthRect.setPosition(windowWidth, 0.f);
        healthRect.setFillColor(sf::Color::Red);

        lvlPointsRect.setSize(sf::Vector2f(10.f, windowHeight));
        lvlPointsRect.setPosition(windowWidth + 20, pointsRectY);
        lvlPointsRect.setFillColor(sf::Color::White);

        shiptexture.loadFromFile("assets/ship.png");
        sship.setTexture(shiptexture);
        normaltexture.loadFromFile("assets/normal.png");
        normalSprite.setTexture(normaltexture);
        dangertexture.loadFromFile("assets/danger.png");
        dangerSprite.setTexture(dangertexture);
        sdangertexture.loadFromFile("assets/sdanger.png");
        sdangerSprite.setTexture(sdangertexture);
        powertexture.loadFromFile("assets/power.png");
        powerSprite.setTexture(powertexture);
        repairtexture.loadFromFile("assets/health.png");
        repairSprite.setTexture(repairtexture);

        //text
        if (!this->font.loadFromFile("assets/fonts/SIXTY.TTF"))
        {
            std::cout << " ERROR ::Game.cpp::initFonts::failed to load font file" << std::endl;
        }
        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(35);
        this->uiText.setFillColor(sf::Color::White);
        this->uiText.setString("none");

        //level
        initLevel(level);
    }

    bool isRunning()
    {
        return running;
    }

    int getPoints()
    {
        return points;
    }

    void input()
    {
        while (window->pollEvent(ev))
        {
            switch (ev.type)
            {
            case sf::Event::Closed:
                window->close();
                running = false;
                break;

                //Controlling without spacebar:
                // case sf::Event::KeyPressed:
                //     // std::cout<<"keyress"<<std::endl;
                //     if (ev.key.code == sf::Keyboard::Escape)
                //     {
                //         window->close();
                //         running = false;
                //     }
                //         else if (ev.key.code == sf::Keyboard::Left)
                //         {
                // //            firstpress++;

                //             if (leftPressed == false || KeyTimer>40.f)
                //             {
                //                 dir =-5.f;
                //             }else dir =0.f;
                //                 KeyTimer = 0;

                //             sship.move(-10.f, 0.f);
                //             leftPressed = true;
                //         }
                //         else if (ev.key.code == sf::Keyboard::Right)
                //         {
                //             if (leftPressed==true || KeyTimer>40.f)
                //             {
                //                 dir =5.f;
                //             }else dir =0.f;

                //                 KeyTimer = 0;

                //             sship.move(+10.f, 0.f);
                //             leftPressed = false;
                //         }

                //controling with spacebar
            case sf::Event::KeyPressed:
                // std::cout<<"keyress"<<std::endl;
                if (ev.key.code == sf::Keyboard::Escape)
                {
                    window->close();
                    running = false;
                }
                else if (ev.key.code == sf::Keyboard::Left)
                {
                    // firstpress++;
                    dir = -5.f;
                }
                else if (ev.key.code == sf::Keyboard::Right)
                {
                    dir = 5.f;
                }
                else if (ev.key.code == sf::Keyboard::Space)
                {
                    dir = 0.f;
                }
                else
                {
                    switch (ev.key.code)
                    {
                    case sf::Keyboard::N:
                        pNormal++;
                        break;
                    case sf::Keyboard::R:
                        pRepair++;
                        break;
                    case sf::Keyboard::D:
                        pDanger++;
                        break;
                    case sf::Keyboard::S:
                        pSuperDanger++;
                        break;
                    case sf::Keyboard::P:
                        pPower++;
                        break;
                    case sf::Keyboard::Comma:
                        pNormal--;
                        pRepair--;
                        pDanger--;
                        pSuperDanger--;
                        pPower--;
                        break;
                    case sf::Keyboard::I:
                        std::cout
                            << "\n\n\nnormal : " << pNormal
                            << "\nrepair : " << pRepair
                            << "\ndanger : " << pDanger
                            << "\nsuper danger : " << pSuperDanger
                            << "\npower : " << pPower
                            << "\nall : " << all
                            << "\n\n\n(pointsForLevelUp*level) : " << (pointsForLevelUp * level)
                            << "\n(points - pointsAtLevelup) : " << (points - pointsAtLevelup) << std::endl;
                        break;
                    }
                    all = pNormal + pRepair + pDanger + pPower + pSuperDanger;
                }

                break;

            default:
                break;
            }
        }
    }

    void update()
    {

        //first rain of ememies only
        if (timer >= spawnTime && obsNo < 10)
        {
            timer = 0.f;
            initObstacle(obsNo);

            obsNo++;
        }

        //controling without spacebar:
        // if (KeyTimer<=36.f)
        // {
        //     sship.move(dir, 0.f);
        // }

        //controling ship with spacebar:
        if (sship.getPosition().x >= 0 && dir < 0.f)
            sship.move(dir, 0.f);
        else if (sship.getPosition().x < windowWidth - sship.getTextureRect().width && dir > 0.f)
            sship.move(dir, 0.f);

        //update obstacles
        for (int i = 0; i < 10; i++)
        {
            obstacles[i].pos.y += obstacles[i].speed;
            if (obstacles[i].pos.y > windowHeight && timer >= spawnTime)
            {
                timer = 0.f;
                initObstacle(i);
            }

            //collision of ship and obstacle
            if (obstacles[i].pos.y > windowHeight - sship.getTextureRect().height - obstacleRect.getSize().y && obstacles[i].pos.x > sship.getPosition().x - obstacles[i].size.x && obstacles[i].pos.x < sship.getPosition().x + sship.getTextureRect().width)
            {
                obstacles[i].pos.x = -100.f;

                //gain more points at higher levels
                points += (obstacles[i].points * level);

                //powerUp when touch power object
                if (invincible == false)
                {
                    if (obstacles[i].type == power)
                    {
                        invincible = true;
                        invinciTimer = 0.f;
                    }
                    else if (invinciTimer >= 300)
                    {
                        invincible = false;
                        invinciTimer = 0.f;
                    }
                }

                //level Up after every "pointsToLevelUp" points (upto level 3)
                if (points - pointsAtLevelup >= pointsForLevelUp * level && level < 3 && health > 4)
                {
                    pointsAtLevelup = points;
                    level++;
                    if (level == 2)
                        lvlPointsRect.setFillColor(sf::Color::Yellow);
                    else if (level == 3)
                        lvlPointsRect.setFillColor(sf::Color::Green);

                    initLevel(level);
                    Sleep(2000);
                }

                //health
                if (obstacles[i].health != 0)
                {
                    health += obstacles[i].health;

                    //end health flash and reset color and points
                    if (healthFlash == true && health > 4)
                    {
                        healthFlash = false;
                        healthRect.setFillColor(sf::Color::Red);
                        pointsRectY = windowHeight;
                        if (points >= pointsForLevelUp / 2)
                        {
                            pointsAtLevelup = points - pointsForLevelUp / 2;
                        }
                    }
                    else if (health < 5)
                    {
                        //decrease level when hit badly;
                        if (level > 1)
                        {
                            level = 1;
                            lvlPointsRect.setFillColor(sf::Color::White);
                            pointsAtLevelup = points - 80;
                            initLevel(level);
                        }

                        //Blink health bar when hit badly
                        healthFlash = true;

                        //endgame when health 0
                        if (health < 1)
                        {
                            running = false;
                            health = 0;

                            ////////////////////////////////////////////////////////////////////////score
                            std::cout << "Your score : " << points << std::endl;

                            hs1.setScore(points);

                            std::fstream file, fout;

                            file.open("highscore.dat", std::ios::in | std::ios::binary);
                            fout.open("temp", std::ios::out | std::ios::binary);

                            bool inserted = false;
                            for (int j = 0; j < 5; j++)
                            {
                                file.read((char *)&hs, sizeof(hs));

                                if (hs1.score > hs.score && inserted == false)
                                {
                                    hs1.setName();
                                    fout.write((char *)&hs1, sizeof(hs1));
                                    fout.write((char *)&hs, sizeof(hs));
                                    inserted = true;
                                }
                                else
                                {
                                    fout.write((char *)&hs, sizeof(hs));
                                }
                            }

                            file.close();
                            fout.close();

                            remove("highscore.dat");
                            rename("temp", "highscore.dat");

                            file.open("highscore.dat", std::ios::in | std::ios::binary);
                            std::cout << "Highscore list : " << std::endl;
                            for (int j = 0; j < 5; j++)
                            {
                                file.read((char *)&hs, sizeof(hs));
                                std::cout << "     " << j + 1 << ") " << hs.name << " : " << hs.score << std::endl;
                            }
                        }
                    }
                    //health limit
                    else if (health > maxHealth)
                        health = maxHealth;

                    //update health bar expected position
                    healthRectY = windowHeight - (health * windowHeight / maxHealth);
                }
            }
        }

        // if (firstpress==2)
        //     loops += 1;
        // else if (firstpress==3)
        //     std::cout<<loops<<std::endl;

        //text
        std::stringstream ss;
        ss << "Health : " << health << "\n"
           << "Points : " << points << "\n";

        uiText.setString(ss.str());

        //time
        timer += 1.f;
        KeyTimer += 1.f;
        if (healthFlash == true)
            flashTimer += 1.f;
        if (invincible == true)
            invinciTimer += 1.f;
        if (timer > 100.f)
            timer = 0;
    }

    void render()
    {
        window->clear(sf::Color::Black);

        //obstacles
        for (int i = 0; i < 10; i++)
        {
            obstacleRect.setPosition(obstacles[i].pos);
            obstacleRect.setSize(obstacles[i].size);
            obstacleRect.setTexture(obstacles[i].tex);
            obstacleRect.setFillColor(obstacles[i].color);
            window->draw(obstacleRect);
        }

        // window->draw(sship);

        //flash health when hit badly
        if (healthFlash == true && flashTimer >= spawnTime)
        {
            flashTimer = 0.f;
            if (healthRect.getFillColor() == sf::Color::Red)
                healthRect.setFillColor(sf::Color::Black);
            else
                healthRect.setFillColor(sf::Color::Red);
        }
        //update health bar
        if (healthRectApproachY < healthRectY)
            healthRectApproachY += 9.f;
        else if (healthRectApproachY > healthRectY)
            healthRectApproachY -= 8.f;
        healthRect.setPosition(windowWidth, healthRectApproachY);

        window->draw(healthRect);

        //update points bar
        if (healthFlash == false)
        {
            if (pointsRectY < windowHeight - ((points - pointsAtLevelup) * windowHeight / (pointsForLevelUp * level)))
                pointsRectY += 4.f;
            else if (pointsRectY > windowHeight - ((points - pointsAtLevelup) * windowHeight / (pointsForLevelUp * level)))
                pointsRectY -= 1.f;
            lvlPointsRect.setPosition(windowWidth + 20, pointsRectY);
            window->draw(lvlPointsRect);
        }

        sship.setPosition(sship.getPosition().x, sship.getPosition().y);
        window->draw(sship);
        window->draw(normalSprite);

        window->draw(uiText);
        window->display();
    }
};

#endif