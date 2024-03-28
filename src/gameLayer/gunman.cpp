#include <gunman.h>
#include "gunman.h"
#include <tiledRenderer.h>

	glm::uvec2 Gunman::getType() const { return type; }
	glm::vec2 Gunman::getPosition() const { return position; }
	glm::vec2 Gunman::getViewDirection() const { return viewDirection; }

	void Gunman::setViewDirection(glm::vec2 v) {
		viewDirection = v;
	}
	void Gunman::setPosition(glm::vec2 p) {
		position = p;
	}
	void Gunman::setType(glm::vec2 t) {
		type = t;
	}
