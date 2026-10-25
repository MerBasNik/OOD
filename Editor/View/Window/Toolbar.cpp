//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "Toolbar.h"
#include <algorithm>
#include <iostream>

const float PADDING = 6.f;
const float BUTTON_WIDTH = 104.f;
const float TOOL_BUTTON_WIDTH = 36.f;
const float ICON_SIZE = 22.f;
const float SEPARATOR_WIDTH = 16.f;
const unsigned FONT_SIZE = 13;
const std::string FONT_PATH = "/System/Library/Fonts/Supplemental/Arial.ttf";
const sf::Color TOOLBAR_COLOR(236, 236, 236);
const sf::Color BUTTON_COLOR(250, 250, 250);
const sf::Color BUTTON_BORDER_COLOR(170, 170, 170);
const sf::Color ACTIVE_BUTTON_COLOR(205, 225, 250);
const sf::Color ACTIVE_BUTTON_BORDER_COLOR(30, 120, 230);
const sf::Color TEXT_COLOR(30, 30, 30);

Toolbar::Toolbar()
{
	const bool hasFont = m_font.openFromFile(FONT_PATH);
	if (!hasFont)
	{
		std::cerr << "Ошибка при загрузке шрифта" << std::endl;
	}
}

void Toolbar::AddButton(const std::string& label, std::function<void()> onClick)
{
	AddButton({ sf::String::fromUtf8(label.begin(), label.end()), {}, std::move(onClick) }, BUTTON_WIDTH);
}

void Toolbar::AddToolButton(const std::filesystem::path& iconPath, std::function<void()> onClick, std::function<bool()> isActive)
{
	sf::Texture icon;
	if (!icon.loadFromFile(iconPath))
	{
		std::cerr << "Ошибка при загрузке иконки " << iconPath << std::endl;
	}
	AddButton({ {}, {}, std::move(onClick), std::move(isActive), std::move(icon) }, TOOL_BUTTON_WIDTH);
}

void Toolbar::AddButton(Button button, const float width)
{
	const float left = m_buttons.empty() ? PADDING : m_buttons.back().m_rect.position.x + m_buttons.back().m_rect.size.x + SEPARATOR_WIDTH;
	button.m_rect = sf::FloatRect({ left, PADDING }, { width, HEIGHT - 2 * PADDING });
	m_buttons.push_back(std::move(button));
}

void Toolbar::Draw(sf::RenderTarget& target) const
{
	sf::RectangleShape background({ target.getView().getSize().x, HEIGHT });
	background.setFillColor(TOOLBAR_COLOR);
	target.draw(background);

	for (const auto& button : m_buttons)
	{
		const bool isActive = button.m_isActive && button.m_isActive();
		sf::RectangleShape rect(button.m_rect.size);
		rect.setPosition(button.m_rect.position);
		rect.setFillColor(isActive ? ACTIVE_BUTTON_COLOR : BUTTON_COLOR);
		rect.setOutlineColor(isActive ? ACTIVE_BUTTON_BORDER_COLOR : BUTTON_BORDER_COLOR);
		rect.setOutlineThickness(-1.f);
		target.draw(rect);

		if (button.m_icon)
		{
			const sf::Vector2f iconSize(button.m_icon->getSize());
			const float scale = ICON_SIZE / std::max(iconSize.x, iconSize.y);
			sf::Sprite icon(*button.m_icon);
			icon.setOrigin(iconSize / 2.f);
			icon.setScale({ scale, scale });
			icon.setPosition(button.m_rect.position + button.m_rect.size / 2.f);
			target.draw(icon);
			continue;
		}

		sf::Text text(m_font, button.m_label, FONT_SIZE);
		text.setFillColor(TEXT_COLOR);
		const sf::FloatRect textBounds = text.getLocalBounds();
		text.setOrigin(textBounds.position + textBounds.size / 2.f);
		text.setPosition(button.m_rect.position + button.m_rect.size / 2.f);
		target.draw(text);
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
