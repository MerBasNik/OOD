//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_ISHAPE_H
#define OOD_EDITOR_ISHAPE_H

#include "IShapeVisitor.h"
#include "Model/Types/Color.h"
#include "Model/Types/Frame.h"

#include <memory>

class GroupShape;

class IShape
{
public:
	virtual ~IShape() = default;
	virtual int GetId() const = 0;
	virtual const IShape* FindShape(int id) const = 0;
	virtual IShape* FindShape(int id) = 0;
	virtual bool HitTest(Point point) const = 0;
	virtual std::unique_ptr<IShape> Clone() const = 0;
	virtual Frame GetFrame() const = 0;
	virtual void SetFrame(const Frame& frame) = 0;
	virtual Color GetFillColor() const = 0;
	virtual Color GetBorderColor() const = 0;
	virtual void SetFillColor(Color color) = 0;
	virtual void SetBorderColor(Color color) = 0;
	virtual void Accept(IShapeVisitor& visitor) const = 0;
	virtual GroupShape* GetGroup() = 0;
	virtual const GroupShape* GetGroup() const = 0;

	inline static int m_nextShapeId = 1;
	static int GenerateShapeId()
	{
		return m_nextShapeId++;
	}
};

#endif // OOD_EDITOR_ISHAPE_H
