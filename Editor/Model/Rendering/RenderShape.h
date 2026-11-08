//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_RENDERSHAPE_H
#define OOD_EDITOR_RENDERSHAPE_H

#include "../../View/Canvas/ICanvas.h"
#include "Model/Shapes/IShapeVisitor.h"
#include <optional>

class RenderShape : public IShapeVisitor
{
public:
	explicit RenderShape(ICanvas& canvas, std::optional<double> time = std::nullopt);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;
	void Visit(const GroupShape& group) override;

private:
	ICanvas& m_canvas;
	// для чего нужен m_time?
	std::optional<double> m_time;
};

#endif // OOD_EDITOR_RENDERSHAPE_H
