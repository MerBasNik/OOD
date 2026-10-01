//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "TextBehavior.h"
#include <iostream>

TextBehavior::~TextBehavior()
{
	std::cout << "delete text behavior" << std::endl;
}

void TextBehavior::Draw(ICanvas& canvas, Color color)
{
	std::cout << "draw text behavior" << std::endl;
}

std::string TextBehavior::GetName()
{
	return "text";
}

std::string TextBehavior::GetInfo()
{
	return "text behavior";
}

void TextBehavior::Move(Point position)
{
	m_position = position;
}