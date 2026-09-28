//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_SHAPE_H
#define OOD_SHAPE_H
#include "IShape.h"

class Shape : public IShape
{
public:
	~Shape() override;
	void Shape(std::string id, Color color, std::unique_ptr<IShapeBehavior> behavior) override;
	std::string GetId() override;
	Color GetColor() override;
	void ChangeColor(Color color) override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas) override;
	void ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior) override;

private:
	std::string m_id;
	Color m_color;
	std::unique_ptr<IShapeBehavior> m_behavior;
};

#endif //OOD_SHAPE_H
