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
	virtual std::string GetId() const = 0;
	virtual void SetId(const std::string& newId) = 0;
	virtual Color GetColor() const = 0;
	virtual std::string GetName() const = 0;
	virtual std::string GetInfo() const = 0;
	virtual void ChangeColor(Color color) = 0;
	virtual void Move(Point position) = 0;
	virtual void Draw(ICanvas& canvas) const = 0;
	virtual void ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior) = 0;
	virtual std::unique_ptr<IShape> Clone() const = 0;
};

#endif //OOD_ISHAPE_H
