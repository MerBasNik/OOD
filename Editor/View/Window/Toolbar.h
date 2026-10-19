//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_TOOLBAR_H
#define OOD_EDITOR_TOOLBAR_H

#include "Types/Button.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

class Toolbar
{
public:
	static constexpr float HEIGHT = 44.f;

	Toolbar();
	void AddButton(const std::string& label, std::function<void()> onClick);
	void Draw(sf::RenderTarget& target) const;
	bool HandleClick(sf::Vector2f point) const;
	bool IsContains(sf::Vector2f point) const;

private:
	sf::Font m_font;
	std::vector<Button> m_buttons;
};

#endif // OOD_EDITOR_TOOLBAR_H
