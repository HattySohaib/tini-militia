#pragma once
#include <gl2d/gl2d.h>
#include "gunman.h"
#include "bullet.h"

constexpr float enemyShipSize = 250.f;

class Enemy: public Gunman, public Bullet
{	protected:
	float speed=0.f;
	float turnSpeed=0.f;
	float firedTime = 0.f;
	float fireTimeReset = 0.f;
	float fireRange = 0.f;
	float life = 0.f;
	
public:
	float getSpeed() const;
	float getTurnSpeed() const;
	float getFiredTime() const;
	float getFireTimeReset() const;
	float getFireRange() const;
	float getLife() const;

	void setSpeed(float newSpeed);
	void setTurnSpeed(float newTurnSpeed);
	void setFiredTime(float newFiredTime);
	void setFireTimeReset(float newFireTimeReset);
	void setFireRange(float newFireRange);
	void setLife(float newLife);

	void setGunman(glm::vec2 _position);

	virtual void setAll(glm::uvec2 _type, float _speed, float _turnSpeed, float _firedTime, float _fireTimeReset,
		float _fireRange, float _life);

	void render(gl2d::Renderer2D &renderer, gl2d::Texture &sprites,
		gl2d::TextureAtlasPadding &atlas) ;

	bool update(float deltaTime, glm::vec2 playerPos);

};

class Loki : public Enemy {
public:
	Loki() {
		Enemy::setAll({ 0,0 },900.f, 2.5f, 1.f, 0.2f, 1.5f, 1.f);
		Bullet::setBSpeed(2600.f);
	}
};

class Hela : public Enemy {
public:
	Hela() {
		Enemy::setAll({ 2,0 },1200.f, 2.8f, 1.f, 0.3f, 1.2f, 1.5);
		Bullet::setBSpeed(2900.f);
	}
};

class Thanos : public Enemy {
public:
	Thanos() {
		Enemy::setAll({ 1,0 },1500.f, 3.1f, 1.f, 0.4f, 1.0f, 2.f);
		Bullet::setBSpeed(3200.f);
	}
};