//
// Created by Вадим Патрушев on 09.11.2026.
//

#ifndef OOD_EDITOR_VIEWPORT_H
#define OOD_EDITOR_VIEWPORT_H

#include "Model/Types/Point.h"

class Viewport
{
public:
	static constexpr double ZOOM_STEP = 1.25;

	Viewport(Point contentSize, Point viewSize, double minZoom, double maxZoom);
	double GetZoom() const;
	Point GetOffset() const;
	Point ToContent(Point viewPoint) const;
	Point ToView(Point contentPoint) const;
	void ZoomAt(double factor, Point viewPoint);
	void ZoomIn();
	void ZoomOut();
	void ResetZoom();
	void Scroll(Point viewDelta);

private:
	Point m_contentSize;
	Point m_viewSize;
	double m_minZoom;
	double m_maxZoom;
	double m_zoom = 1;
	Point m_offset{ 0, 0 };

	Point GetViewCenter() const;
	void ClampOffset();
	static double ClampAxis(double offset, double contentSize, double visibleSize);
};

#endif // OOD_EDITOR_VIEWPORT_H
