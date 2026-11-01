//
// Created by Вадим Патрушев on 01.11.2026.
//

#ifndef OOD_EDITOR_COLORPICKER_H
#define OOD_EDITOR_COLORPICKER_H

#include "Model/Types/Color.h"
#include "Types/Button.h"
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

class ColorPicker
{
public:
	ColorPicker(const sf::Font& font, std::function<void()> onApply, std::function<void()> onCancel);
	void Open(const std::string& title, sf::Vector2f position, Color color, std::function<void(Color)> onSelect);
	void Cancel();
	bool IsOpen() const;
	bool IsContains(sf::Vector2f point) const;
	bool HandleClick(sf::Vector2f point);
	void Draw(sf::RenderTarget& target) const;

private:
	const sf::Font& m_font;
	std::function<void()> m_onApply;
	std::function<void()> m_onCancel;
	std::function<void(Color)> m_onSelect;
	bool m_isOpen = false;
	sf::String m_title;
	sf::FloatRect m_rect;
	Color m_color = 0;
	std::vector<Button> m_buttons;

	void Close();
	sf::FloatRect GetSwatchRect(size_t index) const;
	void DrawSwatches(sf::RenderTarget& target) const;
	void DrawButtons(sf::RenderTarget& target) const;
};

#endif // OOD_EDITOR_COLORPICKER_H
