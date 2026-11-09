//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_TOOLBAR_H
#define OOD_EDITOR_TOOLBAR_H

#include "Types/Button.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>
#include <string>
#include <vector>

class Toolbar
{
public:
	static constexpr float HEIGHT = 44.f;

	Toolbar();
	sf::FloatRect AddButton(const std::string& label, std::function<void()> onClick,
		std::function<bool()> isVisible = {});
	void AddIconButton(sf::Texture icon, std::function<void()> onClick, std::function<bool()> isActive,
		std::function<bool()> isVisible = {});
	void Draw(sf::RenderTarget& target) const;
	bool SetHoverPoint(std::optional<sf::Vector2f> point);
	bool HandleClick(sf::Vector2f point) const;
	bool IsContains(sf::Vector2f point) const;
	const sf::Font& GetFont() const;

private:
	sf::Font m_font;
	std::vector<Button> m_buttons;
	std::optional<sf::Vector2f> m_hoverPoint;

	void AddButton(Button button, float width);
	const Button* FindButton(std::optional<sf::Vector2f> point) const;
	static bool IsVisible(const Button& button);
};

#endif // OOD_EDITOR_TOOLBAR_H
