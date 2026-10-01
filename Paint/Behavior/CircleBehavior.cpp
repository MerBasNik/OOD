//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "CircleBehavior.h"

#include <iostream>
#include <ostream>

CircleBehavior::~CircleBehavior()
{
	std::cout << "delete circle behavior" << std::endl;
}

void CircleBehavior::Draw(ICanvas& canvas, Color color)
{
	std::cout << "draw circle behavior" << std::endl;
}

std::string CircleBehavior::GetInfo()
{
	return "circle behavior";
}

std::string CircleBehavior::GetName()
{
	return "circle";
}

void CircleBehavior::Move(const Point position)
{
	m_position = position;
}
