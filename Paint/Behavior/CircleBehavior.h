//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_CIRCLEBEHAVIOR_H
#define OOD_CIRCLEBEHAVIOR_H
#include "IShapeBehavior.h"

class CircleBehavior : public IShapeBehavior
{
public:
	CircleBehavior(Point position, double radius);
	~CircleBehavior() override = default;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) const override;
	std::string GetInfo() const override;
	std::string GetName() const override;
	std::unique_ptr<IShapeBehavior> Clone() const override;

private:
	Point m_position = {};
	double m_radius = 0;
};

#endif // OOD_CIRCLEBEHAVIOR_H
