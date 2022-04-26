#ifndef Gme_H
#define Gme_h

// add end sprite
// add restart button
// learn to add multiplayer
// add instant win in multiplayer top level

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    int points = 0;
    int health = 0;

    sf::Sprite *spr = NULL;
};

class Level
{
    // use level.type[level][probabilityNum] OR level.type[level][speedNum]
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
    bool running = true, healthFlash = false, invincible = false, firstInitLevel = false, gravity = true;
    sf::RenderWindow *window;
    int windowHeight = 600, windowWidth = 800;
    sf::Event ev;
    sf::RectangleShape healthRect, lvlPointsRect;
    sf::Vector2f ShipSize;

    float pointsRectY = windowHeight, healthRectApproachY = -1.f, healthRectY = 0.f;

    // sprites and textures
    sf::Texture shiptexture, dangertexture, normaltexture, sdangertexture, powertexture, repairtexture;
    sf::Texture bgTexture1, shiptexture1, dangertexture1, normaltexture1, sdangertexture1, repairtexture1;
    sf::Sprite sBg, sship, normalSprite, dangerSprite, sdangerSprite, powerSprite, repairSprite;
    sf::Texture levelUpTex;
    sf::Sprite levelUpSprite;

    int obsNo = 0;
    obstacle obstacles[10];

    Level lvl;
    int level = 1;
    int lvlSpeed = 0, invinciSpeed = 10;
    int points = 0, health = 15, maxHealth = 15, pointsAtLevelup = 0, pointsForLevelUp = 1000;

    float spawnTime = 12.f, timer = 0.f, flashTimer = 0.f, invinciTimer = 0.f;

    float dir = 0.f;

    // text
    sf::Font font;
    sf::Text uiText;

    // probibility--speed
    int pNormal, pRepair, pDanger, pPower, pSuperDanger,
        sNormal, sRepair, sDanger, sPower, sSuperDanger;

    int all;

    // audio
    sf::Music mus;
    sf::SoundBuffer bufNormal, bufDanger, bufPower, bufHealth, bufEndgame, bufLvlUp;
    sf::Sound sndNormal, sndDanger, sndPower, sndHealth, sndEndgame, sndLvlUp;

    // functions
    void initObsFunc(int num, int type, int speed, float sizex, float sizey, int points, int health, sf::Sprite *sprite)
    {
        obstacles[num].type = type;
        obstacles[num].speed = speed;
        obstacles[num].size = sf::Vector2f(sizex, sizey);
        obstacles[num].points = points;
        obstacles[num].health = health;
        obstacles[num].spr = sprite;
    }

    void initObstacle(int num)
    {

        points++;
        int probability = rand() % all;

        // Normal
        if (probability < pNormal)
            initObsFunc(num, normal, sNormal, 40.f, 40.f, 10, 0, &normalSprite);

        // Repair
        else if (probability < pNormal + pRepair)
            initObsFunc(num, repair, sRepair, 20.f, 20.f, 10, +5, &repairSprite);

        // Danger
        else if (probability < pNormal + pRepair + pDanger)
            initObsFunc(num, danger, sDanger, 30.f, 30.f, 0, -1, &dangerSprite);

        // Power
        else if (probability < pNormal + pRepair + pDanger + pPower)
            initObsFunc(num, power, sPower, 25.f, 25.f, 0, 0, &powerSprite);

        // super danger
        else if (probability < pNormal + pRepair + pDanger + pPower + pSuperDanger)
            initObsFunc(num, superDanger, sSuperDanger, 50.f, 50.f, 0, -4, &sdangerSprite);

        obstacles[num].pos.x = rand() % int(windowWidth - obstacles[num].size.x);
        obstacles[num].pos.y = -obstacles[num].size.y;
    }

    void initLevel(int x)
    {
        int level = x - 1;
        lvlSpeed = level * 2;

        // std::cout<<"\n techLevel = "<<level<<std::endl;

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

        // change speed of all objects after level change
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

        sship.setPosition(ShipSize.x * 8, ShipSize.y * 11);

        healthRect.setSize(sf::Vector2f(20.f, windowHeight));
        healthRect.setPosition(windowWidth, 0.f);
        healthRect.setFillColor(sf::Color::Red);

        lvlPointsRect.setSize(sf::Vector2f(10.f, windowHeight));
        lvlPointsRect.setPosition(windowWidth + 20, pointsRectY);
        lvlPointsRect.setFillColor(sf::Color::White);

        bgTexture1.loadFromFile("assets/background.png");
        shiptexture.loadFromFile("assets/ship.png");
        normaltexture.loadFromFile("assets/normal.png");
        dangertexture.loadFromFile("assets/danger.png");
        sdangertexture.loadFromFile("assets/sdanger.png");
        powertexture.loadFromFile("assets/power.png");
        repairtexture.loadFromFile("assets/health.png");
        levelUpTex.loadFromFile("assets/levelUp.png");

        shiptexture1.loadFromFile("assets/ship1.png");
        normaltexture1.loadFromFile("assets/normal1.png");
        dangertexture1.loadFromFile("assets/danger1.png");
        sdangertexture1.loadFromFile("assets/sdanger1.png");
        repairtexture1.loadFromFile("assets/health1.png");

        // sBg.setTexture();
        sship.setTexture(shiptexture);
        normalSprite.setTexture(normaltexture);
        dangerSprite.setTexture(dangertexture);
        sdangerSprite.setTexture(sdangertexture);
        powerSprite.setTexture(powertexture);
        repairSprite.setTexture(repairtexture);
        levelUpSprite.setTexture(levelUpTex);
        sBg.setTexture(bgTexture1);

        // text
        if (!this->font.loadFromFile("assets/fonts/SIXTY.TTF"))
        {
            std::cout << " ERROR ::Game.cpp::initFonts::failed to load font file" << std::endl;
        }
        this->uiText.setFont(this->font);
        this->uiText.setCharacterSize(35);
        this->uiText.setFillColor(sf::Color::White);
        this->uiText.setString("none");

        // level
        initLevel(level);

        // audio
        mus.openFromFile("assets/bg.wav");
        mus.setLoop(true);
        mus.setLoopPoints();
        // mus.setLoopPoints(0.f, mus.getDuration()));
        mus.play();

        bufDanger.loadFromFile("assets/bang.wav");
        bufNormal.loadFromFile("assets/point.wav");
        bufPower.loadFromFile("assets/boost.flac");
        bufHealth.loadFromFile("assets/power.wav");
        bufEndgame.loadFromFile("assets/health flash.wav");
        bufLvlUp.loadFromFile("assets/bang.wav");

        sndNormal.setBuffer(bufNormal);
        sndDanger.setBuffer(bufDanger);
        sndPower.setBuffer(bufPower);
        sndHealth.setBuffer(bufHealth);
        sndEndgame.setBuffer(bufEndgame);
        sndLvlUp.setBuffer(bufNormal);
    }

bool isRunning()
{
    return running;
}

int getPoints()
{
    return points;
}

void devInput(sf::Keyboard::Key)
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

    case sf::Keyboard::G:
        if (gravity == true) gravity = false;
        else gravity = true;
        dir=0.f;
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

            // controling with spacebar
        case sf::Event::KeyPressed:
            devInput(ev.key.code);
        }
    }

        // std::cout<<"keyress"<<std::endl;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        window->close();
        running = false;
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // firstpress++;
        if(!gravity) dir = -5.f;
        else sship.move(-5.f, 0.f);
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (!gravity) dir = 5.f;
        else sship.move(5.f, 0.f);
    }
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        dir = 0.f;
    }
}

void updateHealth(int i)
{
    if (obstacles[i].health == 0 || invincible == true)
        return;

    health += obstacles[i].health;
    if (obstacles[i].health < 0)
        sndDanger.play();
    else
        sndHealth.play();

    // end health flash and reset color and points
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
        // decrease level when hit badly;
        if (level > 1)
        {
            level = 1;
            lvlPointsRect.setFillColor(sf::Color::White);
            pointsAtLevelup = points - 80;
            initLevel(level);
        }

        // Blink health bar when hit badly
        healthFlash = true;

        // endgame when health 0
        if (health < 1)
            endgame();
    }
    // health limit
    else if (health > maxHealth)
        health = maxHealth;

    // update health bar expected position
    healthRectY = windowHeight - (health * windowHeight / maxHealth);
}

void upadateCollision(int i)
{
    obstacles[i].pos.x = -100.f;

    if (invincible == true)
    {
        points += 20;
        sndNormal.play();
    }
    else
    {
        points += (obstacles[i].points * level); // gain more points at higher levels
        sndNormal.play();
    }

    // powerUp when touch power object

    if (obstacles[i].type == power)
    {
        sndPower.play();
        invincible = true;
        invinciTimer = 0.f;
        spawnTime = 0.f;

        sship.setTexture(shiptexture1);
        normalSprite.setTexture(normaltexture1);
        dangerSprite.setTexture(dangertexture1);
        sdangerSprite.setTexture(sdangertexture1);
        repairSprite.setTexture(repairtexture1);
    }
    if (invincible == true && invinciTimer >= 500) // turn off powerup
    {
        invincible = false;

        // sBg.setTexture();
        sship.setTexture(shiptexture);
        normalSprite.setTexture(normaltexture);
        dangerSprite.setTexture(dangertexture);
        sdangerSprite.setTexture(sdangertexture);
        repairSprite.setTexture(repairtexture);
        invinciTimer = 0.f;
        spawnTime = 12.f;
    }

    // level Up after every "pointsToLevelUp" points (upto level 3)
    if (points - pointsAtLevelup >= pointsForLevelUp * level && level < 3 && health > 4)
    {
        pointsAtLevelup = points;
        level++;
        if (level == 2)
            lvlPointsRect.setFillColor(sf::Color::Yellow);
        else if (level == 3)
            lvlPointsRect.setFillColor(sf::Color::Green);

        initLevel(level);
        window->draw(levelUpSprite);
        window->display();
        Sleep(2000);
    }

    // health
    updateHealth(i);
}

void update()
{

    // first rain of ememies only
    if (timer >= spawnTime && obsNo < 10)
    {
        timer = 0.f;
        initObstacle(obsNo);

        obsNo++;
    }

    // move
    sship.move(dir, 0.f);

    //restrict ship from moving outside the screen
    if (sship.getPosition().x < 0)
        sship.move(5.f, 0.f);
    else if (sship.getPosition().x > windowWidth - sship.getTextureRect().width)
        sship.move(-5.f, 0.f);

    // update obstacles
    for (int i = 0; i < 10; i++)
    {
        if (invincible == false)
            obstacles[i].pos.y += obstacles[i].speed;
        else
            obstacles[i].pos.y += 15;

        if (obstacles[i].pos.y > windowHeight && timer >= spawnTime)
        {
            timer = 0.f;
            initObstacle(i);
        }

        // collision of ship and obstacle
        if (obstacles[i].pos.y > windowHeight - sship.getTextureRect().height - obstacles[i].size.y && obstacles[i].pos.x > sship.getPosition().x - obstacles[i].size.x && obstacles[i].pos.x < sship.getPosition().x + sship.getTextureRect().width)
            upadateCollision(i);
    }

    // text
    std::stringstream ss;
    ss << "Health : " << health << "\n"
       << "Points : " << points << "\n";

    uiText.setString(ss.str());

    // time
    timer += 1.f;
    // KeyTimer += 1.f;
    if (healthFlash == true)
        flashTimer += 1.f;
    if (invincible == true)
        invinciTimer += 1.f;
    if (timer > 100.f)
        timer = 0;
}

void endgame()
{
    running = false;
    health = 0;
    sndEndgame.play(); // sound

    ////////////////////////////////////////////////////////////////////////store score
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
            j++;
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

void render()
{
    window->clear(sf::Color::Black);

    window->draw(sBg);

    // obstacles
    for (int i = 0; i < 10; i++)
    {
        if (obstacles[i].spr != NULL)
        {
            obstacles[i].spr->setPosition(obstacles[i].pos);
            window->draw(*obstacles[i].spr);
        }
    }

    // flash health when hit badly
    if (healthFlash == true && flashTimer >= 12.f)
    {
        flashTimer = 0.f;
        if (healthRect.getFillColor() == sf::Color::Red || healthRect.getFillColor() == sf::Color::Blue)
            healthRect.setFillColor(sf::Color::Black);
        else if (invincible == false)
            healthRect.setFillColor(sf::Color::Red);
        else
            healthRect.setFillColor(sf::Color::Blue);
    }

    // update health bar
    if (healthRectApproachY < healthRectY)
        healthRectApproachY += 9.f;
    else if (healthRectApproachY > healthRectY)
        healthRectApproachY -= 8.f;
    healthRect.setPosition(windowWidth, healthRectApproachY);

    window->draw(healthRect);

    // update points bar
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