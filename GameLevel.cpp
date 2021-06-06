#include "GameLevel.h"

#include <iostream>

void GameLevel::Load(const char* file, unsigned int levelWidth, unsigned int levelHeight)
{
	bool debug = false;
	// clear old data
	this->Walls.clear();
	this->Enemies.clear();
	// load from file
	unsigned int wallType, enemyInfo;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> wallData, enemiesData;
	if (fstream)
	{
		std::getline(fstream, line);
		if (line == "WALLS")
		{
			while ((line != "ENEMIES") && std::getline(fstream, line))
			{
				std::istringstream sstream(line); // read each line from level file
				std::vector<unsigned int> row;
				while (sstream >> wallType) // read each word separated by spaces
					row.push_back(wallType);
				if (row.size() > 0)	// SKIP empty lines
					wallData.push_back(row);
			}
		}
		
		if (wallData.size() > 0)
		{
			if (line == "ENEMIES")
			{
				while (std::getline(fstream, line))
				{
					std::istringstream sstream(line); // read each line from level file
					std::vector<unsigned int> row;
					while (sstream >> enemyInfo) // read each word separated by spaces
						row.push_back(enemyInfo);
					if (row.size() > 0)	// SKIP empty lines
						enemiesData.push_back(row);
				}
				if (wallData.size() > 0 && enemiesData.size() > 0)	// debug print of registered data
				{
					if (debug)
					{
						for (int i = 0; i < wallData.size(); ++i)
						{
							for (int j = 0; j < wallData[i].size(); ++j)
							{
								std::cout << wallData[i][j] << " ";
							}
							std::cout << std::endl;
						}
						for (int i = 0; i < enemiesData.size(); ++i)
						{
							for (int j = 0; j < enemiesData[i].size(); ++j)
							{
								std::cout << enemiesData[i][j] << " ";
							}
							std::cout << std::endl;
						}
					}
					this->init(wallData, enemiesData, levelWidth, levelHeight);
				}
			}
			else
			{
				std::cout << "WARNING! Enemy data not read correctly. Level cannot be loaded correctly." << std::endl;
				throw;
			}
		}
		else
		{
			std::cerr << "ERROR! Wall data not read correctly. Level cannot be loaded." << std::endl;
			throw;
		}
	}
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
	for (GameObject& wall : this->Walls)
		wall.Draw(renderer);

	for (GameObject& enemy : this->Enemies)
		if (!enemy.Destroyed)
			enemy.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
	for (GameObject& enemy : this->Enemies)
		if (!enemy.Destroyed)
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> wallsData, std::vector<std::vector<unsigned int>> enemiesData, unsigned int levelWidth, unsigned int levelHeight)
{
	// calculate dimensions
	unsigned int height = static_cast<unsigned int> (wallsData.size());
	unsigned int width = static_cast<unsigned int> (wallsData[0].size());
	float unit_width = levelWidth / static_cast<float>(width);
	float unit_height = levelHeight / static_cast<float>(height);
	// initialize level based on wallsData
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			// check wall material type from level data (2D level array)
			// wallsData[y][x] == 0 // air
			if (wallsData[y][x] == 1) // solid
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("solid_wall"), glm::vec3(0.8f, 0.8f, 0.0f));
				obj.isSolid = true;
				this->Walls.push_back(obj);
			}
			else if (wallsData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (wallsData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (wallsData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (wallsData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (wallsData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				this->Walls.push_back(GameObject(pos, size, ResourceManager::GetTexture("breakable"), color));
			}
		}
	}
	height = static_cast<unsigned int> (enemiesData.size());
	width = static_cast<unsigned int> (enemiesData[0].size());
	for (unsigned int y = 0; y < height; ++y)
	{
		for (unsigned int x = 0; x < width; ++x)
		{
			if (enemiesData[y][x] == 1) // enemy
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size, ResourceManager::GetTexture("Enemy"), glm::vec3(1.0f, 1.0f, 1.0f));
				this->Enemies.push_back(obj);
			}
		}
	}
}

