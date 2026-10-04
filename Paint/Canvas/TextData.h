//
// Created by Вадим Патрушев on 05.10.2026.
//

#ifndef OOD_TEXTDATA_H
#define OOD_TEXTDATA_H

#include <string>
#include <SFML/Graphics/Color.hpp>

struct TextData
{
	float m_x;
	float m_y;
	unsigned m_fontSize;
	std::string m_text;
	sf::Color m_color;
};

#endif //OOD_TEXTDATA_H
