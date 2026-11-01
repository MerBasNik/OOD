//
// Created by Вадим Патрушев on 01.11.2026.
//

#include "ColorPicker.h"
#include "View/Canvas/Canvas.h"
#include <array>
#include <format>

const std::array<Color, 32> PALETTE = {
	0x000000, 0x222222, 0x555555, 0x808080, 0xaaaaaa, 0xcccccc, 0xeeeeee, 0xffffff,
	0xee1111, 0xff8800, 0xffdd00, 0x11ee11, 0x00bbbb, 0x1111ee, 0x8800cc, 0xee11aa,
	0xff9999, 0xffcc88, 0xffee99, 0x99ee99, 0x99dddd, 0x9999ff, 0xcc99ee, 0xff99dd,
	0x880000, 0x884400, 0x887700, 0x006600, 0x006666, 0x000088, 0x440066, 0x880044
};
const size_t COLUMN_COUNT = 8;
const float PADDING = 8.f;
const float TITLE_HEIGHT = 20.f;
const float SWATCH_SIZE = 24.f;
const float SWATCH_GAP = 4.f;
const float FOOTER_HEIGHT = 26.f;
const float BUTTON_WIDTH = 80.f;
const unsigned FONT_SIZE = 13;
const float GRID_WIDTH = COLUMN_COUNT * SWATCH_SIZE + (COLUMN_COUNT - 1) * SWATCH_GAP;
const float GRID_HEIGHT = static_cast<float>(PALETTE.size() / COLUMN_COUNT) * (SWATCH_SIZE + SWATCH_GAP) - SWATCH_GAP;
const sf::Vector2f PICKER_SIZE(GRID_WIDTH + 2 * PADDING, TITLE_HEIGHT + GRID_HEIGHT + FOOTER_HEIGHT + 3 * PADDING);
const sf::Color BACKGROUND_COLOR(250, 250, 250);
const sf::Color BORDER_COLOR(170, 170, 170);
const sf::Color SWATCH_BORDER_COLOR(120, 120, 120);
const sf::Color SELECTED_COLOR(30, 120, 230);
const sf::Color BUTTON_COLOR(240, 240, 240);
const sf::Color TEXT_COLOR(30, 30, 30);

ColorPicker::ColorPicker(const sf::Font& font, std::function<void()> onApply, std::function<void()> onCancel)
	: m_font(font)
	, m_onApply(std::move(onApply))
	, m_onCancel(std::move(onCancel))
{
}

void ColorPicker::Open(const std::string& title, const sf::Vector2f position, const Color color, std::function<void(Color)> onSelect)
{
	m_isOpen = true;
	m_title = sf::String::fromUtf8(title.begin(), title.end());
	m_rect = sf::FloatRect(position, PICKER_SIZE);
	m_color = color;
	m_onSelect = std::move(onSelect);

	const std::string applyLabel = "Применить";
	const std::string cancelLabel = "Отмена";
	const float buttonTop = m_rect.position.y + m_rect.size.y - PADDING - FOOTER_HEIGHT;
	const float cancelLeft = m_rect.position.x + m_rect.size.x - PADDING - BUTTON_WIDTH;
	const float applyLeft = cancelLeft - PADDING - BUTTON_WIDTH;
	m_buttons = {
		{ sf::String::fromUtf8(applyLabel.begin(), applyLabel.end()), sf::FloatRect({ applyLeft, buttonTop }, { BUTTON_WIDTH, FOOTER_HEIGHT }), [this] {
			 Close();
			 m_onApply();
		 } },
		{ sf::String::fromUtf8(cancelLabel.begin(), cancelLabel.end()), sf::FloatRect({ cancelLeft, buttonTop }, { BUTTON_WIDTH, FOOTER_HEIGHT }), [this] {
			 Cancel();
		 } },
	};
}

void ColorPicker::Cancel()
{
	if (!m_isOpen)
	{
		return;
	}
	Close();
	m_onCancel();
}

bool ColorPicker::IsOpen() const
{
	return m_isOpen;
}

bool ColorPicker::IsContains(const sf::Vector2f point) const
{
	return m_isOpen && m_rect.contains(point);
}

bool ColorPicker::HandleClick(const sf::Vector2f point)
{
	if (!IsContains(point))
	{
		return false;
	}
	for (size_t i = 0; i < PALETTE.size(); ++i)
	{
		if (GetSwatchRect(i).contains(point))
		{
			m_color = PALETTE[i];
			m_onSelect(m_color);
			return true;
		}
	}
	for (const auto& button : m_buttons)
	{
		if (button.m_rect.contains(point))
		{
			const auto onClick = button.m_onClick;
			onClick();
			break;
		}
	}
	return true;
}

void ColorPicker::Draw(sf::RenderTarget& target) const
{
	if (!m_isOpen)
	{
		return;
	}
	sf::RectangleShape background(m_rect.size);
	background.setPosition(m_rect.position);
	background.setFillColor(BACKGROUND_COLOR);
	background.setOutlineColor(BORDER_COLOR);
	background.setOutlineThickness(1.f);
	target.draw(background);

	sf::Text title(m_font, m_title, FONT_SIZE);
	title.setFillColor(TEXT_COLOR);
	title.setPosition(m_rect.position + sf::Vector2f(PADDING, PADDING));
	target.draw(title);

	DrawSwatches(target);
	DrawButtons(target);
}

void ColorPicker::Close()
{
	m_isOpen = false;
	m_onSelect = nullptr;
}

sf::FloatRect ColorPicker::GetSwatchRect(const size_t index) const
{
	const float column = static_cast<float>(index % COLUMN_COUNT);
	const float row = static_cast<float>(index / COLUMN_COUNT);
	const sf::Vector2f gridPosition = m_rect.position + sf::Vector2f(PADDING, 2 * PADDING + TITLE_HEIGHT);
	return { gridPosition + sf::Vector2f(column, row) * (SWATCH_SIZE + SWATCH_GAP), { SWATCH_SIZE, SWATCH_SIZE } };
}

void ColorPicker::DrawSwatches(sf::RenderTarget& target) const
{
	for (size_t i = 0; i < PALETTE.size(); ++i)
	{
		const sf::FloatRect rect = GetSwatchRect(i);
		const bool isSelected = PALETTE[i] == m_color;
		sf::RectangleShape swatch(rect.size);
		swatch.setPosition(rect.position);
		swatch.setFillColor(Canvas::ConvertColorToSf(PALETTE[i]));
		swatch.setOutlineColor(isSelected ? SELECTED_COLOR : SWATCH_BORDER_COLOR);
		swatch.setOutlineThickness(isSelected ? 2.f : -1.f);
		target.draw(swatch);
	}
}

void ColorPicker::DrawButtons(sf::RenderTarget& target) const
{
	for (const auto& button : m_buttons)
	{
		sf::RectangleShape rect(button.m_rect.size);
		rect.setPosition(button.m_rect.position);
		rect.setFillColor(BUTTON_COLOR);
		rect.setOutlineColor(BORDER_COLOR);
		rect.setOutlineThickness(-1.f);
		target.draw(rect);

		sf::Text text(m_font, button.m_label, FONT_SIZE);
		text.setFillColor(TEXT_COLOR);
		const sf::FloatRect textBounds = text.getLocalBounds();
		text.setOrigin(textBounds.position + textBounds.size / 2.f);
		text.setPosition(button.m_rect.position + button.m_rect.size / 2.f);
		target.draw(text);
	}
}
