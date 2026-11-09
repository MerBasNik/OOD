//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeIcons.h"
#include <filesystem>
#include <iostream>

const std::filesystem::path ICONS_DIR = EDITOR_ICONS_DIR;

sf::Texture ShapeIcons::Load(const ShapeType type)
{
	switch (type)
	{
	case ShapeType::Ellipse:
		return LoadFile("ellipse.png");
	case ShapeType::Rectangle:
		return LoadFile("rectangle.png");
	case ShapeType::Triangle:
		return LoadFile("triangle.png");
	case ShapeType::Group:
		return LoadFile("group.png");
	}
	return {};
}

sf::Texture ShapeIcons::LoadUngroup()
{
	return LoadFile("ungroup.png");
}

sf::Texture ShapeIcons::LoadFile(const std::string& fileName)
{
	const std::filesystem::path path = ICONS_DIR / fileName;
	sf::Texture icon;
	if (!icon.loadFromFile(path))
	{
		std::cerr << "Ошибка при загрузке иконки " << path << std::endl;
	}
	return icon;
}
