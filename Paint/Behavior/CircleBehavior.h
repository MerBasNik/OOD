//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_CIRCLEBEHAVIOR_H
#define OOD_CIRCLEBEHAVIOR_H
#include "IShapeBehavior.h"

class CircleBehavior : public IShapeBehavior
{
public:
	~CircleBehavior() override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) override;
	std::string GetInfo() override;
	std::string GetName() override;

private:
	Point m_position = {};
	double m_radius = 0;
};

#endif //OOD_CIRCLEBEHAVIOR_H
