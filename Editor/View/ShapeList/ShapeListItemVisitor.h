//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPELISTITEMVISITOR_H
#define OOD_EDITOR_SHAPELISTITEMVISITOR_H

#include "Model/Shapes/IShapeVisitor.h"
#include "Model/Shapes/ShapeType.h"
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <string>

class ShapeListItemVisitor : public IShapeVisitor
{
public:
	explicit ShapeListItemVisitor(const std::map<ShapeType, sf::Texture>& icons);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;

	const std::string& GetName() const;
	const sf::Texture& GetIcon() const;

private:
	const std::map<ShapeType, sf::Texture>& m_icons;
	std::string m_name;
	const sf::Texture* m_icon = nullptr;

	void SetItem(const std::string& name, ShapeType type);
};

#endif // OOD_EDITOR_SHAPELISTITEMVISITOR_H
