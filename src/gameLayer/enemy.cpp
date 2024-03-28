#include <enemy.h>
#include <tiledRenderer.h>



void Enemy::render(gl2d::Renderer2D &renderer,
	gl2d::Texture &sprites, gl2d::TextureAtlasPadding &atlas)
{	
	renderSpaceShip(renderer, position, enemyShipSize,
		sprites, atlas.get(type.x, type.y), viewDirection);
}


bool Enemy::update(float deltaTime, glm::vec2 playerPos){
	glm::vec2 directionToPlayer = playerPos - position;
	if (glm::length(directionToPlayer) == 0) { directionToPlayer = {1,0}; }
	else { directionToPlayer = glm::normalize(directionToPlayer); }

	glm::vec2 newDirection = {};

	bool shoot = (glm::length(directionToPlayer + viewDirection) >= fireRange);

	if (shoot)
	{
		if (firedTime <= 0.f)
		{
			//we can shoot
			firedTime = fireTimeReset;
		}
		else
		{
			shoot = 0;
		}
	}

	firedTime -= deltaTime;
	if (firedTime < 0) { firedTime = 0.f; }


	if (glm::length(directionToPlayer + viewDirection) <= 0.2)
	{
		if (rand() % 2)
		{
			newDirection = glm::vec2(directionToPlayer.y, -directionToPlayer.x);
		}
		else
		{
			newDirection = glm::vec2(-directionToPlayer.y, directionToPlayer.x);
		}

	}
	else
	{
		newDirection =
			deltaTime * turnSpeed * directionToPlayer + viewDirection;
	}

	float length = glm::length(newDirection);
	viewDirection = glm::normalize(newDirection);
	
	length = glm::clamp(length, 0.1f, 3.f);

	position += viewDirection * deltaTime * speed * length;

	return shoot;
}


float Enemy::getSpeed() const { return speed; }

float Enemy::getTurnSpeed() const { return turnSpeed; }

float Enemy::getFiredTime() const { return firedTime; }

float Enemy::getFireTimeReset() const { return fireTimeReset; }

float Enemy::getFireRange() const { return fireRange; }


float Enemy::getLife() const { return life; }



void Enemy::setSpeed(float newSpeed) { speed = newSpeed; }

void Enemy::setTurnSpeed(float newTurnSpeed) { turnSpeed = newTurnSpeed; }

void Enemy::setFiredTime(float newFiredTime) { firedTime = newFiredTime; }

void Enemy::setFireTimeReset(float newFireTimeReset) { fireTimeReset = newFireTimeReset; }

void Enemy::setFireRange(float newFireRange) { fireRange = newFireRange; }

void Enemy::setLife(float newLife) { life = newLife; }

void Enemy::setAll(glm::uvec2 _type, float _speed, float _turnSpeed, float _firedTime, float _fireTimeReset,
	float _fireRange,float _life) {
	speed = _speed;
	turnSpeed = _turnSpeed;
	firedTime = _firedTime;
	fireTimeReset = _fireTimeReset;
	fireRange = _fireRange;
	life = _life;
	type = _type;
}

void Enemy::setGunman(glm::vec2 _position) {
	position = _position;
}