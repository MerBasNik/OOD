//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_TRIANGLEBEHAVIOR_H
#define OOD_TRIANGLEBEHAVIOR_H
#include "IShapeBehavior.h"

class TriangleBehavior : public IShapeBehavior
{
public:
	TriangleBehavior(Point v1, Point v2, Point v3);
	~TriangleBehavior() override = default;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) const override;
	std::string GetInfo() const override;
	std::string GetName() const override;
	std::unique_ptr<IShapeBehavior> Clone() const override;

private:
	Point m_v1 = {};
	Point m_v2 = {};
	Point m_v3 = {};

	static void MoveVertex(Point& vertex, Point position);
};

#endif // OOD_TRIANGLEBEHAVIOR_H
