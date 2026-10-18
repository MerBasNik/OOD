//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorController.h"
#include "Command/AddShapeCommand/AddShapeCommand.h"
#include "Command/ChangeShapeFrameCommand/ChangeShapeFrameCommand.h"
#include "Command/DeleteShapeCommand/DeleteShapeCommand.h"
#include "FrameCalculator.h"
#include "Model/EditorState/SelectionFrame.h"
#include "Model/Serialization/DocumentSerializer.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include "ShapeDefaults.h"

EditorController::EditorController(Document& document, EditorState& state)
	: m_document(document)
	, m_state(state)
{
}

void EditorController::AddShape(const ShapeType type)
{
	const Frame frame = ShapeDefaults::GetDefaultFrame(m_document.GetBounds());
	std::unique_ptr<IShape> shape;
	switch (type)
	{
	case ShapeType::Rectangle:
		shape = std::make_unique<Rectangle>(frame, ShapeDefaults::RECTANGLE_COLOR);
		break;
	case ShapeType::Ellipse:
		shape = std::make_unique<Ellipse>(frame, ShapeDefaults::ELLIPSE_COLOR);
		break;
	case ShapeType::Triangle:
		shape = std::make_unique<Triangle>(frame, ShapeDefaults::TRIANGLE_COLOR);
		break;
	}

	AddShapeCommand command(m_document, std::move(shape));
	command.Execute();
	m_state.SetSelectedShapeId(command.GetShapeId());
}

void EditorController::DeleteSelectedShape()
{
	const auto selectedId = m_state.GetSelectedShapeId();
	if (!selectedId)
	{
		return;
	}
	ResetDrag();
	m_state.SetSelectedShapeId(std::nullopt);
	DeleteShapeCommand command(m_document, *selectedId);
	command.Execute();
}

void EditorController::OnMouseDown(const Point point)
{
	const HitResult hit = HitTest(point);
	switch (hit.m_area)
	{
	case HitArea::ResizeHandle:
		StartDrag(DragMode::Resize, *hit.m_shapeId, point, *hit.m_handle);
		break;
	case HitArea::SelectionBorder:
	case HitArea::Shape:
		m_state.SetSelectedShapeId(hit.m_shapeId);
		StartDrag(DragMode::Move, *hit.m_shapeId, point);
		break;
	case HitArea::Empty:
		ResetDrag();
		m_state.SetSelectedShapeId(std::nullopt);
		break;
	}
}

void EditorController::OnMouseMove(const Point point)
{
	if (m_drag.m_mode == DragMode::None)
	{
		return;
	}
	if (!m_document.FindShape(m_drag.m_shapeId))
	{
		ResetDrag();
		return;
	}
	m_state.SetPreviewFrame(CalculateDraggedFrame(point));
}

void EditorController::OnMouseUp(const Point point)
{
	if (m_drag.m_mode == DragMode::None)
	{
		return;
	}
	if (!m_document.FindShape(m_drag.m_shapeId))
	{
		ResetDrag();
		return;
	}

	const int shapeId = m_drag.m_shapeId;
	const Frame startFrame = m_drag.m_startFrame;
	const Frame frame = CalculateDraggedFrame(point);
	ResetDrag();
	if (frame != startFrame)
	{
		ChangeShapeFrameCommand command(m_document, shapeId, frame);
		command.Execute();
	}
}

HitResult EditorController::HitTest(const Point point) const
{
	const auto selectedId = m_state.GetSelectedShapeId();
	if (const auto selectedFrame = GetSelectedFrame())
	{
		if (const auto handle = SelectionFrame::HitTestHandle(*selectedFrame, point))
		{
			return { HitArea::ResizeHandle, selectedId, handle };
		}
		if (SelectionFrame::HitTestBorder(*selectedFrame, point))
		{
			return { HitArea::SelectionBorder, selectedId, std::nullopt };
		}
	}

	if (const auto shapeId = m_document.FindTopShape(point))
	{
		return { HitArea::Shape, shapeId, std::nullopt };
	}
	return {};
}

CursorType EditorController::GetResizeCursor(const ResizeHandle handle)
{
	switch (handle)
	{
	case ResizeHandle::Left:
	case ResizeHandle::Right:
		return CursorType::SizeHorizontal;
	case ResizeHandle::Top:
	case ResizeHandle::Bottom:
		return CursorType::SizeVertical;
	case ResizeHandle::TopLeft:
	case ResizeHandle::BottomRight:
		return CursorType::SizeTopLeftBottomRight;
	case ResizeHandle::TopRight:
	case ResizeHandle::BottomLeft:
		return CursorType::SizeBottomLeftTopRight;
	}
	return CursorType::Arrow;
}

CursorType EditorController::GetCursorType(const Point point) const
{
	switch (m_drag.m_mode)
	{
	case DragMode::Move:
		return CursorType::Move;
	case DragMode::Resize:
		return GetResizeCursor(m_drag.m_handle);
	case DragMode::None:
		break;
	}

	const HitResult hit = HitTest(point);
	switch (hit.m_area)
	{
	case HitArea::ResizeHandle:
		return GetResizeCursor(*hit.m_handle);
	case HitArea::SelectionBorder:
		return CursorType::Move;
	case HitArea::Shape:
	case HitArea::Empty:
		break;
	}
	return CursorType::Arrow;
}

void EditorController::NewDocument()
{
	ResetDrag();
	m_state.SetSelectedShapeId(std::nullopt);
	m_state.SetFilePath(std::nullopt);
	m_document.Clear();
}

void EditorController::SaveDocument(const std::filesystem::path& path)
{
	DocumentSerializer::SaveToFile(m_document, path);
	m_state.SetFilePath(path);
}

void EditorController::OpenDocument(const std::filesystem::path& path)
{
	auto shapes = DocumentSerializer::LoadFromFile(path);
	const Frame bounds = m_document.GetBounds();
	for (const auto& shape : shapes)
	{
		const Frame frame = shape->GetFrame();
		if (!bounds.IsContainsFrame(frame))
		{
			throw std::runtime_error("Фигура выходит за пределы холста");
		}
		if (frame.m_width < ShapeDefaults::MIN_SIZE || frame.m_height < ShapeDefaults::MIN_SIZE)
		{
			throw std::runtime_error("Размер фигуры меньше минимально допустимого");
		}
	}

	ResetDrag();
	m_state.SetSelectedShapeId(std::nullopt);
	m_document.ReplaceShapes(std::move(shapes));
	m_state.SetFilePath(path);
}

std::optional<Frame> EditorController::GetSelectedFrame() const
{
	const auto selectedId = m_state.GetSelectedShapeId();
	if (!selectedId)
	{
		return std::nullopt;
	}
	const IShape* shape = m_document.FindShape(*selectedId);
	if (!shape)
	{
		return std::nullopt;
	}
	return m_state.GetPreviewFrame().value_or(shape->GetFrame());
}

void EditorController::ResetDrag()
{
	m_drag = {};
	m_state.SetPreviewFrame(std::nullopt);
}

Frame EditorController::CalculateDraggedFrame(const Point point) const
{
	const Point offset{ point.m_x - m_drag.m_startPoint.m_x, point.m_y - m_drag.m_startPoint.m_y };
	const Frame bounds = m_document.GetBounds();
	return m_drag.m_mode == DragMode::Move
		? FrameCalculator::CalculateMovedFrame(m_drag.m_startFrame, offset, bounds)
		: FrameCalculator::CalculateResizedFrame(m_drag.m_startFrame, m_drag.m_handle, offset, bounds, ShapeDefaults::MIN_SIZE);
}

void EditorController::StartDrag(const DragMode mode, const int id, const Point point, const ResizeHandle handle)
{
	// Предыдущий жест мог не получить OnMouseUp, например если кнопку отпустили за пределами окна
	ResetDrag();
	const IShape* shape = m_document.FindShape(id);
	if (!shape)
	{
		return;
	}
	m_drag = { mode, id, handle, point, shape->GetFrame() };
}
