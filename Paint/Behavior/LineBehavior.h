//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_LINEBEHAVIOR_H
#define OOD_LINEBEHAVIOR_H
#include "IShapeBehavior.h"

class LineBehavior : public IShapeBehavior
{
public:
	~LineBehavior() override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) override;
	std::string GetInfo() override;
	std::string GetName() override;

private:
	Point m_start = {};
	Point m_end = {};
};

#endif //OOD_LINEBEHAVIOR_H
