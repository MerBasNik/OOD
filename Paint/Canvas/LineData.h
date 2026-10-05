//
// Created by Вадим Патрушев on 05.10.2026.
//

#ifndef OOD_LINESTRUCT_H
#define OOD_LINESTRUCT_H

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct LineData
{
	sf::Vector2f m_a;
	sf::Vector2f m_b;
	sf::Color m_color;
};

#endif // OOD_LINESTRUCT_H
