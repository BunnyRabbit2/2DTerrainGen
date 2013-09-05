#include "TerrainGenerator.hpp"

TerrainGenerator::TerrainGenerator()
{
	TerrainGenerator(WINDOW_WIDTH, WINDOW_HEIGHT);
}

TerrainGenerator::TerrainGenerator(int width, int height)
{
	// std::random_device rd;
	rand.seed(std::time(0));
	dist.param(std::uniform_int_distribution<>::param_type(0, height));

	variance = Variance::MEDIUM;

	terrainWidth = width;
	terrainHeight = height;

	renderTex.create(terrainWidth, terrainHeight);

	terrainSprite.setTexture(renderTex.getTexture());

	currentLevel = 0;
}

sf::Sprite *TerrainGenerator::getSprite()
{
	return &terrainSprite;
}

bool TerrainGenerator::Ready()
{
	return ready;
}

int TerrainGenerator::CurrentLevel()
{
	return currentLevel;
}

std::string TerrainGenerator::CurrentVariance()
{
	switch (variance) 
	{
		case Variance::LOW :
			return "LOW";
			break;

		case Variance::MEDIUM :
			return "MEDIUM";
			break;

		case Variance::HIGH :
			return "HIGH";
			break;

		default:
			return "NO VALUE";
			break;
	}
}

bool TerrainGenerator::reduceVariance()
{
	if(variance == Variance::HIGH)
	{
		variance = Variance::MEDIUM;
		return true;
	}
	else if(variance == Variance::MEDIUM)
	{
		variance = Variance::LOW;
		return true;
	}
	else
		return false;
}

bool TerrainGenerator::increaseVariance()
{
	if(variance == Variance::LOW)
	{
		variance = Variance::MEDIUM;
		return true;
	}
	else if(variance == Variance::MEDIUM)
	{
		variance = Variance::HIGH;
		return true;
	}
	else
		return false;
}

int TerrainGenerator::generateMidpoint(int min, int max)
{
	if(min > max)
		std::swap(min, max);

	int temp = max - min;
	// Reduce variance
	switch (variance) 
	{
		case Variance::LOW :
			min += temp * 0.40;
			max -= temp * 0.40;
			break;

		case Variance::MEDIUM :
			min += temp * 0.20;
			max -= temp * 0.20;
			break;

		case Variance::HIGH :
			min += temp * 0.0;
			max -= temp * 0.0;
			break;

		default:
			min += temp * 0.25;
			max -= temp * 0.25;
			break;
	}

	// Old School approach
	// return min + (rand() % (int)(max - min + 1));

	// New School approach
	// UniIntDist dist(min, max);
	// return dist(rand);

	// Mixed School approach
	return min + (dist(rand) % (int)(max - min + 1));
}

void TerrainGenerator::generatePoints(int startLevel, int level)
{
	for(auto p : points)
	{
		delete p;
	}

	points.clear();
	points.shrink_to_fit();

	points.push_back(new NumberPair(0, terrainHeight * 0.75));
	points.push_back(new NumberPair(terrainWidth * 0.5, terrainHeight * 0.25));
	points.push_back(new NumberPair(terrainWidth, terrainHeight * 0.75));

	if(startLevel == 2)
	{
		points.push_back(new NumberPair(terrainWidth * 0.75, terrainHeight * 0.5));
		points.push_back(new NumberPair(terrainWidth * 0.25, terrainHeight * 0.5));
	}

	if(startLevel == 3)
	{
		points.push_back(new NumberPair(terrainWidth * 0.75, terrainHeight * 0.5));
		points.push_back(new NumberPair(terrainWidth * 0.25, terrainHeight * 0.5));

		points.push_back(new NumberPair(terrainWidth * 0.875, terrainHeight * 0.625));
		points.push_back(new NumberPair(terrainWidth * 0.125, terrainHeight * 0.625));

		points.push_back(new NumberPair(terrainWidth * 0.625, terrainHeight * 0.375));
		points.push_back(new NumberPair(terrainWidth * 0.375, terrainHeight * 0.375));
	}

	std::sort(points.begin(), points.end(), compareNumberPair);

	currentLevel = startLevel;

	for(int i = startLevel; i < level; i++)
		generateNextLevel();
}

void TerrainGenerator::generateNextLevel()
{
	int size = points.size();

	for( int i = 0; i < size - 1; i++ )
	{
		points.push_back(new NumberPair(
			points[i+1]->x - ((points[i+1]->x - points[i]->x) / 2),
			generateMidpoint(points[i]->y, points[i+1]->y)));
	}

	std::sort(points.begin(), points.end(), compareNumberPair);

	currentLevel++;
}

void TerrainGenerator::revertBackLevel()
{
	std::vector<NumberPair*> tempV;
	
	for( int i = 0; i < points.size(); i++ )
	{
		if(i % 2 == 0)
			tempV.push_back(points[i]);
	}

	points.clear();
	points.shrink_to_fit();

	for( auto p : tempV )
	{
		points.push_back(p);
	}

	std::sort(points.begin(), points.end(), compareNumberPair);
	points.shrink_to_fit();

	currentLevel--;
}

void TerrainGenerator::drawPointsToTex()
{
	std::sort(points.begin(), points.end(), compareNumberPair);
	points.shrink_to_fit();
	
	sf::CircleShape circle(1.0f);
	circle.setOrigin(circle.getRadius(), circle.getRadius());
	circle.setFillColor(sf::Color::Black);

	renderTex.clear(sf::Color::Transparent);

	// Crazy ass hack to fix the fact the rendertex isn't clearing properly
	sf::RectangleShape screenClearer;
	screenClearer.setSize(sf::Vector2f(terrainWidth, terrainHeight));
    screenClearer.setPosition(0.f, 0.f);
    screenClearer.setFillColor(sf::Color::Transparent);
	
	for( int i = 0; i < points.size(); i++ )
	{
		if(i < points.size() - 1)
		{
			sf::Vertex line[] = 
			{
				sf::Vertex(sf::Vector2f(points[i]->x, points[i]->y)),
				sf::Vertex(sf::Vector2f(points[i + 1]->x, points[i + 1]->y))
			};

			renderTex.draw(line, 2, sf::Lines);
		}

		circle.setPosition(points[i]->x, points[i]->y);
		renderTex.draw(circle);
	}

	renderTex.draw(screenClearer);
	renderTex.display();
}