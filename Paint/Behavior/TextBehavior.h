//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_TEXTBEHAVIOR_H
#define OOD_TEXTBEHAVIOR_H
#include "IShapeBehavior.h"

class TextBehavior : public IShapeBehavior
{
public:
	~TextBehavior() override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) override;
	std::string GetInfo() override;
	std::string GetName() override;

private:
	Point m_position = {};
	double m_fontSize = 0;
	std::string m_text = "";
};

#endif //OOD_TEXTBEHAVIOR_H
