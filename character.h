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
class Character{
public:
    Character();
    int getAttribute(int);
    sf::Sprite getSprite(void);
    void setSprite(sf::Texture*, int);
    void setPosition(int, int);
    void moveSprite(int);
    void animateSprite(int, int, int);
    void setSelected(bool);
    void increaseATB();
    void setDamage(int);
    void resetATB();
    int getATB();
    bool isSelected();
    bool isReady();
    bool isAlive();
    sf::RectangleShape getTimeBar();
    sf::RectangleShape getTimeBarOutline();
    sf::RectangleShape getHealthBar();

private:
    int attributes[NUMATTRIBUTES];
    int health;
    sf::Sprite sprite;
    bool selected;
    bool ready;
    bool alive;
    int atb;
};


//***********************************************
#endif // _CHARACTER_H
