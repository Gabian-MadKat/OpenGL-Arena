#include "Game.h"
#include "Resource_Manager.h"
#include "SpriteRenderer.h"
#include "Particle_Generator.h"
#include "Post_Processor.h"
#include "TextRenderer.h"

#define _USE_MATH_DEFINES
#include <vector>
#include <math.h>
#include <irrklang/irrKlang.h>

// Initial size of the player
const glm::vec2 PLAYER_SIZE(34.0f, 48.0f);
// Initial velocity of the player
const float PLAYER_VELOCITY(100.0f);
const float PROJECTILE_RADIUS(10.0f);
// Pointers - Game-related State data
SpriteRenderer* Renderer;
GameObject* Player;
PostProcessor* Effects;
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();
TextRenderer* Text, * SmallText;

// Game variables
float ShakeTime = 0.0f;

// Functions
bool CheckPlayerCollision(GameObject& one, GameObject& two)
{
	// Collision x-axis
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x && two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y && two.Position.y + two.Size.y >= one.Position.y;
	// Collision detected only if both axes collide
	return collisionX && collisionY;
}

bool CheckCollision(ProjectileObject& one, GameObject& two)
{
	// Get center point circle first
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
		);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	return glm::length(difference) < one.Radius;
}

// Game Class
Game::Game(unsigned int width, unsigned int height)
	: State(GameState::GAME_MENU), Keys(), KeysProcessed(), Width(width), Height(height), Level(0), Lives(3), Score(0)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Effects;
	delete Text;
}

void Game::Init()
{
	// load shaders
	ResourceManager::LoadShader("resources/shaders/Used/Sprite.vs", "resources/shaders/Used/Sprite.fs", nullptr, "sprite");
	ResourceManager::LoadShader("resources/shaders/Used/Particle.vs", "resources/shaders/Used/Particle.fs", nullptr, "particle");
	ResourceManager::LoadShader("resources/shaders/Used/Post_Processing.vs", "resources/shaders/Used/Post_Processing.fs", nullptr, "postprocessing");
	// configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
	// load textures
	// ResourceManager::LoadTexture("resources/textures/Skyrim_tr.png", true, "Empire");
	ResourceManager::LoadTexture("resources/textures/wall.png", false, "solid_wall");
	ResourceManager::LoadTexture("resources/textures/crate.png", false, "breakable");
	ResourceManager::LoadTexture("resources/textures/thief_temp.png", true, "Enemy");
	ResourceManager::LoadTexture("resources/textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("resources/textures/Playerchar.png", true, "Darkelf");
	ResourceManager::LoadTexture("resources/textures/fireparticle.png", true, "Projectile");
	ResourceManager::LoadTexture("resources/textures/particle_orange.png", true, "particle");
	// Set render-specific controls
	Shader shader;
	shader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(shader);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	Text = new TextRenderer(this->Width, this->Height);
	Text->Load("resources/fonts/prstart.ttf", 24);
	SmallText = new TextRenderer(this->Width, this->Height);
	SmallText->Load("resources/fonts/prstart.ttf", 12);
	// load levels
	GameLevel one, two, three, four;
	one.Load("data/levels/1.lvl", this->Width, this->Height);
	two.Load("data/levels/2.lvl", this->Width, this->Height);
	three.Load("data/levels/3.lvl", this->Width, this->Height);
	four.Load("data/levels/4.lvl", this->Width, this->Height);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("Darkelf"));
	
}

void Game::Update(float dt)
{
	//int vec_pos = 0;
	// Update objects and particles
	if (this->Projectiles.size() != NULL && this->Particles.size() != NULL)
	{
		auto p = Projectiles.begin();
		auto part = Particles.begin();
		// for (auto p = this->Projectiles.begin(); p != this->Projectiles.end(); ++p)
		while (p != Projectiles.end() || part != Particles.end())
		{
			p->Move(dt, this->Width, this->Height);
			part->Update(dt, *p, 2, glm::vec2(p->Radius / 2));
			
			if (p->Position.x <= 0.0f)
			{
				this->Projectiles.erase(p);
				this->Particles.erase(part);
				break;
			}
			else if (p->Position.x + p->Size.x >= Width)
			{
				this->Projectiles.erase(p);
				this->Particles.erase(part);
				break;
			}
			if (p->Position.y <= 0.0f)
			{
				this->Projectiles.erase(p);
				this->Particles.erase(part);
				break;
			}
			else if (p->Position.y + p->Size.y >= Height)
			{
				this->Projectiles.erase(p);
				this->Particles.erase(part);
				break;
			}
			++p;
			++part;
		}
	}

	// Check for collisions
	this->DoCollisions();
	// Reduce shake time
	if (ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = false;
	}
	// Check loss condition
	if (this->Lives == 0)
	{
		this->ResetLevel();
		this->State = GameState::GAME_MENU;
	}
	// Check win condition
	if (this->State == GameState::GAME_ACTIVE && this->Levels[this->Level].IsCompleted())
	{
		Effects->Chaos = true;
		SoundEngine->play2D("data/sound/270330__littlerobotsoundfactory__jingle-achievement-01.wav", false);
		this->State = GameState::GAME_WIN;
	}
}

void Game::ProcessInput(float dt, double xpos, double ypos, bool lmbclck)
{
	if (this->State == GameState::GAME_MENU)
	{
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
		{
			this->State = GameState::GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
		}
		if (this->Keys[GLFW_KEY_W] && !this->KeysProcessed[GLFW_KEY_W])
		{
			this->Level = (this->Level + 1) % 4;
			this->KeysProcessed[GLFW_KEY_W] = true;
		}
		if (this->Keys[GLFW_KEY_S] && !this->KeysProcessed[GLFW_KEY_S])
		{
			if (this->Level > 0)
				--this->Level;
			else
				this->Level = 3;
			//this->Level = (this->Level - 1) % 4;
			this->KeysProcessed[GLFW_KEY_S] = true;
		}
	}

	if (this->State == GameState::GAME_WIN)
	{
		if (this->Keys[GLFW_KEY_ENTER])
		{
			this->KeysProcessed[GLFW_KEY_ENTER] = true;
			Effects->Chaos = false;
			this->ResetLevel();
			this->ResetPlayer();
			this->State = GameState::GAME_MENU;
		}
	}

	if (this->State == GameState::GAME_ACTIVE)
	{
		if (this->Keys[GLFW_KEY_SPACE])
		{
			std::cout << "Positions: (" << xpos << " : " << ypos << ")" << std::endl;
  		}
		float velocity = PLAYER_VELOCITY * dt;
		if (this->Keys[GLFW_KEY_LEFT_SHIFT])
		{
			velocity = 1.85f * PLAYER_VELOCITY * dt;
		}
		// Move Player
		GameObject nextPlayer = *Player;
		
		if (this->Keys[GLFW_KEY_W])
		{
			if (nextPlayer.Position.y >= 0.0f)
				nextPlayer.Position.y -= velocity;
		}
		if (this->Keys[GLFW_KEY_S])
		{
			if (nextPlayer.Position.y <= this->Height - Player->Size.y)
			{
				nextPlayer.Position.y += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_A])
		{
			if (nextPlayer.Position.x >= 0.0f)
				nextPlayer.Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (nextPlayer.Position.x <= this->Width - Player->Size.x)
				nextPlayer.Position.x += velocity;
		}
		bool wallCollision = false;
		for (auto& wall : this->Levels[this->Level].Walls)
		{
			if (CheckPlayerCollision(nextPlayer, wall))
			{
				wallCollision = true;
				break;
			}
		}
		if (!wallCollision)
		{
			Player->Position.x = nextPlayer.Position.x;
			Player->Position.y = nextPlayer.Position.y;
		}
		if (lmbclck)	// NEW PROJECTILE
		{
			SoundEngine->play2D("data/sound/270332__littlerobotsoundfactory__hit-03.wav", false);
			double angle;
			if (xpos - (Player->Position.x + (double)PLAYER_SIZE.x / 2.0f) != 0)
			{
				angle = atan((ypos - (Player->Position.y + (double)PLAYER_SIZE.y / 2.0f)) / (xpos - (Player->Position.x + (double)PLAYER_SIZE.x / 2.0f)));
			}
			else
			{
				angle = M_PI / 2;
			}
			
			glm::vec2 projectilePos = Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - PROJECTILE_RADIUS, PLAYER_SIZE.y / 2.0f - PROJECTILE_RADIUS);
			glm::vec2 projectileVel = glm::vec2(std::copysign(250.0f * cos(angle), xpos - (Player->Position.x + (double)PLAYER_SIZE.x / 2.0f)), std::copysign(250.0f * sin(angle), ypos - (Player->Position.y + (double)PLAYER_SIZE.y / 2.0f)));
			ProjectileObject Projectile = ProjectileObject(projectilePos, PROJECTILE_RADIUS, 
				projectileVel,			// glm::vec2(std::copysign(250, xpos - Player->Position.x), std::copysign(250, ypos - Player->Position.y)), 
				ResourceManager::GetTexture("Projectile"));
			ParticleGenerator Particle = ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
			Projectiles.push_back(Projectile);
			Particles.push_back(Particle);
			//std::cout << "pancakes" << std::endl;
			std::cout << "Size " << Projectiles.size() << std::endl;
		}
		else
		{
			for (ProjectileObject& p : Projectiles)
			{
				p.Stuck = false;
			}
		}
	}
}

void Game::Render()
{
	/*
	Texture2D texture;
	texture = ResourceManager::GetTexture("Empire");
	Renderer->DrawSprite(texture, glm::vec2(300.0f, 0.0f), glm::vec2(300.0f, 568.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	*/
	Texture2D texture;
	texture = ResourceManager::GetTexture("background");
	if (this->State == GameState::GAME_ACTIVE || this->State == GameState::GAME_MENU || this->State == GameState::GAME_WIN)
	{
		// Begin rendering to PPC framebuffer
		Effects->BeginRender();
		// draw packground
		Renderer->DrawSprite(texture, glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		// draw level
		this->Levels[this->Level].Draw(*Renderer);
		// draw player
		Player->Draw(*Renderer);
		// draw particles
		for (ParticleGenerator p : Particles)
			p.Draw();
		// draw projectiles
		for (ProjectileObject& p : Projectiles)
			p.Draw(*Renderer);
		// End rendering to PPC framebuffer
		Effects->EndRender();
		// Render PPC quad
		Effects->Render((float)glfwGetTime());

		SmallText->RenderText("Version: 0.0.1a", 5.0f, this->Height - 14.0f, 1.0f);
	}

	if (this->State == GameState::GAME_ACTIVE)
	{
		std::stringstream ss;
		ss << this->Lives;
		Text->RenderText("Lives: " + ss.str(), 5.0f, 5.0f, 1.0f);
		ss.str(std::string()); // empty string
		ss << this->Score;
		Text->RenderText("Score: " + ss.str(), 550.0f, 5.0f, 1.0f);
	}
	if (this->State == GameState::GAME_MENU)
	{
		std::stringstream ss; ss << this->Level + 1;
		Text->RenderText("Press ENTER to start", this->Width / 2.0f - 225.0f, this->Height / 2.0f - 20.0f, 1.0f);
		Text->RenderText("Press W or S to select level: " + ss.str(), this->Width / 2.0f - 275.0f, this->Height / 2.0f + 20.0f, 0.75f);
		ss.str(std::string()); // empty string
		ss << this->Score;
		Text->RenderText("Score: " + ss.str(), 550.0f, 5.0f, 1.0f);
	}
	if (this->State == GameState::GAME_WIN)
	{
		Text->RenderText("Level clear!", this->Width / 2.0f - 144.0f, this->Height / 2.0f - 40.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		Text->RenderText("Press ENTER to retry", this->Width / 2.0f - 240.0f, this->Height / 2.0f + 20.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
		Text->RenderText("or ESC to quit", this->Width / 2.0f - 168.0f, this->Height / 2.0f + 50.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
		std::stringstream ss;
		ss << this->Score;
		Text->RenderText("Score: " + ss.str(), 550.0f, 5.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void Game::DoCollisions()
{
	for (GameObject& enemy : this->Levels[this->Level].Enemies)
	{
		if (!enemy.Destroyed)
		{
			auto part = Particles.begin();
			for (auto p = Projectiles.begin(); p != Projectiles.end(); ++p, ++part)
			{
				if (CheckCollision(*p, enemy))
				{
					if (!enemy.isSolid)
					{
						SoundEngine->play2D("data/sound/270306__littlerobotsoundfactory__explosion-02.wav", false);
						enemy.Destroyed = true;
						ShakeTime = 0.2f;
						Effects->Shake = true;
					}
					Projectiles.erase(p);
					Particles.erase(part);
					this->Score += 10;
					break;
				}
			}

			if (CheckPlayerCollision(*Player, enemy))
			{
				--this->Lives;
				SoundEngine->play2D("data/sound/270340__littlerobotsoundfactory__pickup-01.wav", false);
				this->ResetPlayer();
			}
		}
	}

	for (GameObject& wall : this->Levels[this->Level].Walls)
	{
		auto part = Particles.begin();
		for (auto p = Projectiles.begin(); p != Projectiles.end(); ++p, ++part)
		{
			if (CheckCollision(*p, wall))
			{
				if (!wall.isSolid)
				{
					SoundEngine->play2D("data/sound/270332__littlerobotsoundfactory__hit-03.wav", false);
					wall.Destroyed = true;
				}
				else
				{
					SoundEngine->play2D("data/sound/270325__littlerobotsoundfactory__hit-02.wav", false);
					if (this->Score >= 5)
					{
						this->Score -= 5;
					}
					else
					{
						this->Score = 0;
					}
				}
				Projectiles.erase(p);
				Particles.erase(part);
				break;
			}
		}
	}
}

void Game::ResetPlayer()
{
	// Reset player
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height / 2.0f - PLAYER_SIZE.y / 2.0f);
	Projectiles.clear();
	Particles.clear();
	// Disable all active efects
	Effects->Chaos = Effects->Confuse = false;
}

void Game::ResetLevel()
{
	switch (this->Level)
	{
	case 0:
		this->Levels[0].Load("data/levels/1.lvl", this->Width, this->Height);
		break;
	case 1:
		this->Levels[1].Load("data/levels/2.lvl", this->Width, this->Height);
		break;
	case 2:
		this->Levels[2].Load("data/levels/3.lvl", this->Width, this->Height);
		break;
	case 3:
		this->Levels[3].Load("data/levels/4.lvl", this->Width, this->Height);
		break;
	}

	this->Lives = 3;
}
