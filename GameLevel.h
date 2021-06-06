#pragma once
#include "Resource_Manager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"

#include <vector>

class GameLevel
{
public:
	// level state
	std::vector<GameObject> Walls;
	std::vector<GameObject> Enemies;
	// c-tor
	GameLevel() { }
	// loads level from file
	void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	// render level
	void Draw(SpriteRenderer& renderer);
	// check if the level is completed (all enemies are killed)
	bool IsCompleted();
private:
	// initialize level from the tile data
	void init(std::vector<std::vector<unsigned int>> wallsData,
		std::vector<std::vector<unsigned int>> enemiesData,
		unsigned int levelWidth, unsigned int levelHeight);
};