//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_ISHAPEBEHAVIOR_H
#define OOD_ISHAPEBEHAVIOR_H
#include "../Lib/Color.h"
#include "ICanvas.h"
#include "Point.h"

#include <string>

class IShapeBehavior
{
public:
	virtual ~IShapeBehavior() = default;
	virtual void Move(Point position) = 0;
	virtual void Draw(ICanvas& canvas, Color color) const = 0;
	virtual std::string GetInfo() const = 0;
	virtual std::string GetName() const = 0;
	virtual std::unique_ptr<IShapeBehavior> Clone() const = 0;
};

#endif // OOD_ISHAPEBEHAVIOR_H
