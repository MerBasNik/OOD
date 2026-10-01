//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "RectangleBehavior.h"
#include <iostream>

RectangleBehavior::~RectangleBehavior()
{
	std::cout << "delete rectangle behavior" << std::endl;
}

void RectangleBehavior::Draw(ICanvas& canvas, Color color)
{
	std::cout << "draw rectangle behavior" << std::endl;
}

std::string RectangleBehavior::GetName()
{
	return "rectangle";
}

std::string RectangleBehavior::GetInfo()
{
	return "rectangle behavior";
}

void RectangleBehavior::Move(Point position)
{
	m_position = position;
}