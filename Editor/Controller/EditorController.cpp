//
// Created by Вадим Патрушев on 15.10.2026.
//

#include "EditorController.h"
#include "Command/AddShapeCommand/AddShapeCommand.h"
#include "Command/ChangeShapeColorsCommand/ChangeShapeColorsCommand.h"
#include "Command/ChangeShapeFrameCommand/ChangeShapeFrameCommand.h"
#include "Command/DeleteShapeCommand/DeleteShapeCommand.h"
#include "Command/GroupShapesCommand/GroupShapesCommand.h"
#include "Command/MacroCommand/MacroCommand.h"
#include "Command/UngroupShapeCommand/UngroupShapeCommand.h"
#include "FrameCalculator.h"
#include "Model/EditorState/SelectionFrame.h"
#include "Model/Shapes/Ellipse.h"
#include "Model/Shapes/GroupShape.h"
#include "Model/Shapes/Rectangle.h"
#include "Model/Shapes/Triangle.h"
#include "ShapeDefaults.h"
#include <algorithm>
#include <stdexcept>

EditorController::EditorController(Document& document, EditorState& state, CommandHistory& history)
	: m_document(document)
	, m_state(state)
	, m_history(history)
{
	m_document.AddObserver(*this);
}

EditorController::~EditorController()
{
	m_document.RemoveObserver(*this);
}

void EditorController::AddShape(const ShapeType type)
{
	const Frame frame = ShapeDefaults::GetDefaultFrame(m_document.GetBounds());
	AddSelectShape(CreateShape(type, frame));
}

void EditorController::DeleteSelectedShapes()
{
	const auto& selectedIds = m_state.GetSelectedShapeIds();
	std::vector<std::unique_ptr<ICommand>> commands;
	std::set<int> deletedIds;
	for (size_t i = m_document.GetShapeCount(); i > 0; --i)
	{
		AppendDeleteCommands(m_document.GetShape(i - 1), selectedIds, commands, deletedIds);
	}
	if (commands.empty())
	{
		return;
	}
	std::vector<int> shapeIds;
	for (const int id : selectedIds)
	{
		if (deletedIds.erase(id))
		{
			shapeIds.push_back(id);
		}
	}
	auto command = std::make_unique<MacroCommand>(std::move(commands), std::move(shapeIds));
	ResetPreview();
	m_state.SetSelectedShapeIds({});
	ExecuteCommand(std::move(command));
}

bool EditorController::AppendDeleteCommands(const IShape& shape, const std::vector<int>& shapeIds,
	std::vector<std::unique_ptr<ICommand>>& commands, std::set<int>& deletedIds) const
{
	if (std::ranges::find(shapeIds, shape.GetId()) != shapeIds.end())
	{
		commands.push_back(std::make_unique<DeleteShapeCommand>(m_document, shape.GetId()));
		deletedIds.insert(shape.GetId());
		return true;
	}
	const GroupShape* group = shape.GetGroup();
	if (!group)
	{
		return false;
	}
	const size_t previousCommandCount = commands.size();
	size_t remainingChildren = 0;
	for (size_t i = group->GetChildCount(); i > 0; --i)
	{
		if (!AppendDeleteCommands(group->GetChild(i - 1), shapeIds, commands, deletedIds))
		{
			++remainingChildren;
		}
	}
	if (commands.size() == previousCommandCount || remainingChildren >= 2)
	{
		return false;
	}
	commands.push_back(std::make_unique<UngroupShapeCommand>(m_document, shape.GetId()));
	return remainingChildren == 0;
}

void EditorController::GroupSelectedShapes()
{
	std::vector<int> shapeIds;
	for (const int id : m_state.GetSelectedShapeIds())
	{
		if (m_document.FindShapeIndex(id))
		{
			shapeIds.push_back(id);
		}
	}
	if (shapeIds.size() < 2)
	{
		return;
	}
	ResetPreview();
	auto command = std::make_unique<GroupShapesCommand>(m_document, std::move(shapeIds));
	const GroupShapesCommand& groupCommand = *command;
	ExecuteCommand(std::move(command));
	m_state.SetSelectedShapeIds({ *groupCommand.GetGroupId() });
}

void EditorController::UngroupSelectedShape()
{
	const auto& selectedIds = m_state.GetSelectedShapeIds();
	if (selectedIds.size() != 1)
	{
		return;
	}
	const int id = selectedIds.front();
	const IShape* shape = m_document.FindShape(id);
	if (!shape || !shape->GetGroup() || !m_document.FindShapeIndex(id))
	{
		return;
	}
	ResetPreview();
	auto command = std::make_unique<UngroupShapeCommand>(m_document, id);
	const UngroupShapeCommand& ungroupCommand = *command;
	ExecuteCommand(std::move(command));
	m_state.SetSelectedShapeIds(ungroupCommand.GetChildIds());
}

void EditorController::SelectShapeTool(const std::optional<ShapeType> type)
{
	ResetPreview();
	m_state.SetShapeTool(type);
}

void EditorController::PreviewFillColor(const Color color) const
{
	m_state.SetPreviewFillColor(color);
}

void EditorController::PreviewBorderColor(const Color color) const
{
	m_state.SetPreviewBorderColor(color);
}

void EditorController::CommitColors()
{
	const auto fillColor = m_state.GetPreviewFillColor();
	const auto borderColor = m_state.GetPreviewBorderColor();
	ResetPreview();
	if (!fillColor && !borderColor)
	{
		return;
	}

	std::vector<std::unique_ptr<ICommand>> commands;
	for (const int id : m_state.GetSelectedShapeIds())
	{
		const IShape* shape = m_document.FindShape(id);
		if (!shape || shape->GetGroup())
		{
			continue;
		}
		const Color newFillColor = fillColor ? *fillColor : shape->GetFillColor();
		const Color newBorderColor = borderColor ? *borderColor : shape->GetBorderColor();
		if (newFillColor != shape->GetFillColor() || newBorderColor != shape->GetBorderColor())
		{
			commands.push_back(std::make_unique<ChangeShapeColorsCommand>(m_document, id, newFillColor, newBorderColor));
		}
	}
	ExecuteCommands(std::move(commands));
}

void EditorController::CancelColors() const
{
	m_state.SetPreviewFillColor(std::nullopt);
	m_state.SetPreviewBorderColor(std::nullopt);
}

void EditorController::Cancel()
{
	if (m_drag.m_mode != DragMode::None)
	{
		ResetPreview();
		return;
	}
	m_state.SetShapeTool(std::nullopt);
}

void EditorController::Undo()
{
	ResetPreview();
	std::vector<int> shapeIds;
	m_document.StartUpdate();
	try
	{
		shapeIds = m_history.Undo();
	}
	catch (...)
	{
		m_document.EndUpdate();
		throw;
	}
	m_document.EndUpdate();
	SelectShapes(shapeIds);
}

void EditorController::Redo()
{
	ResetPreview();
	std::vector<int> shapeIds;
	m_document.StartUpdate();
	try
	{
		shapeIds = m_history.Redo();
	}
	catch (...)
	{
		m_document.EndUpdate();
		throw;
	}
	m_document.EndUpdate();
	SelectShapes(shapeIds);
}

void EditorController::OnMouseDown(const Point point, const bool isToggle)
{
	if (m_state.GetShapeTool())
	{
		StartCreate(point);
		return;
	}
	if (isToggle)
	{
		ResetPreview();
		if (const auto rootId = m_document.FindTopShape(point, m_state.GetCurrentTime()))
		{
			const int id = GetClickShapeId(*rootId, point);
			const auto& selected = m_state.GetSelectedShapeIds();
			if (!selected.empty())
			{
				const auto selectedRoot = m_document.FindRootShapeId(selected.front());
				const bool selectedChild = selectedRoot && *selectedRoot != selected.front();
				const bool clickedChild = id != *rootId;
				if (selectedChild != clickedChild || (clickedChild && selectedRoot != rootId))
				{
					m_state.SetSelectedShapeIds({});
				}
			}
			m_state.ToggleShapeSelection(id);
		}
		return;
	}

	const HitResult hit = HitTest(point);
	switch (hit.m_area)
	{
	case HitArea::ResizeHandle:
		StartDrag(DragMode::Resize, *hit.m_shapeId, point, *hit.m_handle);
		break;
	case HitArea::SelectionBorder:
	case HitArea::Shape: {
		const bool wasSelected = m_state.IsShapeSelected(*hit.m_shapeId);
		if (!wasSelected)
		{
			m_state.SetSelectedShapeIds({ *hit.m_shapeId });
		}
		StartDrag(DragMode::Move, *hit.m_shapeId, point);
		if (wasSelected && hit.m_area == HitArea::Shape && m_document.FindShapeIndex(*hit.m_shapeId).has_value())
		{
			m_drag.m_clickTarget = m_document.FindTargetShape(*hit.m_shapeId, point, m_state.GetCurrentTime());
		}
		break;
	}
	case HitArea::Empty:
		ResetPreview();
		m_state.SetSelectedShapeIds({});
		break;
	}
}

void EditorController::OnMouseMove(const Point point)
{
	switch (m_drag.m_mode)
	{
	case DragMode::None:
		break;
	case DragMode::Create:
		UpdateCreateDirection(point);
		m_state.SetCreatingShape(CreateShape(*m_state.GetShapeTool(), CalculateCreatedFrame(point)));
		break;
	case DragMode::Move:
	case DragMode::Resize: {
		if (point != m_drag.m_startPoint)
		{
			m_drag.m_clickTarget.reset();
		}
		const auto frames = CalculateDraggedFrames(point);
		std::map<int, Frame> previewFrames;
		for (size_t i = 0; i < frames.size(); ++i)
		{
			previewFrames.emplace(m_drag.m_shapeIds[i], frames[i]);
		}
		m_state.SetPreviewFrames(std::move(previewFrames));
		break;
	}
	}
}

void EditorController::OnMouseUp(const Point point)
{
	switch (m_drag.m_mode)
	{
	case DragMode::None:
		break;
	case DragMode::Create:
		FinishCreate(point);
		break;
	case DragMode::Move:
	case DragMode::Resize:
		FinishDrag(point);
		break;
	}
}

int EditorController::GetClickShapeId(const int rootId, const Point point) const
{
	const auto& selected = m_state.GetSelectedShapeIds();
	const bool isEditingChildren = !selected.empty() && selected.front() != rootId
		&& m_document.FindRootShapeId(selected.front()) == rootId;
	if (isEditingChildren)
	{
		if (const auto child = m_document.FindTargetShape(rootId, point, m_state.GetCurrentTime()))
		{
			return *child;
		}
	}
	return rootId;
}

HitResult EditorController::HitTest(const Point point) const
{
	const auto& selectedIds = m_state.GetSelectedShapeIds();
	for (const int id : selectedIds)
	{
		const auto frame = GetShapeFrame(id);
		const auto handle = frame ? SelectionFrame::HitTestHandle(*frame, point) : std::nullopt;
		if (handle)
		{
			return { HitArea::ResizeHandle, id, handle };
		}
	}
	for (const int id : selectedIds)
	{
		const auto frame = GetShapeFrame(id);
		if (frame && SelectionFrame::HitTestBorder(*frame, point))
		{
			return { HitArea::SelectionBorder, id, std::nullopt };
		}
	}

	if (const auto rootId = m_document.FindTopShape(point, m_state.GetCurrentTime()))
	{
		return { HitArea::Shape, GetClickShapeId(*rootId, point), std::nullopt };
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
	case DragMode::Create:
		return CursorType::Cross;
	case DragMode::None:
		break;
	}
	if (m_state.GetShapeTool())
	{
		return CursorType::Cross;
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

void EditorController::DocumentUpdate()
{
	for (size_t i = 0; i < m_drag.m_shapeIds.size(); ++i)
	{
		const IShape* shape = m_document.FindShape(m_drag.m_shapeIds[i]);
		if (!shape || m_document.FindRootShapeId(m_drag.m_shapeIds[i]) != m_drag.m_rootIds[i]
			|| shape->GetFrame() != m_drag.m_startFrames[i])
		{
			ResetPreview();
			break;
		}
	}
	if (m_drag.m_clickTarget && !m_document.FindShape(*m_drag.m_clickTarget))
	{
		ResetPreview();
	}
	RemoveMissingShapesFromState();
}

std::optional<Frame> EditorController::GetShapeFrame(const int id) const
{
	const IShape* shape = m_document.FindShape(id);
	if (!shape || !shape->GetTimeInterval().IsContains(m_state.GetCurrentTime()))
	{
		return std::nullopt;
	}
	const auto previewFrame = m_state.GetPreviewFrame(id);
	return previewFrame ? *previewFrame : shape->GetFrame();
}

void EditorController::ResetPreview()
{
	m_drag = {};
	m_state.SetPreviewFrames({});
	m_state.SetCreatingShape(nullptr);
	CancelColors();
}

std::vector<Frame> EditorController::CalculateDraggedFrames(const Point point) const
{
	const Point offset{ point.m_x - m_drag.m_startPoint.m_x, point.m_y - m_drag.m_startPoint.m_y };
	const Frame bounds = m_document.GetBounds();
	if (m_drag.m_mode == DragMode::Move)
	{
		return FrameCalculator::CalculateGroupMovedFrames(m_drag.m_startFrames, offset, bounds);
	}
	const bool scalePositions = m_drag.m_shapeIds.size() > 1 && !m_document.FindShapeIndex(m_drag.m_shapeIds.front());
	return FrameCalculator::CalculateGroupResizedFrames(m_drag.m_startFrames, m_drag.m_shapeIndex, m_drag.m_handle,
		offset, bounds, ShapeDefaults::MIN_SIZE, scalePositions);
}

void EditorController::ExecuteCommand(std::unique_ptr<ICommand> command) const
{
	m_document.StartUpdate();
	try
	{
		m_history.Execute(std::move(command));
	}
	catch (...)
	{
		m_document.EndUpdate();
		throw;
	}
	m_document.EndUpdate();
}

void EditorController::ExecuteCommands(std::vector<std::unique_ptr<ICommand>> commands) const
{
	if (commands.empty())
	{
		return;
	}
	ExecuteCommand(std::make_unique<MacroCommand>(std::move(commands)));
}

Frame EditorController::CalculateCreatedFrame(const Point point) const
{
	return FrameCalculator::CalculateCreatedFrame(m_drag.m_startPoint, point, m_document.GetBounds(), ShapeDefaults::MIN_SIZE,
		m_drag.m_isCreateForwardX, m_drag.m_isCreateForwardY);
}

void EditorController::UpdateCreateDirection(const Point point)
{
	m_drag.m_isCreateForwardX = FrameCalculator::IsCreateForward(m_drag.m_startPoint.m_x, point.m_x,
		ShapeDefaults::MIN_SIZE, m_drag.m_isCreateForwardX);
	m_drag.m_isCreateForwardY = FrameCalculator::IsCreateForward(m_drag.m_startPoint.m_y, point.m_y,
		ShapeDefaults::MIN_SIZE, m_drag.m_isCreateForwardY);
}

std::unique_ptr<IShape> EditorController::CreateShape(const ShapeType type, const Frame& frame)
{
	switch (type)
	{
	case ShapeType::Rectangle:
		return std::make_unique<Rectangle>(frame, ShapeDefaults::RECTANGLE_COLOR, ShapeDefaults::BORDER_COLOR);
	case ShapeType::Ellipse:
		return std::make_unique<Ellipse>(frame, ShapeDefaults::ELLIPSE_COLOR, ShapeDefaults::BORDER_COLOR);
	case ShapeType::Triangle:
		return std::make_unique<Triangle>(frame, ShapeDefaults::TRIANGLE_COLOR, ShapeDefaults::BORDER_COLOR);
	case ShapeType::Group:
		break;
	}
	throw std::invalid_argument("Неизвестный тип фигуры");
}

void EditorController::StartDrag(const DragMode mode, const int id, const Point point, const ResizeHandle handle)
{
	ResetPreview();
	DragState drag{ mode, handle, point };
	for (const int selectedId : m_state.GetSelectedShapeIds())
	{
		const auto frame = GetShapeFrame(selectedId);
		if (!frame)
		{
			continue;
		}
		if (selectedId == id)
		{
			drag.m_shapeIndex = drag.m_shapeIds.size();
		}
		drag.m_shapeIds.push_back(selectedId);
		drag.m_rootIds.push_back(*m_document.FindRootShapeId(selectedId));
		drag.m_startFrames.push_back(*frame);
	}
	m_drag = std::move(drag);
}

void EditorController::StartCreate(const Point point)
{
	ResetPreview();
	m_drag.m_mode = DragMode::Create;
	m_drag.m_startPoint = point;
	m_drag.m_isCreateForwardX = std::nullopt;
	m_drag.m_isCreateForwardY = std::nullopt;
}

void EditorController::FinishCreate(const Point point)
{
	const bool isMoved = point != m_drag.m_startPoint;
	const ShapeType type = *m_state.GetShapeTool();
	UpdateCreateDirection(point);
	const Frame frame = CalculateCreatedFrame(point);
	ResetPreview();
	if (!isMoved)
	{
		return;
	}

	AddSelectShape(CreateShape(type, frame));
	m_state.SetShapeTool(std::nullopt);
}

void EditorController::FinishDrag(const Point point)
{
	const auto frames = CalculateDraggedFrames(point);
	const DragState drag = std::move(m_drag);
	ResetPreview();
	if (drag.m_mode == DragMode::Move && point == drag.m_startPoint)
	{
		if (!drag.m_shapeIds.empty())
		{
			m_state.SetSelectedShapeIds({ drag.m_clickTarget.value_or(drag.m_shapeIds[drag.m_shapeIndex]) });
		}
		return;
	}

	std::vector<std::unique_ptr<ICommand>> commands;
	for (size_t i = 0; i < frames.size(); ++i)
	{
		if (frames[i] != drag.m_startFrames[i])
		{
			commands.push_back(std::make_unique<ChangeShapeFrameCommand>(m_document, drag.m_shapeIds[i], frames[i]));
		}
	}
	ExecuteCommands(std::move(commands));
}

void EditorController::AddSelectShape(std::unique_ptr<IShape> shape) const
{
	const double duration = m_document.GetDuration();
	const double start = std::min(m_state.GetCurrentTime(), duration - ShapeDefaults::MIN_DURATION);
	shape->SetTimeInterval({ start, duration });
	auto command = std::make_unique<AddShapeCommand>(m_document, std::move(shape));
	const AddShapeCommand& addCommand = *command;
	ExecuteCommand(std::move(command));
	m_state.SetSelectedShapeIds({ *addCommand.GetShapeId() });
}

void EditorController::SelectShapes(const std::vector<int>& ids) const
{
	if (ids.empty())
	{
		return;
	}
	std::vector<int> selectedIds;
	for (const int id : ids)
	{
		if (m_document.FindShape(id))
		{
			selectedIds.push_back(id);
		}
	}
	m_state.SetSelectedShapeIds(std::move(selectedIds));
}

void EditorController::RemoveMissingShapesFromState()
{
	std::vector<int> selectedIds;
	std::optional<int> selectedGroup;
	for (const int id : m_state.GetSelectedShapeIds())
	{
		const auto rootId = m_document.FindRootShapeId(id);
		if (!rootId)
		{
			continue;
		}
		if (selectedIds.empty() && *rootId != id)
		{
			selectedGroup = rootId;
		}
		if (selectedGroup)
		{
			if (*rootId != *selectedGroup || id == *selectedGroup)
			{
				continue;
			}
		}
		else if (id != *rootId)
		{
			continue;
		}
		selectedIds.push_back(id);
	}
	m_state.SetSelectedShapeIds(std::move(selectedIds));

	std::map<int, Frame> previewFrames;
	for (const auto& [id, frame] : m_state.GetPreviewFrames())
	{
		if (m_document.FindShape(id))
		{
			previewFrames.emplace(id, frame);
		}
	}
	m_state.SetPreviewFrames(std::move(previewFrames));

	std::map<int, TimeInterval> previewTimeIntervals;
	for (const auto& [id, timeInterval] : m_state.GetPreviewTimeIntervals())
	{
		if (m_document.FindShape(id))
		{
			previewTimeIntervals.emplace(id, timeInterval);
		}
	}
	m_state.SetPreviewTimeIntervals(std::move(previewTimeIntervals));
}
