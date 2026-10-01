//
// Created by Вадим Патрушев on 01.10.2026.
//

#include "TriangleBehavior.h"
#include <iostream>

TriangleBehavior::~TriangleBehavior()
{
	std::cout << "delete trangle behavior" << std::endl;
}

void TriangleBehavior::Draw(ICanvas& canvas, Color color)
{
	std::cout << "draw triangle behavior" << std::endl;
}

std::string TriangleBehavior::GetName()
{
	return "triangle";
}

std::string TriangleBehavior::GetInfo()
{
	return "triangle behavior";
}

void TriangleBehavior::Move(Point position)
{
	m_v1 = position;
	m_v2 = position;
	m_v3 = position;
}