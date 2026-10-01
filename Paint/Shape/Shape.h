//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_SHAPE_H
#define OOD_SHAPE_H
#include "IShape.h"

class Shape : public IShape
{
public:
	~Shape() override = default;
	Shape(const std::string &id, Color color, std::unique_ptr<IShapeBehavior> behavior);
	std::string GetId() const override;
	Color GetColor() const override;
	std::string GetName() const override;
	std::string GetInfo() const override;
	void ChangeColor(Color color) override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas) const override;
	void ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior) override;
	std::unique_ptr<IShape> Clone() const override;

private:
	std::string m_id;
	Color m_color;
	std::unique_ptr<IShapeBehavior> m_behavior;
};

#endif //OOD_SHAPE_H
