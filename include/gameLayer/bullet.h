#pragma once
#include <gl2d/gl2d.h>


class Bullet
{
	bool isEnemy = 0;
	float bulletSpeed = 2800;
	glm::vec2 bulletPosition = {};
	glm::vec2 fireDirection = {};

public:
	bool getIsEnemy() const;
	void setIsEnemy(bool newIsEnemy);

	float getBSpeed() const;
	void setBSpeed(float newSpeed);

	glm::vec2 getBPosition() const;
	glm::vec2 getFireDirection() const;

	void setBPosition(const glm::vec2& newPosition);
	void setFireDirection(const glm::vec2& newFireDirection);

	void render(gl2d::Renderer2D &renderer,
		gl2d::Texture bulletsTexture, gl2d::TextureAtlasPadding bulletsAtlas
		);

	void update(float deltaTime);

};

