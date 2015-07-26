#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

//Window Constants
#define WW 800
#define WH 600
#define SCALE 5

#define NUMATTRIBUTES 5
#define ATTACK 0
#define DEFENSE 1
#define EVADE 2
#define LUCK 3
#define SPEED 4


#define NUMCHARACTERS 3

using namespace std;

class Character{
public:
    Character();

    int getAttribute(int attribute){
        return attributes[attribute];
    }
    sf::Sprite getSprite(){
        return sprite;
    }

    void setSprite(sf::Texture* texture, int row){
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(0, 20*row, 10, 20));
        sprite.setScale(SCALE, SCALE);
    }
    void setPosition(int x, int y){
        sprite.setPosition(x, y);
    }

private:
    int attributes[NUMATTRIBUTES];
    sf::Sprite sprite;



};

Character::Character()
{
    attributes[ATTACK] = (rand() % 100) + 1;
    attributes[DEFENSE] = (rand() % 100) + 1;
    attributes[EVADE] = (rand() % 100) + 1;
    attributes[LUCK] = (rand() % 100) + 1;
    attributes[SPEED] = (rand() % 100) + 1;
}

int main()
{
    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "RPG Game");
    window.setPosition(sf::Vector2i(0, 0));

    //Setting up the background image
    sf::Image bgImage;
    if(!bgImage.loadFromFile("BG2.png")){
    }
    sf::Texture bgTexture;
    if(!bgTexture.loadFromImage(bgImage)){
    }
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTexture);

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
        characters[x].setPosition(WW - 150, 75 + x*175);
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
        }

        window.clear(sf::Color::Black);
        window.draw(bgSprite);

        for(int x = 0; x < NUMCHARACTERS; x++){
            window.draw(characters[x].getSprite());
        }

        window.display();
    }
}
