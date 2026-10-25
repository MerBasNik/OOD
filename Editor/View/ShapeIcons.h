//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPEICONS_H
#define OOD_EDITOR_SHAPEICONS_H

#include "Model/Shapes/ShapeType.h"
#include <SFML/Graphics/Texture.hpp>

class ShapeIcons
{
public:
	static sf::Texture Load(ShapeType type);
};

#endif // OOD_EDITOR_SHAPEICONS_H
