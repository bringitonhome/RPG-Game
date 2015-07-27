//***********************************************
// include guard
#ifndef CHARACTER_CPP
#define CHARACTER_CPP

//***********************************************
// include dependencies
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "constants.h" //TODO this has to come before "character.h". Fix this.
#include "character.h"

//***********************************************
Character::Character(){
    attributes[ATTACK] = 50 + (rand() % 50);
    attributes[DEFENSE] = 50 + (rand() % 50);
    attributes[EVADE] = 50 + (rand() % 50);
    attributes[LUCK] = 50 + (rand() % 50);
    attributes[SPEED] = 50 + (rand() % 50);
    selected = false;
    ready = false;
    alive = true;
    atb = 0;
}

int Character::getAttribute(int attribute){
	return attributes[attribute];
}

sf::Sprite Character::getSprite(){
	return sprite;
}

void Character::setSprite(sf::Texture* texture, int row){
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 20*row, 10, 20));
	sprite.setScale(SCALE, SCALE);
}
void Character::setPosition(int x, int y){
	sprite.setPosition(x, y);
}
void Character::moveSprite(int spriteAlternate){
	sprite.move(10*spriteAlternate, 0);
}
void Character::animateSprite(int numLoop, int spriteSheetRow, int numImages){
	sprite.setTextureRect(sf::IntRect((numLoop % numImages)*10, 20*spriteSheetRow, 10, 20));
}
void Character::setSelected(bool isSelected){
    selected = isSelected;
}
bool Character::isSelected(){
    return selected;
}
void Character::increaseATB(){
    if(atb < MAXTIME && alive == true){
        atb += attributes[SPEED];
    }
    if(atb > MAXTIME && alive == true){
        atb = MAXTIME;
        ready = true;
    }
}
bool Character::isReady(){
    return ready;
}

sf::RectangleShape Character::getTimeBar(){
    sf::RectangleShape rect(sf::Vector2f(atb/100, 10));
    rect.setFillColor(sf::Color::Cyan);
    rect.setPosition(sprite.getPosition().x - 25,
                        sprite.getPosition().y + 110);
    return rect;
}
sf::RectangleShape Character::getTimeBarOutline(){
    sf::RectangleShape rect(sf::Vector2f(100, 10));
    rect.setFillColor(sf::Color::Transparent);
    rect.setPosition(sprite.getPosition().x - 25,
                               sprite.getPosition().y + 110);
    rect.setOutlineThickness(3);
    rect.setOutlineColor(sf::Color::Black);

    return rect;
}
int Character::getATB(){
    return atb;
}



//***********************************************
#endif //CHARACTER_CPP
