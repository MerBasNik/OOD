//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_ISHAPE_H
#define OOD_EDITOR_ISHAPE_H

#include "ICanvas.h"
#include "Model/Types/Color.h"
#include "Model/Types/Frame.h"
#include "ShapeType.h"
#include <memory>

class IShape
{
public:
	virtual ~IShape() = default;

	virtual ShapeType GetType() const = 0;
	virtual bool HitTest(Point point) const = 0;
	virtual void Draw(ICanvas& canvas) const = 0;
	virtual std::unique_ptr<IShape> Clone() const = 0;

	virtual Frame GetFrame() const = 0;
	virtual void SetFrame(const Frame& frame) = 0;
	virtual Color GetColor() const = 0;
};

#endif // OOD_EDITOR_ISHAPE_H
