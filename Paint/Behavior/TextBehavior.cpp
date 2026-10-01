//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "TextBehavior.h"
#include <sstream>
#include <iomanip>
#include <iostream>

TextBehavior::TextBehavior(const Point position, const double fontSize, const std::string& text)
	: m_position(position), m_fontSize(fontSize), m_text(text)
{
}

void TextBehavior::Draw(ICanvas& canvas, const Color color) const
{
	canvas.SetColor(color);
	std::cout << "draw text behavior" << std::endl;
}

std::string TextBehavior::GetName() const
{
	return "text";
}

std::string TextBehavior::GetInfo() const
{
	std::ostringstream output;
	output << std::fixed << std::setprecision(2)
		<< m_position.m_x << " " << m_position.m_y << m_fontSize << " " << m_text;
	return output.str();
}

void TextBehavior::Move(const Point position)
{
	m_position.m_x += position.m_x;
	m_position.m_y += position.m_y;
}

std::unique_ptr<IShapeBehavior> TextBehavior::Clone() const
{
	return std::make_unique<TextBehavior>(*this);
}