//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_RECTANGLEBEHAVIOR_H
#define OOD_RECTANGLEBEHAVIOR_H
#include "IShapeBehavior.h"

class RectangleBehavior : public IShapeBehavior
{
public:
	~RectangleBehavior() override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) override;
	std::string GetInfo() override;
	std::string GetName() override;

private:
	Point m_position = {};
	double m_width = 0;
	double m_height = 0;
};

#endif //OOD_RECTANGLEBEHAVIOR_H
