//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_TEXTBEHAVIOR_H
#define OOD_TEXTBEHAVIOR_H
#include "IShapeBehavior.h"

class TextBehavior : public IShapeBehavior
{
public:
	TextBehavior(Point position, double fontSize, const std::string& text);
	~TextBehavior() override = default;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) const override;
	std::string GetInfo() const override;
	std::string GetName() const override;
	std::unique_ptr<IShapeBehavior> Clone() const override;

private:
	Point m_position = {};
	double m_fontSize = 0;
	std::string m_text = "";
};

#endif //OOD_TEXTBEHAVIOR_H
