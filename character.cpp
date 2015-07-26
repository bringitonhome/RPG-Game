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
Character::Character()
{
    attributes[ATTACK] = (rand() % 100) + 1;
    attributes[DEFENSE] = (rand() % 100) + 1;
    attributes[EVADE] = (rand() % 100) + 1;
    attributes[LUCK] = (rand() % 100) + 1;
    attributes[SPEED] = (rand() % 100) + 1;
}

int Character::getAttribute(int attribute)
{
	return attributes[attribute];
}

sf::Sprite Character::getSprite()
{
	return sprite;
}

void Character::setSprite(sf::Texture* texture, int row){
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 20*row, 10, 20));
	sprite.setScale(SCALE, SCALE);
}
void Character:: setPosition(int x, int y){
	sprite.setPosition(x, y);
}
void Character:: moveSprite(int spriteAlternate){
	sprite.move(10*spriteAlternate, 0);
}
void Character:: animateSprite(int numLoop, int spriteSheetRow, int numImages){
	sprite.setTextureRect(sf::IntRect((numLoop % numImages)*10, 20*spriteSheetRow, 10, 20));
}

//***********************************************
#endif //CHARACTER_CPP
