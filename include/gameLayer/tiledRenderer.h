#pragma once
#include <gl2d/gl2d.h>

class TiledRenderer
{
	float backgroundSize = 10000;
	gl2d::Texture texture;
	float paralaxStrength = 1;

	public:
	void setTexture(gl2d::Texture &bg);
	void setParalaxStrength(float p);
	void render(gl2d::Renderer2D& renderer, int w, int h) const;
};

void renderSpaceShip(
	gl2d::Renderer2D &renderer,
	glm::vec2 position, float size,
	gl2d::Texture texture,
	glm::vec4 uvs, glm::vec2 viewDirection);