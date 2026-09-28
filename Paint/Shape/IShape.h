//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_ISHAPE_H
#define OOD_ISHAPE_H
#include "IShapeBehavior.h"

class IShape
{
public:
	virtual ~IShape() = default;
	virtual void Shape(std::string id, Color color, std::unique_ptr<IShapeBehavior> behavior);
	virtual std::string GetId();
	virtual Color GetColor();
	virtual void ChangeColor(Color color);
	virtual void Move(Point position);
	virtual void Draw(ICanvas& canvas);
	virtual void ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior);
};

#endif //OOD_ISHAPE_H
