//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_RENDERSHAPE_H
#define OOD_EDITOR_RENDERSHAPE_H

#include "../../View/Canvas/ICanvas.h"
#include "Model/Shapes/IShapeVisitor.h"

class RenderShape : public IShapeVisitor
{
public:
	explicit RenderShape(ICanvas& canvas);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;

private:
	ICanvas& m_canvas;
};

#endif // OOD_EDITOR_RENDERSHAPE_H
