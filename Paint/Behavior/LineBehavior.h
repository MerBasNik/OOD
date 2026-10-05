//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_LINEBEHAVIOR_H
#define OOD_LINEBEHAVIOR_H
#include "IShapeBehavior.h"

class LineBehavior : public IShapeBehavior
{
public:
	LineBehavior(Point start, Point end);
	~LineBehavior() override = default;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) const override;
	std::string GetInfo() const override;
	std::string GetName() const override;
	std::unique_ptr<IShapeBehavior> Clone() const override;

private:
	Point m_start = {};
	Point m_end = {};
};

#endif // OOD_LINEBEHAVIOR_H
