#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

//Window Constants
#define WW 800
#define WH 600

using namespace std;

int main()
{
    //Setting up the window
    sf::RenderWindow window(sf::VideoMode(WW, WH), "RPG Game");
    window.setPosition(sf::Vector2i(0, 0));

    //Setting up the background
    sf::Image bgImage;
    if(!bgImage.loadFromFile("BG2.png")){
    }
    sf::Texture bgTexture;
    if(!bgTexture.loadFromImage(bgImage)){
    }
    sf::Sprite bgSprite;
    bgSprite.setTexture(bgTexture);


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
        window.display();
    }
}
