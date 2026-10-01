//
// Created by Вадим Патрушев on 02.10.2026.
//

#include "Color.h"

Color::Color(const uint8_t red, const uint8_t green, const uint8_t blue)
	: m_red(red), m_green(green), m_blue(blue)
{
}

Color::Color()
	: m_red(0), m_green(0), m_blue(0)
{
}

uint8_t Color::GetRed() const
{
	return m_red;
}

uint8_t Color::GetGreen() const
{
	return m_green;
}

uint8_t Color::GetBlue() const
{
	return m_blue;
}