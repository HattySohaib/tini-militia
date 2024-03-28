#define GLM_ENABLE_EXPERIMENTAL
#include "gameLayer.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "platformInput.h"
#include <iostream>
#include <sstream>
#include <gl2d/gl2d.h>
#include <platformTools.h>
#include <tiledRenderer.h>
#include <bullet.h>
#include <vector>
#include <enemy.h>
#include <cstdio>
#include <glui/glui.h>
#include <raudio.h>

class GameplayData
{	
	public:
	glm::vec2 playerPos = {100,100};
	std::vector<Bullet> bullets;
	std::vector<Enemy> enemies;
	float health = 1.f;
	float spawnEnemyTimerSecconds = 6;
	int score=0;
};


GameplayData data;

gl2d::Renderer2D renderer;

constexpr int BACKGROUNDS = 3;

gl2d::Texture spaceShipsTexture;
gl2d::TextureAtlasPadding spaceShipsAtlas;

gl2d::Texture bulletsTexture;
gl2d::TextureAtlasPadding bulletsAtlas;

gl2d::Texture backgroundTexture[BACKGROUNDS];
TiledRenderer tiledRenderer[BACKGROUNDS];

gl2d::Texture healthBar;
gl2d::Texture health;

Sound shootSound;

static bool intersectBullet(glm::vec2 bulletPos, glm::vec2 shipPos, float shipSize)
{
	return glm::distance(bulletPos, shipPos) <= shipSize;
}

static void restartGame()
{
	data = {};
	renderer.currentCamera.follow(data.playerPos
		, 550, 0, 0, renderer.windowW, renderer.windowH);
}

bool initGame()
{
	std::srand(std::time(0));

	//initializing stuff for the renderer
	gl2d::init();
	renderer.create();

	spaceShipsTexture.loadFromFileWithPixelPadding
	(RESOURCES_PATH "spaceShip/stitchedFiles/new.png", 128, true);
	spaceShipsAtlas = gl2d::TextureAtlasPadding(5, 2, spaceShipsTexture.GetSize().x, spaceShipsTexture.GetSize().y);

	bulletsTexture.loadFromFileWithPixelPadding
	(RESOURCES_PATH "spaceShip/stitchedFiles/bullets.png", 500, true);
	bulletsAtlas = gl2d::TextureAtlasPadding(3, 2, bulletsTexture.GetSize().x, bulletsTexture.GetSize().y);

	healthBar.loadFromFile(RESOURCES_PATH "healthBar.png", true);
	health.loadFromFile(RESOURCES_PATH "health.png", true);

	shootSound = LoadSound(RESOURCES_PATH "gunshot.mp3");
	SetSoundVolume(shootSound, 0.1);

	backgroundTexture[0].loadFromFile(RESOURCES_PATH "background1.jpg", true);
	backgroundTexture[1].loadFromFile(RESOURCES_PATH "background8.png", true);
	backgroundTexture[2].loadFromFile(RESOURCES_PATH "background3.png", true);
	/*backgroundTexture[3].loadFromFile(RESOURCES_PATH "background4.png", true);*/

	tiledRenderer[0].setTexture(backgroundTexture[0]);
	tiledRenderer[1].setTexture(backgroundTexture[1]);
	tiledRenderer[2].setTexture(backgroundTexture[2]);
	//tiledRenderer[3].setTexture(backgroundTexture[3]);

	tiledRenderer[0].setParalaxStrength(0);
	tiledRenderer[1].setParalaxStrength(0.2);
	tiledRenderer[2].setParalaxStrength(0.4);
	//tiledRenderer[3].setParalaxStrength(0.7);

	restartGame();

	return true;
}


constexpr float shipSize = 250.f;

static void spawnEnemy()
{
	Loki l;
	Hela h;
	Thanos t;
	Enemy eTypes[] = {l,h,t};
	
	int e = rand() % 3;

	glm::vec2 offset(2000, 0);
	offset = glm::vec2(  glm::vec4(offset,0,1) * glm::rotate(glm::mat4(1.f), glm::radians((float)(rand()%360)), glm::vec3(0,0, 1))  );

	eTypes[e].setGunman(data.playerPos + offset);

	data.enemies.push_back(eTypes[e]);
}

bool gameLogic(float deltaTime)
{

#pragma region init stuff
	int w = 0; int h = 0;
	w = platform::getFrameBufferSizeX(); //window w
	h = platform::getFrameBufferSizeY(); //window h
	
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT); //clear screen

	renderer.updateWindowMetrics(w, h);
#pragma endregion



#pragma region movement

	glm::vec2 move = {0,0.05};
	glm::vec2 setSkin = { 3,0 };

	if (
		platform::isButtonHeld(platform::Button::W) ||
		platform::isButtonHeld(platform::Button::Up)
		)
	{
		move.y = -1;
		setSkin = { 0,1 };
	}
	if (
		platform::isButtonHeld(platform::Button::S) ||
		platform::isButtonHeld(platform::Button::Down)
		)
	{
		move.y = 1;
	}
	if (
		platform::isButtonHeld(platform::Button::A) ||
		platform::isButtonHeld(platform::Button::Left)
		)
	{
		move.x = -1;
	}
	if (
		platform::isButtonHeld(platform::Button::D) ||
		platform::isButtonHeld(platform::Button::Right)
		)
	{
		move.x = 1;
	}

	if (move.x != 0 || move.y != 0)
	{
		move = glm::normalize(move);
		move *= deltaTime * 2000; //500 pixels per seccond
		if (data.playerPos.x+move.x < 1.5*w-200 && data.playerPos.x + move.x > -1.5*w+200) {
			data.playerPos.x += move.x;
		}
		if (data.playerPos.y + move.y < 1.5 * h-200 && data.playerPos.y +move.y > -1.5 * h+200) {
			data.playerPos.y += move.y;
		}

	}

#pragma endregion

#pragma region follow

	renderer.currentCamera.follow(data.playerPos
		, deltaTime * 550, 1, 150, w, h);

#pragma endregion

#pragma region render background

	renderer.currentCamera.zoom = 0.75;

	for (int i = 0; i < BACKGROUNDS; i++)
	{
		tiledRenderer[i].render(renderer, w, h);
	}
	//tiledRenderer[0].render(renderer);
#pragma endregion


#pragma region mouse pos

	glm::vec2 mousePos = platform::getRelMousePosition();
	glm::vec2 screenCenter(w / 2.f, h / 2.f);

	glm::vec2 mouseDirection = mousePos - screenCenter;

	if (glm::length(mouseDirection) == 0.f)
	{
		mouseDirection = {1,0};
	}
	else
	{
		mouseDirection = normalize(mouseDirection);
	}

	float spaceShipAngle = atan2(mouseDirection.y, -mouseDirection.x);

#pragma endregion

#pragma region handle bulets


	if (platform::isLMousePressed())
	{
		Bullet b;

		b.setBPosition(data.playerPos);
		b.setFireDirection(mouseDirection);

		data.bullets.push_back(b);

		PlaySound(shootSound);

	}


	for (int i = 0; i < data.bullets.size(); i++)
	{
		
		if (glm::distance(data.bullets[i].getBPosition(), data.playerPos) > 5'000)
		{
			data.bullets.erase(data.bullets.begin() + i);
			i--;
			continue;
		}

		if (!data.bullets[i].getIsEnemy())
		{
			bool breakBothLoops = false;
			for (int e = 0; e < data.enemies.size(); e++)
			{

				if (intersectBullet(data.bullets[i].getBPosition(), data.enemies[e].getPosition(),
					enemyShipSize))
				{
					data.enemies[e].setLife(data.enemies[e].getLife()-0.5);

					if (data.enemies[e].getLife() <= 0)
					{
						//kill enemy
						data.enemies.erase(data.enemies.begin() + e);
					}

					data.bullets.erase(data.bullets.begin() + i);
					i--;
					breakBothLoops = true;
					continue;
				}

			}

			if (breakBothLoops)
			{
				continue;
			}
		}
		else
		{
			if (intersectBullet(data.bullets[i].getBPosition(), data.playerPos,
				shipSize))
			{
				data.health -= 0.1;

				data.bullets.erase(data.bullets.begin() + i);
				i--;
				continue;
			}

		}

		data.bullets[i].update(deltaTime);

	}

	if (data.health <= 0)
	{
		//kill player
		restartGame();
	}
	else
	{
		data.health += deltaTime * 0.05;
		data.health = glm::clamp(data.health, 0.f, 1.f);
	}

#pragma endregion

#pragma region handle enemies

	if (data.enemies.size() < 15) 
	{
		data.spawnEnemyTimerSecconds -= deltaTime;

		if (data.spawnEnemyTimerSecconds < 0)
		{
			data.spawnEnemyTimerSecconds = rand() % 6 + 1;

			spawnEnemy();
			if (rand() % 3 == 0)
			{
				spawnEnemy();
				spawnEnemy();
			}

		}
	
	}


	for (int i = 0; i < data.enemies.size(); i++)
	{

		if (glm::distance(data.playerPos, data.enemies[i].getPosition()) > 4000.f)
		{
			//dispawn enemy
			data.enemies.erase(data.enemies.begin() + i);
			i--;
			continue;
		}

		if (data.enemies[i].update(deltaTime, data.playerPos))
		{
			Bullet b;
			b.setBPosition(data.enemies[i].getPosition());
			b.setFireDirection (data.enemies[i].getViewDirection());
			b.setBSpeed(data.enemies[i].getBSpeed());
			b.setIsEnemy(true);
			data.bullets.push_back(b);

			if (!IsSoundPlaying(shootSound)) PlaySound(shootSound);

		}
	}

#pragma endregion

#pragma region render enemies

	for (auto &e : data.enemies)
	{
		e.render(renderer, spaceShipsTexture, spaceShipsAtlas);
	}

#pragma endregion

#pragma region render ship

	renderSpaceShip(renderer, data.playerPos, shipSize,
		spaceShipsTexture, spaceShipsAtlas.get(setSkin.x, setSkin.y), mouseDirection);
	

#pragma endregion

#pragma region render bullets

	for (auto &b : data.bullets)
	{
		b.render(renderer, bulletsTexture, bulletsAtlas);
	}

#pragma endregion


#pragma region ui

	renderer.pushCamera();
	{

		glui::Frame f({0,0, w, h});

		glui::Box healthBox = glui::Box().xLeftPerc(0.65).yTopPerc(0.1).
			xDimensionPercentage(0.3).yAspectRatio(1.f/8.f);

		renderer.renderRectangle(healthBox, healthBar);

		glm::vec4 newRect = healthBox();
		newRect.z *= data.health;

		glm::vec4 textCoords = {0,1,1,0};
		textCoords.z *= data.health;

		renderer.renderRectangle(newRect, health, Colors_White, {}, {}, 
			textCoords);

	}
	renderer.popCamera();

#pragma endregion



	renderer.flush();
	

	//ImGui::ShowDemoWindow();

	//ImGui::Begin("debug");
	//
	//ImGui::Text("Bullets count: %d", (int)data.bullets.size());
	//ImGui::Text("Enemies count: %d", (int)data.enemies.size());
	//
	//if (ImGui::Button("Spawn enemy"))
	//{
	//	spawnEnemy();
	//}
	//
	//if (ImGui::Button("Reset game"))
	//{
	//	restartGame();
	//}
	//
	//ImGui::SliderFloat("Player Health", &data.health, 0, 1);
	//
	//ImGui::End();


	return true;
#pragma endregion

}

//This function might not be be called if the program is forced closed
void closeGame()
{


}
