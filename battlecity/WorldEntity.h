#pragma once
#include <SFML\Graphics.hpp>
#include "Macros.h"

enum entityType {
	Brick,
	Steel,
	Bush,
	Water,
	Ice,
	Eagle,
	WorldBound
};

class WorldEntity
{
public:
	WorldEntity(entityType type, float posX, float posY);
	entityType GetType() const;
	sf::Sprite GetSprite() const;

private:
	sf::Texture m_worldEntityTexture;
	sf::Sprite m_worldEntitySprite;
	entityType m_entityType;
	Position m_entityPosition;
};

