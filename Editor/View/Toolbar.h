//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_TOOLBAR_H
#define OOD_EDITOR_TOOLBAR_H

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
	void AddSeparator();
	void Draw(sf::RenderTarget& target) const;
	bool HandleClick(sf::Vector2f point) const;
	bool IsContains(sf::Vector2f point) const;

private:
	struct Button
	{
		sf::String m_label;
		sf::FloatRect m_rect;
		std::function<void()> m_onClick;
	};

	sf::Font m_font;
	bool m_hasFont = false;
	std::vector<Button> m_buttons;
	float m_nextLeft;
};

#endif // OOD_EDITOR_TOOLBAR_H
