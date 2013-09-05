#ifndef TERRAIN_GENERATOR_HPP
#define TERRAIN_GENERATOR_HPP

#include <random>
#include <vector>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "Globals.hpp"
#include "NumberPair.hpp"

typedef std::mt19937 Random;
typedef std::uniform_int_distribution<> UniIntDist;

enum class Variance { HIGH, MEDIUM, LOW };

class TerrainGenerator
{
private:
	bool ready;
	std::vector<NumberPair*> points;
	int terrainWidth, terrainHeight;
	int currentLevel;

	Random rand;
	UniIntDist dist;
	
	sf::RenderTexture renderTex;
	sf::Sprite terrainSprite;
	Variance variance;
	
	int generateMidpoint(int min, int max);
	int generateMidpoint();
	

public:
	TerrainGenerator();
	TerrainGenerator(int width, int height);

	void generatePoints(int startLevel, int level);
	void generateNextLevel();
	void revertBackLevel();

	void drawPointsToTex();
	sf::Sprite *getSprite();
	
	bool reduceVariance();
	bool increaseVariance();

	bool Ready();
	int CurrentLevel();
	std::string CurrentVariance();
};

#endif