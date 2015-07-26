#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <typeinfo>


//Graphic Constants
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SCALE 5
#define FONTSIZE 30

//Speed Constant
#define DELAY 40


#define NUM_ATTRIBUTES 5
#define ATTACK 0
#define DEFENSE 1
#define EVADE 2
#define LUCK 3
#define SPEED 4

#define MAXLEVEL 98
#define CRITICAL_MULT 1.5
#define CRITICAL_CHANCE 10
#define NUMRESULTS 3

#define NUMCHARACTERS 3


using namespace std;


class Character{

    public:

        Character();

        int getAttributes(int attribute){
            return attributes[attribute];
        }

        void setSprite(sf::Texture* texture, int spriteSheetRow){
            sprite.setTexture(*texture);
            sprite.setTextureRect(sf::IntRect(0, 20*spriteSheetRow, 10, 20));
            sprite.setScale(SCALE, SCALE);
        }

        void setPosition(int x, int y){
            sprite.setPosition(x, y);
        }

        void moveSprite(int spriteAlternate){
            sprite.move(10*spriteAlternate, 0);
        }

        void animateSprite(int numLoop, int spriteSheetRow, int numImages){
            sprite.setTextureRect(sf::IntRect((numLoop % numImages)*10 , 20*spriteSheetRow, 10, 20));
        }

        sf::Sprite getSprite(){
            return sprite;
        }

        void setHealth(int damage){
            characterHealth -= damage;
            if(characterHealth < 0){
                characterHealth = 0;
                alive = false;
            }
        }

        int getHealth(){
            return characterHealth;
        }

        void addTime(){
            if(characterTime <= 10000){
                characterTime += attributes[SPEED]*2;
            }
            if(characterTime > 10000){
                characterTime = 10000;
            }
        }

        sf::RectangleShape getTimeBar(){
            sf::RectangleShape timeBar(sf::Vector2f(characterTime/100, 10));
            timeBar.setFillColor(sf::Color::Cyan);
            timeBar.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y + 110);

            return timeBar;
        }

        sf::RectangleShape getTimeBarOutline(){
            sf::RectangleShape timeBarOutline(sf::Vector2f(100, 10));
            timeBarOutline.setFillColor(sf::Color::Transparent);
            timeBarOutline.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y + 110);
            timeBarOutline.setOutlineThickness(3);
            timeBarOutline.setOutlineColor(sf::Color::Black);

            return timeBarOutline;
        }

        sf::RectangleShape getHealthBarOutline(){
            sf::RectangleShape healthBarOutline(sf::Vector2f(100, 10));
            healthBarOutline.setFillColor(sf::Color::Transparent);
            healthBarOutline.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y - 20);
            healthBarOutline.setOutlineThickness(3);
            healthBarOutline.setOutlineColor(sf::Color::Black);

            return healthBarOutline;
        }

        void checkReady(int numLoop){
            if(numLoop % 100 == 100 - attributes[SPEED]){
                ready = true;
            }
        }


        bool isReady(){
            return ready;
        }

        bool isAlive(){
            return alive;
        }

        sf::RectangleShape getHealthBar(){
            sf::RectangleShape healthBar(sf::Vector2f(characterHealth, 10));
            healthBar.setFillColor(sf::Color(250, 0, 0));
            healthBar.setPosition(sprite.getPosition().x - 25, sprite.getPosition().y - 20);

            return healthBar;
        }

    private:

        int attributes[NUM_ATTRIBUTES];

        sf::Sprite sprite;

        int characterHealth;
        int characterTime;
        bool alive;
        bool ready;
};

Character::Character()
{
    attributes[ATTACK] = rand() % MAXLEVEL + 1;
    attributes[DEFENSE] = rand() % MAXLEVEL + 1;
    attributes[EVADE] = rand() % MAXLEVEL + 1;
    attributes[LUCK] = rand() % MAXLEVEL + 1;
    attributes[SPEED] = rand() % MAXLEVEL + 1;

    characterHealth = 100;
    characterTime = 0;
    alive = true;
    ready = false;
}

int selectingSprite(sf::Event event, int selectedSprite, int numSprites, Character sprites[])
{

    bool upPressed = false;
    bool downPressed = false;
    int nextSprite = 0;
    int deadSprites = 0;

    if(event.key.code == sf::Keyboard::Up)
    {
        upPressed = true;
        nextSprite = -1;
    }
    if(event.key.code == sf::Keyboard::Down)
    {
        downPressed = true;
        nextSprite = 1;
    }

    selectedSprite = (selectedSprite + nextSprite + numSprites) % numSprites;

    while(sprites[selectedSprite].isAlive() == false){
        selectedSprite = (selectedSprite + nextSprite + numSprites) % numSprites;

    }

    return selectedSprite;
}

void battleSystem(Character* attacker, Character* defender, int* turnResults)
{

    int hitTarget = 1;
    int criticalHit = 0;
    int attemptedDamage;
    int reducedDamage;

    //Calculating damage with respect to attacker's ATTACK attribute

        attemptedDamage = ((*attacker).getAttributes(ATTACK)*90 + (rand() % ((*attacker).getAttributes(ATTACK)*10)))/100;

        //Chance of inflicting critical hit


            if(rand()%100 < (*defender).getAttributes(LUCK))
            {
                attemptedDamage *= CRITICAL_MULT;
                criticalHit = true;
            }

    //Reducing inflicted damage with respect to defender's DEFENSE attribute

        reducedDamage = attemptedDamage * (100 - (*defender).getAttributes(DEFENSE))/100;

        //Up to a maximum of hald the DEFENSE attribute

            if(reducedDamage > (*defender).getAttributes(DEFENSE)/2){
                reducedDamage = attemptedDamage - (*defender).getAttributes(DEFENSE)/2;
            }

    //Chance of evading incoming attack

        if(rand()%100 < (*defender).getAttributes(EVADE)/4)
        {
            reducedDamage = 0;
            hitTarget = false;
        }

        turnResults[0] = reducedDamage;
        turnResults[1] = (int)hitTarget;
        turnResults[2] = (int)criticalHit;

        (*defender).setHealth(reducedDamage);

}


int main()
{

    //Initializing variables

        int spriteAnimateSlow = 1;
        int spriteAnimateMedium = 1;
        int numLoop = 0;
        int mediumSpeed = 0;

        //int numCharacters = 3;
        int numEnemies = 3; //rand() % 10;
        int turns[6];
        for(int x = 0; x < 6; x++){
            turns[x] = -1;
        }

        int selectedCharacter = 0;
        int selectedEnemy = 0;
        int turnResults[NUMRESULTS];
        int selectedSprite;

        bool upPressed;
        bool downPressed;
        bool enterPressed;
        bool charactersReady[NUMCHARACTERS];
        bool enemiesReady[numEnemies];

        bool characterSelect = true;
        bool enemySelect = false;
        bool attacking = false;
        bool beingAttacked = false;



        srand(time(0));


    //Initializing Window

        sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "RPG Fight");
        window.setPosition(sf::Vector2i(1500 - WINDOW_WIDTH, 0));

    //Initializing Background

        sf::Image backgroundImage;
        if(!backgroundImage.loadFromFile("BG2.png")){
        }
        sf::Texture backgroundTexture;
        if(!backgroundTexture.loadFromImage(backgroundImage)){
        }
        sf::Sprite backgroundSprite;
        backgroundSprite.setTexture(backgroundTexture);



    //Initializing Characters

        sf::Image charSpriteSheet;
        if(!charSpriteSheet.loadFromFile("Characters 10x20.png")){
        }
        charSpriteSheet.createMaskFromColor(sf::Color::White);

        sf::Texture charTexture;
        if(!charTexture.loadFromImage(charSpriteSheet)){
        }

        Character characters[NUMCHARACTERS];

        for(int x = 0; x < NUMCHARACTERS; x++)
        {
            characters[x].setSprite(&charTexture, x);
            characters[x].setPosition(WINDOW_WIDTH - 150, 75 + x*175);
        }


    //Initializing Enemies


        sf::Image enemSpriteSheet;
        if(!enemSpriteSheet.loadFromFile("Flames 10x20.png")){
        }
        enemSpriteSheet.createMaskFromColor(sf::Color::White);

        sf::Texture enemTexture;
        if(!enemTexture.loadFromImage(enemSpriteSheet)){
        }

        Character enemies[numEnemies];

        for(int x = 0; x < numEnemies; x++)
        {
            enemies[x].setSprite(&enemTexture, x);
            enemies[x].setPosition(100, 75 + x*175);
        }

    //Initializing Pointer Arrows

        sf::Image arrowImage;
        if(!arrowImage.loadFromFile("arrow 9x6.png")){
        }

        sf::Texture arrowTexture;
        if(!arrowTexture.loadFromImage(arrowImage)){
        }

        sf::Sprite arrowSprite;
        arrowSprite.setTexture(arrowTexture);
        arrowSprite.setTextureRect(sf::IntRect(0, 0, 9, 6));
        arrowSprite.setScale(SCALE, SCALE);

    // Initializing Miss sign
        sf::Image missImage;
        if(!missImage.loadFromFile("MISS.png")){
        }
        missImage.createMaskFromColor(sf::Color::White);

        sf::Texture missTexture;
        if(!missTexture.loadFromImage(missImage)){
        }

        sf::Sprite missSprite;
        missSprite.setTexture(missTexture);
        missSprite.setScale(SCALE, SCALE);

    // Initializing Critical sign
        sf::Image critImage;
        if(!critImage.loadFromFile("Critical.png")){
        }
        critImage.createMaskFromColor(sf::Color::White);

        sf::Texture critTexture;
        if(!critTexture.loadFromImage(critImage)){
        }

        sf::Sprite critSprite;
        critSprite.setTexture(critTexture);
        critSprite.setScale(SCALE, SCALE);

    // Importing font
        sf::Font font;
        if(!font.loadFromFile("Freehand521 BT.ttf")){
        }
        sf::Text damageDisplay;

        damageDisplay.setFont(font);
        //damageDisplay.setString("test !");
        damageDisplay.setCharacterSize(FONTSIZE);
        damageDisplay.setColor(sf::Color::Blue);
        damageDisplay.setPosition(-100, 0);

    //debugging stats output


        for (int x = 0; x < NUMCHARACTERS; x++)
        {
            cout << "character " << x << "'s attack is: " << characters[x].getAttributes(ATTACK) << endl;
            cout << "character " << x << "'s defense is: " << characters[x].getAttributes(DEFENSE) << endl;
            cout << "character " << x << "'s evade is: " << characters[x].getAttributes(EVADE) << endl;
            cout << "character " << x << "'s luck is: " << characters[x].getAttributes(LUCK) << endl;
            cout << "character " << x << "'s speed is: " << characters[x].getAttributes(SPEED) << endl;
            cout << endl;
        }
        cout << endl;


        for (int x = 0; x < numEnemies; x++)
        {
            cout << "enemy " << x << "'s attack is: " << enemies[x].getAttributes(ATTACK) << endl;
            cout << "enemy " << x << "'s defense is: " << enemies[x].getAttributes(DEFENSE) << endl;
            cout << "enemy " << x << "'s evade is: " << enemies[x].getAttributes(EVADE) << endl;
            cout << "enemy " << x << "'s luck is: " << enemies[x].getAttributes(LUCK) << endl;
            cout << "enemy " << x << "'s speed is: " << enemies[x].getAttributes(SPEED) << endl;
            cout << endl;
        }
        cout << endl;

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)// || event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }

        }




        //Animate Sprites


            //Slow Animation

            if(numLoop % (DELAY * 8) == 0)
            {

                for(int x = 0; x < NUMCHARACTERS; x++)
                {
                    characters[x].moveSprite(spriteAnimateSlow);
                    spriteAnimateSlow *= -1;
                }


                for(int x = 0; x < numEnemies; x++)
                {
                    enemies[x].moveSprite(spriteAnimateSlow);
                    spriteAnimateSlow *= -1;
                }

                //Check if Ready

                for(int x = 0; x < 6; x++){
                cout << turns[x] << " ";
                }
                cout << endl;


                spriteAnimateSlow *= -1;
            }


            //Medium Animation

            if(numLoop % (DELAY*2) == 0)
            {

                for(int x = 0; x < NUMCHARACTERS; x++){
                    characters[x].animateSprite(mediumSpeed, x, 8);
                }

                for(int x = 0; x < numEnemies; x++){
                    enemies[x].animateSprite(mediumSpeed, x, 6);
                }


                if(characterSelect == true){
                    arrowSprite.setPosition(characters[selectedCharacter].getHealthBar().getPosition().x + 28, characters[selectedCharacter].getHealthBar().getPosition().y - 40 + 2*spriteAnimateMedium);
                }

                if(enemySelect == true){
                    arrowSprite.setPosition(enemies[selectedEnemy].getHealthBar().getPosition().x + 28, enemies[selectedEnemy].getHealthBar().getPosition().y - 40 + 2*spriteAnimateMedium);
                }

                for(int x = 0; x < NUMCHARACTERS; x++){
                    characters[x].addTime();
                }
                for(int x = 0; x < numEnemies; x++){
                    enemies[x].addTime();
                }

                mediumSpeed += 1;
                spriteAnimateMedium *= -1;
            }

            for(int x = 0; x < 6; x++){
                if(characters[x].isReady() == true){
                    for(int y = 0; y < 6; y++){
                        if(turns[y] == -1){
                            turns[y] = x;
                            break;
                        }
                    }
                }

                if(enemies[x].isReady() == true){
                    for(int y = 0; y < 6; y++){
                        if(turns[y] == -1){
                            turns[y] = x + 3;
                            break;
                        }
                    }
                }
            }


            //Fast Animation

            if(numLoop % DELAY == 0)
            {
                if(damageDisplay.getPosition().y > enemies[selectedEnemy].getSprite().getPosition().y - 100 && enemySelect == false){
                    damageDisplay.move(0, -10);
                }

            }

            numLoop += 1;


        //Display everything

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);

            for(int x = 0; x < NUMCHARACTERS; x++)
            {
                window.draw(characters[x].getSprite());
                window.draw(characters[x].getHealthBar());
                window.draw(characters[x].getTimeBar());
                window.draw(characters[x].getTimeBarOutline());
                window.draw(characters[x].getHealthBarOutline());
            }

            for(int x = 0; x < numEnemies; x++)
            {
                window.draw(enemies[x].getSprite());
                window.draw(enemies[x].getHealthBar());
                window.draw(enemies[x].getTimeBar());
                window.draw(enemies[x].getTimeBarOutline());
                window.draw(enemies[x].getHealthBarOutline());
            }

            window.draw(arrowSprite);


            if(damageDisplay.getPosition().y > enemies[selectedEnemy].getSprite().getPosition().y - 100 && enemySelect == false){
                window.draw(damageDisplay);
            }


            window.display();

    }
}
