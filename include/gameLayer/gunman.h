#pragma once
#include <gl2d/gl2d.h>

class Gunman{
	protected:
	glm::uvec2 type = {};
	glm::vec2 position = {};
	glm::vec2 viewDirection = { 0,0 };

	public:
		glm::uvec2 getType() const;
		glm::vec2 getPosition() const;
		glm::vec2 getViewDirection() const;

		void setViewDirection(glm::vec2 v);
		void setPosition(glm::vec2 p);
		void setType(glm::vec2 t);	
	
};
