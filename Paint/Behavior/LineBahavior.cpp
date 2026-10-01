//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "LineBehavior.h"
#include <iostream>

LineBehavior::~LineBehavior()
{
	std::cout << "delete line behavior" << std::endl;
}

void LineBehavior::Draw(ICanvas& canvas, Color color)
{
	std::cout << "draw line behavior" << std::endl;
}

std::string LineBehavior::GetName()
{
	return "line";
}

std::string LineBehavior::GetInfo()
{
	return "line behavior";
}

void LineBehavior::Move(Point position)
{
	m_end = position;
	m_start = position;
}