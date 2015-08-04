#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <ctime>

#include "constants.h"
#include "character.h"

using namespace std;

int charSelect(sf::Event, int, Character*, sf::Sprite*, bool, int);

sf::Texture loadTexture(string);

void showStats(Character*, Character*); //Used for debug. Should hide later
void displayEverything(sf::Sprite*, Character*, Character*, sf::Sprite*, sf::RenderWindow*,int);

void battleSystem(Character* attacker, Character* defender){
    int attemptedDamage;
    int reducedDamage;
    int damageReduction;
    bool critical = false;
    bool miss = false;

    //Attempted damage given attacker's attack attribute
    attemptedDamage = (90*(*attacker).getAttribute(ATTACK)
             + rand()%(10*(*attacker).getAttribute(ATTACK)))/100; //Attempt to do 90-100% of attack power
    //Critical strike check
    if(rand()%100 < (*attacker).getAttribute(LUCK)){
        attemptedDamage *= CRITMULT;
        critical = true;
    }

    //Reduced damage given defender's defense attribute
    reducedDamage = (attemptedDamage*(100 - (*defender).getAttribute(DEFENSE)))/100;
    if(attemptedDamage - reducedDamage > (*defender).getAttribute(DEFENSE)/2){
        reducedDamage = (attemptedDamage*(100 - (*defender).getAttribute(DEFENSE)/2))/100;
    }

    //Miss check
    if(rand()%100 < (*defender).getAttribute(EVADE)/EVADEMULT){
        reducedDamage = 0;
        miss = true;
    }
    (*defender).setDamage(reducedDamage);
    (*attacker).resetATB();
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
    int arrowAlternate = 1;
    int fastAlternate = 1;
    int halfAlternate = 1;
    int quarterAlternate = 1;
    int selectedChar = -1;
    int selectedEnem = 0;

    srand(time(0));

    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "RPG Game");
    window.setPosition(sf::Vector2i(400, 0));
    window.setFramerateLimit(60);

    //Load in textures
    sf::Texture textures[3] = {loadTexture("MasterSpriteSheet.png"), loadTexture("BG2.png"), loadTexture("Arrow 9x6.png")};

    //Prepare sprites

    //Setting up background sprite
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

    //Setting up arrow sprite
    sf::Sprite arrow;
    arrow.setTexture(textures[2]);
    arrow.setScale(SCALE, SCALE);

    //Display stats
    showStats(characters, enemies);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event)){
            //Close window
            if(event.type == sf::Event::Closed){
                window.close();
            }

            //Go into manual selection when an event such as
            //KeyPress or KeyRelease happens
            selectedChar = charSelect(event, selectedChar, characters, &arrow, false, arrowAlternate);
        }

        //Regardless of event occurence, check for autoselect
        //elegibility such as selectedChar = -1 and readyCheck > 0
        selectedChar = charSelect(event, selectedChar, characters, &arrow, true, arrowAlternate);

        for(int attacker = 0; attacker < NUMENEMIES; attacker++){
            if(enemies[attacker].isReady() == true){
                int defender = 0;
                while(characters[defender].isAlive() == false){
                    defender++;
                }
                battleSystem(&enemies[attacker], &characters[defender]);
            }
        }


        //Animations

        //Full Animation Speed (x1 delay)
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

        //Half Animation Speed (x2 delay)
        if(numLoop % (DELAY*2) == 0){

            arrowAlternate *= -1;

            halfAlternate *= -1;
        }

        //Quarter Animation Speed (x4 delay)
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

        //Eight Animation Speed (x8 delay)
        if(numLoop % (DELAY*8) == 0){

        }

        numLoop++;

        //Display everything on the screen
        displayEverything(&bgSprite, characters, enemies, &arrow, &window, selectedChar);
    }
}

int charSelect(sf::Event event, int selectedChar, Character *characters, sf::Sprite* arrow, bool autoSelect, int arrowAlternate){
    int readyCheck = 0;

    //Check if someone is ready
    for(int x = 0; x < NUMCHARACTERS; x++){
        readyCheck += (int)characters[x].isReady();
    }

    if(readyCheck == 0){ //If nobody is ready
        selectedChar = -1; //Set to none selected (-1)
    }

    if(readyCheck > 0){ //If someone is ready

        //Find first available character
        if(autoSelect == true){ //Function called from outside of pollEvent loop. Independent of window event
            if(selectedChar == -1){
                selectedChar = 0;
                while(characters[selectedChar].isReady() == false){
                    selectedChar ++;
                }
            }
        }


        //User selection

        if(event.type == sf::Event::KeyReleased && autoSelect == false){//Function called from inside pollEvent loop. Depends on window event

            if(event.key.code == sf::Keyboard::Up){
                selectedChar = (selectedChar - 1 + NUMCHARACTERS)%NUMCHARACTERS;
                //If the newly selected character is not ready, move on to the next one until back at original
                while(characters[selectedChar].isReady() == false){
                    selectedChar = (selectedChar - 1 + NUMCHARACTERS)%NUMCHARACTERS; // Assures positive number
                }
            }

            if(event.key.code == sf::Keyboard::Down){
                selectedChar = (selectedChar + 1)%NUMCHARACTERS;
                //If the newly selected character is not ready, move on to the next one until back at original
                while(characters[selectedChar].isReady() == false){//} && loopCount < NUMCHARACTERS){
                    selectedChar = (selectedChar + 1)%NUMCHARACTERS;
                }
            }
        }

        //Set the selection arrow's position on top of selected character
        (*arrow).setPosition(characters[selectedChar].getSprite().getPosition().x,
                             characters[selectedChar].getSprite().getPosition().y - 50 + arrowAlternate*5);
    }

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

void showStats(Character *characters, Character *enemies){

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
}

void displayEverything(sf::Sprite* bgSprite, Character *characters, Character *enemies, sf::Sprite* arrow, sf::RenderWindow* window, int selectedChar){

    (*window).clear(sf::Color::Black);

    (*window).draw(*bgSprite);

    for(int x = 0; x < NUMCHARACTERS; x++){
        (*window).draw(characters[x].getSprite());
        (*window).draw(characters[x].getHealthBar());
        (*window).draw(characters[x].getTimeBar());
        (*window).draw(characters[x].getTimeBarOutline());
    }
    for(int x = 0; x < NUMENEMIES; x++){
        (*window).draw(enemies[x].getSprite());
        (*window).draw(enemies[x].getHealthBar());
        (*window).draw(enemies[x].getTimeBar());
        (*window).draw(enemies[x].getTimeBarOutline());
    }

    if(selectedChar != -1){
        (*window).draw(*arrow);
    }

    (*window).display();
}
