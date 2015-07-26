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
// special class functions

/**
 * @desc Constructor of the Character() class. Assigns members of Character().
 */
Character::Character()
{
    attributes[ATTACK] = (rand() % 100) + 1;
    attributes[DEFENSE] = (rand() % 100) + 1;
    attributes[EVADE] = (rand() % 100) + 1;
    attributes[LUCK] = (rand() % 100) + 1;
    attributes[SPEED] = (rand() % 100) + 1;
}

//***********************************************
// interface functions

/**
 * @desc retrieve an attribute from Character()
 *
 * @param int attribute - index number to determine which attribute to return
 * @return int - the value of the given attribute
 */
int Character::getAttribute(int attribute)
{
	return attributes[attribute];
}

/**
 * @desc retrieve the Sprite() from Character()
 *
 * @return sf::Sprite - the Sprite() assigned to Character()
 */
sf::Sprite Character::getSprite()
{
	return sprite;
}

/**
 * @desc set the Sprite() for Character() from a sprite sheet Texture()
 *
 * @param sf::Texture* texture - the sprite sheet's Texture()
 * @param int row - select a certain row of the sprite sheet
 * @return sf::Sprite - the Sprite() assigned to Character()
 */
void Character::setSprite(sf::Texture* texture, int row)
{
	sprite.setTexture(*texture);
	sprite.setTextureRect(sf::IntRect(0, 20*row, 10, 20));
	sprite.setScale(SCALE, SCALE);
}

/**
 * @desc set the position of the Character()'s Sprite()
 *
 * @param int x - the new x coordinate for the Sprite()
 * @param int y - the new y coordinate for the Sprite()
 */
void Character:: setPosition(int x, int y)
{
	sprite.setPosition(x, y);
}

//************************************************
// other class functions

void Character:: moveSprite(int spriteAlternate)
{
	sprite.move(10*spriteAlternate, 0);
}

void Character:: animateSprite(int numLoop, int spriteSheetRow, int numImages)
{
	sprite.setTextureRect(sf::IntRect((numLoop % numImages)*10, 20*spriteSheetRow, 10, 20));
}

//***********************************************
#endif //CHARACTER_CPP
