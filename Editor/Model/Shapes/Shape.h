//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_SHAPE_H
#define OOD_EDITOR_SHAPE_H

#include "IShape.h"

class Shape : public IShape
{
public:
	Shape(const Frame& frame, Color fillColor, Color borderColor);
	Frame GetFrame() const override;
	void SetFrame(const Frame& frame) override;
	Color GetFillColor() const override;
	Color GetBorderColor() const override;
	void SetFillColor(Color color) override;
	void SetBorderColor(Color color) override;
	TimeInterval GetTimeInterval() const override;
	void SetTimeInterval(TimeInterval timeInterval) override;
	// почему два метода?
	GroupShape* GetGroup() override;
	const GroupShape* GetGroup() const override;

private:
	Frame m_frame;
	Color m_fillColor;
	Color m_borderColor;
	TimeInterval m_timeInterval;
};

#endif // OOD_EDITOR_SHAPE_H
