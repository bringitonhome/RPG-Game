//***********************************************
// include guard
#ifndef _CHARACTER_H
#define _CHARACTER_H

//***********************************************
// forward declared dependencies
//class Foo;(just an example)
//class Bar;(just an example)

//***********************************************
// include dependencies
//#include <ctime> (just an example)
//#include "test.h" (just an example)

//***********************************************
class Character
{
public:
    Character();
    int getAttribute(int);
    sf::Sprite getSprite(void);
    void setSprite(sf::Texture*, int);
    void setPosition(int, int);
    void moveSprite(int);
    void animateSprite(int, int, int);
private:
    int attributes[NUMATTRIBUTES];
    sf::Sprite sprite;
};


//***********************************************
#endif // _CHARACTER_H
