//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_SHAPE_H
#define OOD_EDITOR_SHAPE_H

#include "IShape.h"

class Shape : public IShape
{
public:
	Shape(const Frame& frame, Color color);
	Frame GetFrame() const override;
	void SetFrame(const Frame& frame) override;
	Color GetColor() const override;

private:
	Frame m_frame;
	Color m_color;
};

#endif // OOD_EDITOR_SHAPE_H
