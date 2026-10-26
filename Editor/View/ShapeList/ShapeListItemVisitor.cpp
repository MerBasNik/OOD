//
// Created by Вадим Патрушев on 26.10.2026.
//

#include "ShapeListItemVisitor.h"

ShapeListItemVisitor::ShapeListItemVisitor(const std::map<ShapeType, sf::Texture>& icons)
	: m_icons(icons)
{
}

void ShapeListItemVisitor::Visit(const Rectangle&)
{
	SetItem("Прямоугольник", ShapeType::Rectangle);
}

void ShapeListItemVisitor::Visit(const Ellipse&)
{
	SetItem("Эллипс", ShapeType::Ellipse);
}

void ShapeListItemVisitor::Visit(const Triangle&)
{
	SetItem("Треугольник", ShapeType::Triangle);
}

const std::string& ShapeListItemVisitor::GetName() const
{
	return m_name;
}

const sf::Texture& ShapeListItemVisitor::GetIcon() const
{
	return *m_icon;
}

void ShapeListItemVisitor::SetItem(const std::string& name, const ShapeType type)
{
	m_name = name;
	m_icon = &m_icons.at(type);
}
