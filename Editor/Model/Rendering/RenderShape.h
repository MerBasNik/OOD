//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_RENDERSHAPE_H
#define OOD_EDITOR_RENDERSHAPE_H

#include "../../View/Canvas/ICanvas.h"
#include "Model/Shapes/IShapeVisitor.h"
#include "Model/Timeline/Timeline.h"

class RenderShape : public IShapeVisitor
{
public:
	explicit RenderShape(ICanvas& canvas);
	RenderShape(ICanvas& canvas, const Timeline& timeline, double time);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;
	void Visit(const GroupShape& group) override;

private:
	ICanvas& m_canvas;
	const Timeline* m_timeline = nullptr;
	double m_time = 0;
};

#endif // OOD_EDITOR_RENDERSHAPE_H
