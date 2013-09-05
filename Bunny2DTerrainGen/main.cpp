#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>

#include "Globals.hpp"
#include "TerrainGenerator.hpp"

template <typename T>
std::string ToStr( const T & t )
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT), WINDOW_NAME);

	TerrainGenerator terrainGen(800, 600);
	if(!terrainGen.Ready())
		return 1;

	int startPoints = 1;
	int upToPoints = 3;

	terrainGen.generatePoints(startPoints, upToPoints);
	terrainGen.drawPointsToTex();

	sf::Font font;
	if (!font.loadFromFile("OpenSans-Regular.ttf"))
	{
		return 1;
	}
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setColor(sf::Color::Red);

	while(window.isOpen())
	{
		sf::Event windowEvent;

		while( window.pollEvent(windowEvent) )
		{
			if(windowEvent.type == sf::Event::Closed)
				window.close();

			if(windowEvent.type == sf::Event::KeyPressed)
			{
				bool redraw = false;
				bool regen = false;

				switch (windowEvent.key.code)
				{
					case sf::Keyboard::A :
						if(startPoints > 1)
						{
							startPoints--;
							regen = true;
						}
						break;
					case sf::Keyboard::Q :
						if(startPoints < 3)
						{
							startPoints++;
							regen = true;
						}
						break;

					case sf::Keyboard::S :
						if(upToPoints > startPoints)
						{
							upToPoints--;
							regen = true;
						}
						break;
					case sf::Keyboard::W :
						if(upToPoints < 10)
						{
							upToPoints++;
							regen = true;
						}
						break;

					case sf::Keyboard::D :
						if(terrainGen.CurrentLevel() > upToPoints)
						{
							terrainGen.revertBackLevel();
							redraw = true;
						}
						break;
					case sf::Keyboard::E :
						if(terrainGen.CurrentLevel() < 15)
						{
						terrainGen.generateNextLevel();
						redraw = true;
						}
						break;

					case sf::Keyboard::F :
						regen = terrainGen.reduceVariance();
						break;
					case sf::Keyboard::R :
						regen = terrainGen.increaseVariance();
						break;
				}

				if(regen)
				{
					terrainGen.generatePoints(startPoints, upToPoints);
					terrainGen.drawPointsToTex();
				}
				else if(redraw)
				{
					terrainGen.drawPointsToTex();
				}
			}
		}

		window.clear(sf::Color::Cyan);

		window.draw(*terrainGen.getSprite());

		text.setString("Starting Level: " + ToStr(startPoints));
		text.setPosition(10,10);
		window.draw(text);

		text.setString("Up To Level: " + ToStr(upToPoints));
		text.setPosition(10,35);
		window.draw(text);

		text.setString("Current Level: " + ToStr(terrainGen.CurrentLevel()));
		text.setPosition(10,60);
		window.draw(text);

		text.setString("Current Variance: " + terrainGen.CurrentVariance());
		text.setPosition(10,85);
		window.draw(text);

		window.display();
	}

	return 0;
}