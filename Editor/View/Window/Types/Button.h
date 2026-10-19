//
// Created by Вадим Патрушев on 19.10.2026.
//

#ifndef OOD_EDITOR_BUTTON_H
#define OOD_EDITOR_BUTTON_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/String.hpp>
#include <functional>

struct Button
{
	sf::String m_label;
	sf::FloatRect m_rect;
	std::function<void()> m_onClick;
};

#endif // OOD_EDITOR_BUTTON_H
