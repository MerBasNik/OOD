//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_ISHAPEBEHAVIOR_H
#define OOD_ISHAPEBEHAVIOR_H
#include "../Lib/Color/Color.h"
#include "ICanvas.h"
#include "Point.h"

#include <string>

class IShapeBehavior
{
public:
	virtual ~IShapeBehavior() = default;
	virtual void Move(Point position);
	virtual void Draw(ICanvas& canvas, Color color);
	virtual std::string GetInfo();
	virtual std::string GetName();
};


#endif //OOD_ISHAPEBEHAVIOR_H
