//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeIcons.h"
#include <filesystem>
#include <iostream>

const std::filesystem::path ICONS_DIR = EDITOR_ICONS_DIR;

sf::Texture ShapeIcons::Load(const ShapeType type)
{
	std::filesystem::path path;
	switch (type)
	{
	case ShapeType::Ellipse:
		path = ICONS_DIR / "ellipse.png";
		break;
	case ShapeType::Rectangle:
		path = ICONS_DIR / "rectangle.png";
		break;
	case ShapeType::Triangle:
		path = ICONS_DIR / "triangle.png";
		break;
	}

	sf::Texture icon;
	if (!icon.loadFromFile(path))
	{
		std::cerr << "Ошибка при загрузке иконки " << path << std::endl;
	}
	return icon;
}
