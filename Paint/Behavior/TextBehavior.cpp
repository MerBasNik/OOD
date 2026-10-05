//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "TextBehavior.h"
#include <iomanip>
#include <iostream>
#include <sstream>

TextBehavior::TextBehavior(const Point position, const double fontSize, const std::string& text)
	: m_position(position)
	, m_fontSize(fontSize)
	, m_text(text)
{
}

void TextBehavior::Draw(ICanvas& canvas, const Color color) const
{
	canvas.SetColor(color);
	canvas.DrawText(m_position, m_fontSize, m_text);
}

std::string TextBehavior::GetName() const
{
	return "text";
}

std::string TextBehavior::GetInfo() const
{
	std::ostringstream output;
	output << m_position.m_x << " " << m_position.m_y << " " << m_fontSize << " " << m_text;
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