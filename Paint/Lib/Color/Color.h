//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_COLOR_H
#define OOD_COLOR_H
#include <string>

class Color
{
public:
	Color();
	Color(uint8_t red, uint8_t green, uint8_t blue);
	Color ParseColorFromString(const std::string& hexString);
	std::string ToString();
	uint8_t GetRed() const;
	uint8_t GetGreen() const;
	uint8_t GetBlue() const;

private:
	std::uint8_t m_red;
	std::uint8_t m_green;
	std::uint8_t m_blue;
};

#endif //OOD_COLOR_H
