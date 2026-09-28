//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_TRIANGLEBEHAVIOR_H
#define OOD_TRIANGLEBEHAVIOR_H
#include "IShapeBehavior.h"

class TriangleBehavior : public IShapeBehavior
{
public:
	~TriangleBehavior() override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas, Color color) override;
	std::string GetInfo() override;
	std::string GetName() override;

private:
	Point m_v1 = {};
	Point m_v2 = {};
	Point m_v3 = {};
};

#endif //OOD_TRIANGLEBEHAVIOR_H
