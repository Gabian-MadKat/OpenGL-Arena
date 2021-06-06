#pragma once
#include <glad/glad.h>
#include <GLFW//glfw3.h>

#include "GameLevel.h"
#include "Projectile.h"
#include <vector>

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	// game state
	GameState	State;
	bool		Keys[1024];
	bool		KeysProcessed[1024];
	unsigned int Width, Height;
	std::vector<GameLevel> Levels;
	std::vector<ProjectileObject> Projectiles;
	std::vector<ParticleGenerator> Particles;
	unsigned int Level;
	unsigned int Lives;
	unsigned int Score;

	// c-tor/d-tor
	Game(unsigned int width, unsigned int height);
	~Game();
	// initialize game state (load all shaders/textures/levels)
	void Init();
	// game loop
	void ProcessInput(float dt, double xpos, double ypos, bool lmbclck);
	void Update(float dt);
	void Render();
	void DoCollisions();
	void ResetPlayer();
	void ResetLevel();
};