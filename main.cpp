#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <ctime>

#include "constants.h"
#include "character.h"

using namespace std;

int charSelect(sf::Event, int, Character*);
sf::Texture loadTexture(string fileName);
void showStats(Character *characters, string characterOrEnemy);

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
    int arrowAlternate = 1;
    int fastAlternate = 1;
    int halfAlternate = 1;
    int quarterAlternate = 1;
    int selectedChar = -1;

    srand(time(0));

    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "RPG Game");
    window.setPosition(sf::Vector2i(200, 0));
    window.setFramerateLimit(60);

    //Load in textures
    sf::Texture textures[3] = {loadTexture("MasterSpriteSheet.png"), loadTexture("BG2.png"), loadTexture("Arrow 9x6.png")};

    //Set Sprites

    //Set up background
    sf::Sprite bgSprite;
    bgSprite.setTexture(textures[1]);

    //Initializing Characters
    Character characters[NUMCHARACTERS];

    for(int x = 0; x < NUMCHARACTERS; x++){
        characters[x].setSprite(&textures[0], x);
        characters[x].setPosition(WW - 150, 75 + x*175);
    }

    //Initializing Enemies
    Character enemies[NUMENEMIES];

    for(int x = 0; x < NUMENEMIES; x ++){
        enemies[x].setSprite(&textures[0], x + 3);
        enemies[x].setPosition(100, 75 + x*175);
    }

    //Inserting Arrow Image
    sf::Sprite arrow;
    arrow.setTexture(textures[2]);
    arrow.setScale(SCALE, SCALE);


    //Display stats
    showStats(characters, "Character");
    showStats(enemies, "Enemy");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
                //if(event.type == sf::Event::KeyReleased){
                    selectedChar = charSelect(event, selectedChar, characters);


                    arrow.setPosition(characters[selectedChar].getSprite().getPosition().x,
                                      characters[selectedChar].getSprite().getPosition().y - 50);
                    arrowAlternate = 1;

                //}

        }

        arrow.setPosition(characters[selectedChar].getSprite().getPosition().x, arrow.getPosition().y);

        //Animations

        //Full Speed
        if(numLoop % DELAY == 0){
            for(int x = 0; x < NUMCHARACTERS; x++){
                characters[x].animateSprite(numLoop/(DELAY), x, 8);
            }
            for(int x = 0; x < NUMENEMIES; x++){
                enemies[x].animateSprite(numLoop/(DELAY), x + 3, 6);
            }



            for(int x = 0; x < NUMCHARACTERS; x++){
                characters[x].increaseATB();
            }
            for(int x = 0; x < NUMENEMIES; x++){
                enemies[x].increaseATB();
            }

            fastAlternate *= -1;
        }

        //Half Speed
        if(numLoop % (DELAY*2) == 0){

            arrow.move(0, arrowAlternate*2*SCALE);
            arrowAlternate *= -1;

            halfAlternate *= -1;
        }

        //Quarter Speed
        if(numLoop % (DELAY*4) == 0){
            for(int x = 0; x < NUMCHARACTERS; x++){
                characters[x].moveSprite(quarterAlternate);
                quarterAlternate *= -1;
            }
            for(int x = 0; x < NUMENEMIES; x++){
                enemies[x].moveSprite(quarterAlternate);
                quarterAlternate *= -1;
            }

            quarterAlternate *= -1;
        }

        //Eighth Speed
        if(numLoop % (DELAY*8) == 0){

        }

        numLoop += 1;

        //Display Everything

        window.clear(sf::Color::Black);
        window.draw(bgSprite);


        window.draw(arrow);


        for(int x = 0; x < NUMCHARACTERS; x++){
            window.draw(characters[x].getSprite());
            window.draw(characters[x].getTimeBar());
            window.draw(characters[x].getTimeBarOutline());
        }

        for(int x = 0; x < NUMENEMIES; x++){
            window.draw(enemies[x].getSprite());
            window.draw(enemies[x].getTimeBar());
            window.draw(enemies[x].getTimeBarOutline());
        }

        window.display();
    }
}

int charSelect(sf::Event event, int selectedChar, Character *characters){

    int x = 0;
    int readyCheck = 0;

    for(int x = 0; x < NUMCHARACTERS; x++){
        readyCheck += (int)characters[x].isReady();
    }

    if(readyCheck == 0){
        selectedChar = -1;
    }

    if(readyCheck > 0){
        characters[selectedChar].setSelected(false);
        while(characters[selectedChar].isReady() == false){
            selectedChar ++;
        }
        characters[selectedChar].setSelected(true);
    }

    if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::Up){
            //upPressed = true;
            selectedChar = (selectedChar - 1 + NUMCHARACTERS)%NUMCHARACTERS;
            while(characters[selectedChar].isReady() == false && x < NUMCHARACTERS){
                selectedChar = (selectedChar - 1 + NUMCHARACTERS)%NUMCHARACTERS;
                x++;
            }
        }

        if(event.key.code == sf::Keyboard::Down){
            //downPressed = true;
            selectedChar = (selectedChar + 1 + NUMCHARACTERS)%NUMCHARACTERS;
            while(characters[selectedChar].isReady() == false && x < NUMCHARACTERS){
                selectedChar = (selectedChar + 1 + NUMCHARACTERS)%NUMCHARACTERS;
                x++;
            }
        }
    }

    //selectedChar = (selectedChar + (int)downPressed - (int)upPressed + NUMCHARACTERS)%NUMCHARACTERS;

    return selectedChar;
}

sf::Texture loadTexture(string fileName){
    sf::Image image;
    if(!image.loadFromFile(fileName)){
    }
    image.createMaskFromColor(sf::Color::White);
    sf::Texture texture;
    if(!texture.loadFromImage(image)){
    }
    return texture;
}

void showStats(Character *characters, string characterOrEnemy){

    for(int x = 0; x < NUMCHARACTERS; x++){
        cout << characterOrEnemy<< " " << x << ":" << endl << endl;
        cout << "Attack: "  << characters[x].getAttribute(ATTACK)  << endl;
        cout << "Defense: " << characters[x].getAttribute(DEFENSE) << endl;
        cout << "Evade: "   << characters[x].getAttribute(EVADE)   << endl;
        cout << "Luck: "    << characters[x].getAttribute(LUCK)    << endl;
        cout << "Speed: "   << characters[x].getAttribute(SPEED)   << endl << endl;
    }
}





