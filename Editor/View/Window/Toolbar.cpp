//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Toolbar.h"
#include <iostream>

const float PADDING = 6.f;
const float BUTTON_WIDTH = 112.f;
const float SEPARATOR_WIDTH = 16.f;
const unsigned FONT_SIZE = 14;
const std::string FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
const sf::Color TOOLBAR_COLOR(236, 236, 236);
const sf::Color BUTTON_COLOR(250, 250, 250);
const sf::Color BUTTON_BORDER_COLOR(170, 170, 170);
const sf::Color TEXT_COLOR(30, 30, 30);

Toolbar::Toolbar()
	: m_nextLeft(PADDING)
{
	m_hasFont = m_font.openFromFile(FONT_PATH);
	if (!m_hasFont)
	{
		std::cerr << "Ошибка при загрузке шрифта" << std::endl;
	}
}

void Toolbar::AddButton(const std::string& label, std::function<void()> onClick)
{
	const sf::FloatRect rect({ m_nextLeft, PADDING }, { BUTTON_WIDTH, HEIGHT - 2 * PADDING });
	m_buttons.push_back({ sf::String::fromUtf8(label.begin(), label.end()), rect, std::move(onClick) });
	m_nextLeft += BUTTON_WIDTH + PADDING;
}

void Toolbar::AddSeparator()
{
	m_nextLeft += SEPARATOR_WIDTH;
}

void Toolbar::Draw(sf::RenderTarget& target) const
{
	sf::RectangleShape background({ target.getView().getSize().x, HEIGHT });
	background.setFillColor(TOOLBAR_COLOR);
	target.draw(background);

	for (const auto& button : m_buttons)
	{
		sf::RectangleShape rect(button.m_rect.size);
		rect.setPosition(button.m_rect.position);
		rect.setFillColor(BUTTON_COLOR);
		rect.setOutlineColor(BUTTON_BORDER_COLOR);
		rect.setOutlineThickness(-1.f);
		target.draw(rect);

		if (m_hasFont)
		{
			sf::Text text(m_font, button.m_label, FONT_SIZE);
			text.setFillColor(TEXT_COLOR);
			const sf::FloatRect textBounds = text.getLocalBounds();
			text.setOrigin(textBounds.position + textBounds.size / 2.f);
			text.setPosition(button.m_rect.position + button.m_rect.size / 2.f);
			target.draw(text);
		}
	}
}

bool Toolbar::HandleClick(const sf::Vector2f point) const
{
	if (!IsContains(point))
	{
		return false;
	}
	for (const auto& button : m_buttons)
	{
		if (button.m_rect.contains(point))
		{
			button.m_onClick();
			break;
		}
	}
	return true;
}

bool Toolbar::IsContains(const sf::Vector2f point) const
{
	return point.y >= 0 && point.y < HEIGHT;
}
