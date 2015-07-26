#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <ctime>

#include "constants.h"

#include "character.h"

using namespace std;

int charSelect(sf::Event event, int selectedChar){
    bool upPressed = false;
    bool downPressed = false;

    if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::Up){
            upPressed = true;
        }
        if(event.key.code == sf::Keyboard::Down){
            downPressed = true;
        }
    }
    selectedChar += (int)downPressed - (int)upPressed;

    return selectedChar;
}

int main()
{
    /*
    //Start Song
    sf::Music music;
    if(!music.openFromFile("song.wav"))
        return -1;
    music.play();
    */

    //Initialize Variables
    int numLoop = 0;
    int spriteAnimateSlow = 1;
    int selectedChar = 0;

    srand(time(0));

    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "RPG Game");
    window.setPosition(sf::Vector2i(0, 0));
    window.setFramerateLimit(60);

    //Setting up the background image
    sf::Image bgImage;
    if(!bgImage.loadFromFile("BG2.png")){
    }
    sf::Texture bgTexture;
    if(!bgTexture.loadFromImage(bgImage)){
    }
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTexture);

    //Loading Master Sprite sheet
    sf::Image spriteSheet;
    if(!spriteSheet.loadFromFile("MasterSpriteSheet.png")){
    }
    spriteSheet.createMaskFromColor(sf::Color::White);

    sf::Texture charTexture;
    if(!charTexture.loadFromImage(spriteSheet)){
    }

    //Initializing Characters
    Character characters[NUMCHARACTERS];

    for(int x = 0; x < NUMCHARACTERS; x++){
        characters[x].setSprite(&charTexture, x);
        characters[x].setPosition(WW - 150, 75 + x*175);
    }

    //Initializing Enemies
    Character enemies[NUMENEMIES];

    for(int x = 0; x < NUMENEMIES; x ++){
        enemies[x].setSprite(&charTexture, x + 3);
        enemies[x].setPosition(100, 75 + x*175);
    }

    //Display stats
    for(int x = 0; x < NUMCHARACTERS; x++){
        cout << "Character " << x << ":" << endl << endl;
        cout << "Attack: "  << characters[x].getAttribute(ATTACK)  << endl;
        cout << "Defense: " << characters[x].getAttribute(DEFENSE) << endl;
        cout << "Evade: "   << characters[x].getAttribute(EVADE)   << endl;
        cout << "Luck: "    << characters[x].getAttribute(LUCK)    << endl;
        cout << "Speed: "   << characters[x].getAttribute(SPEED)   << endl << endl;
    }
    for(int x = 0; x < NUMENEMIES; x++){
        cout << "Enemy " << x << ":" << endl << endl;
        cout << "Attack: "  << enemies[x].getAttribute(ATTACK)  << endl;
        cout << "Defense: " << enemies[x].getAttribute(DEFENSE) << endl;
        cout << "Evade: "   << enemies[x].getAttribute(EVADE)   << endl;
        cout << "Luck: "    << enemies[x].getAttribute(LUCK)    << endl;
        cout << "Speed: "   << enemies[x].getAttribute(SPEED)   << endl << endl;
    }


    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            selectedChar = charSelect(event, selectedChar);


        }


        cout << selectedChar << endl;
        //Animations

        //Full Speed
        if(numLoop % DELAY == 0){
            for(int x = 0; x < NUMCHARACTERS; x++){
                characters[x].animateSprite(numLoop/(DELAY), x, 8);
            }
            for(int x = 0; x < NUMENEMIES; x++){
                enemies[x].animateSprite(numLoop/(DELAY), x + 3, 6);
            }
        }
        //Half Speed
        if(numLoop % (DELAY*2) == 0){
            for(int x = 0; x < NUMCHARACTERS; x++){
                characters[x].moveSprite(spriteAnimateSlow);
                spriteAnimateSlow *= -1;
            }
            for(int x = 0; x < NUMENEMIES; x++){
                enemies[x].moveSprite(spriteAnimateSlow);
                spriteAnimateSlow *= -1;
            }

            spriteAnimateSlow *= -1;
        }
        //Quarter Speed
        if(numLoop % (DELAY*4) == 0){

        }

        //Eighth Speed
        if(numLoop % (DELAY*8) == 0){

        }


        numLoop += 1;



        //Display Everything

        window.clear(sf::Color::Black);
        window.draw(bgSprite);

        for(int x = 0; x < NUMCHARACTERS; x++){
            window.draw(characters[x].getSprite());
        }
        for(int x = 0; x < NUMENEMIES; x++){
            window.draw(enemies[x].getSprite());
        }

        window.display();

    }
}
