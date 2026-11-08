//
// Created by Вадим Патрушев on 08.11.2026.
//

#include "TimelineController.h"
#include "Command/ChangeShapeIndexCommand/ChangeShapeIndexCommand.h"
#include "Command/ChangeShapeTimeIntervalCommand/ChangeShapeTimeIntervalCommand.h"
#include "Command/MacroCommand/MacroCommand.h"
#include "ShapeDefaults.h"
#include <algorithm>
#include <cmath>
#include <limits>

constexpr double TIME_STEPS_PER_SECOND = 100;

TimelineController::TimelineController(Document& document, EditorState& state, CommandHistory& history)
	: m_document(document)
	, m_state(state)
	, m_history(history)
{
	m_document.AddObserver(*this);
}

TimelineController::~TimelineController()
{
	m_document.RemoveObserver(*this);
}

void TimelineController::SetCurrentTime(const double time) const
{
	m_state.SetCurrentTime(std::clamp(time, 0.0, m_document.GetDuration()));
}

void TimelineController::OnMouseDown(const std::optional<size_t> row, const double time, const TimelineDragMode mode,
	const bool isToggle)
{
	Cancel();
	if (mode == TimelineDragMode::Scrub)
	{
		m_mode = mode;
		SetCurrentTime(time);
		return;
	}
	if (!row || *row >= m_document.GetShapeCount())
	{
		if (!isToggle)
		{
			m_state.SetSelectedShapeIds({});
		}
		return;
	}

	const int id = GetShapeIdAtRow(*row);
	if (isToggle)
	{
		m_state.ToggleShapeSelection(id);
		return;
	}
	if (!m_state.IsShapeSelected(id))
	{
		m_state.SetSelectedShapeIds({ id });
	}
	if (mode != TimelineDragMode::None)
	{
		StartDrag(id, time, mode);
	}
}

void TimelineController::OnMouseMove(const size_t slot, const double time)
{
	switch (m_mode)
	{
	case TimelineDragMode::None:
		break;
	case TimelineDragMode::Scrub:
		SetCurrentTime(time);
		break;
	case TimelineDragMode::MoveLayers:
		m_dropSlot = slot;
		break;
	case TimelineDragMode::MoveTime:
	case TimelineDragMode::ResizeStart:
	case TimelineDragMode::ResizeEnd: {
		const auto intervals = CalculateIntervals(time);
		std::map<int, TimeInterval> previewIntervals;
		for (size_t i = 0; i < intervals.size(); ++i)
		{
			previewIntervals.emplace(m_shapeIds[i], intervals[i]);
		}
		m_state.SetPreviewTimeIntervals(std::move(previewIntervals));
		break;
	}
	}
}

void TimelineController::OnMouseUp(const size_t slot, const double time)
{
	switch (m_mode)
	{
	case TimelineDragMode::None:
		break;
	case TimelineDragMode::Scrub:
		SetCurrentTime(time);
		Cancel();
		break;
	case TimelineDragMode::MoveLayers:
		FinishLayersDrag(slot);
		break;
	case TimelineDragMode::MoveTime:
	case TimelineDragMode::ResizeStart:
	case TimelineDragMode::ResizeEnd:
		FinishTimeDrag(time);
		break;
	}
}

void TimelineController::Cancel()
{
	m_mode = TimelineDragMode::None;
	m_shapeIds.clear();
	m_startIntervals.clear();
	m_dropSlot = std::nullopt;
	m_state.SetPreviewTimeIntervals({});
}

TimelineDragMode TimelineController::GetDragMode() const
{
	return m_mode;
}

std::optional<size_t> TimelineController::GetDropSlot() const
{
	return m_mode == TimelineDragMode::MoveLayers ? m_dropSlot : std::nullopt;
}

void TimelineController::DocumentUpdate()
{
	for (const int id : m_shapeIds)
	{
		if (!m_document.FindShape(id))
		{
			Cancel();
			return;
		}
	}
}

void TimelineController::StartDrag(const int id, const double time, const TimelineDragMode mode)
{
	m_mode = mode;
	m_clickedId = id;
	m_startTime = time;
	for (const int selectedId : m_state.GetSelectedShapeIds())
	{
		if (const IShape* shape = m_document.FindShape(selectedId))
		{
			m_shapeIds.push_back(selectedId);
			m_startIntervals.push_back(shape->GetTimeInterval());
		}
	}
}

std::vector<TimeInterval> TimelineController::CalculateIntervals(const double time) const
{
	const double offset = ClampOffset(std::round((time - m_startTime) * TIME_STEPS_PER_SECOND) / TIME_STEPS_PER_SECOND);
	const double duration = m_document.GetDuration();
	std::vector<TimeInterval> intervals;
	intervals.reserve(m_startIntervals.size());
	for (const auto& [start, end] : m_startIntervals)
	{
		TimeInterval interval{ start, end };
		if (m_mode == TimelineDragMode::MoveTime || m_mode == TimelineDragMode::ResizeStart)
		{
			interval.m_start = std::clamp(start + offset, 0.0, duration);
		}
		if (m_mode == TimelineDragMode::MoveTime || m_mode == TimelineDragMode::ResizeEnd)
		{
			interval.m_end = std::clamp(end + offset, 0.0, duration);
		}
		intervals.push_back(interval);
	}
	return intervals;
}

double TimelineController::ClampOffset(const double offset) const
{
	const double duration = m_document.GetDuration();
	double minOffset = std::numeric_limits<double>::lowest();
	double maxOffset = std::numeric_limits<double>::max();
	for (const auto& interval : m_startIntervals)
	{
		switch (m_mode)
		{
		case TimelineDragMode::MoveTime:
			minOffset = std::max(minOffset, -interval.m_start);
			maxOffset = std::min(maxOffset, duration - interval.m_end);
			break;
		case TimelineDragMode::ResizeStart:
			minOffset = std::max(minOffset, -interval.m_start);
			maxOffset = std::min(maxOffset, interval.GetDuration() - ShapeDefaults::MIN_DURATION);
			break;
		case TimelineDragMode::ResizeEnd:
			minOffset = std::max(minOffset, ShapeDefaults::MIN_DURATION - interval.GetDuration());
			maxOffset = std::min(maxOffset, duration - interval.m_end);
			break;
		default:
			return 0;
		}
	}
	if (minOffset > maxOffset)
	{
		return 0;
	}
	return std::clamp(offset, minOffset, maxOffset);
}

void TimelineController::FinishTimeDrag(const double time)
{
	const auto intervals = CalculateIntervals(time);
	const auto shapeIds = m_shapeIds;
	const auto startIntervals = m_startIntervals;
	const TimelineDragMode mode = m_mode;
	const int clickedId = m_clickedId;
	Cancel();

	std::vector<std::unique_ptr<ICommand>> commands;
	for (size_t i = 0; i < intervals.size(); ++i)
	{
		if (intervals[i] != startIntervals[i])
		{
			commands.push_back(std::make_unique<ChangeShapeTimeIntervalCommand>(m_document, shapeIds[i], intervals[i]));
		}
	}
	if (commands.empty() && mode == TimelineDragMode::MoveTime)
	{
		m_state.SetSelectedShapeIds({ clickedId });
		return;
	}
	ExecuteCommands(std::move(commands));
}

void TimelineController::FinishLayersDrag(const size_t slot)
{
	const auto shapeIds = m_shapeIds;
	const int clickedId = m_clickedId;
	const bool isDropped = m_dropSlot.has_value();
	Cancel();

	const auto order = GetLayerOrder();
	const auto newOrder = CalculateLayerOrder(order, shapeIds, slot);
	if (!isDropped || newOrder == order)
	{
		m_state.SetSelectedShapeIds({ clickedId });
		return;
	}
	ExecuteCommands(CreateLayerCommands(order, newOrder));
}

std::vector<int> TimelineController::CalculateLayerOrder(const std::vector<int>& order, const std::vector<int>& movedIds,
	const size_t slot) const
{
	const size_t dropSlot = std::min(slot, order.size());
	std::vector<int> moved;
	std::vector<int> rest;
	size_t insertPosition = 0;
	for (size_t row = 0; row < order.size(); ++row)
	{
		const bool isMoved = std::ranges::find(movedIds, order[row]) != movedIds.end();
		(isMoved ? moved : rest).push_back(order[row]);
		if (!isMoved && row < dropSlot)
		{
			++insertPosition;
		}
	}
	const auto insertIt = rest.begin() + static_cast<std::ptrdiff_t>(insertPosition);
	std::vector<int> newOrder(rest.begin(), insertIt);
	newOrder.insert(newOrder.end(), moved.begin(), moved.end());
	newOrder.insert(newOrder.end(), insertIt, rest.end());
	return newOrder;
}

std::vector<std::unique_ptr<ICommand>> TimelineController::CreateLayerCommands(const std::vector<int>& order,
	const std::vector<int>& newOrder) const
{
	std::vector<int> current(order.rbegin(), order.rend());
	const std::vector<int> target(newOrder.rbegin(), newOrder.rend());
	std::vector<std::unique_ptr<ICommand>> commands;
	for (size_t index = 0; index < target.size(); ++index)
	{
		const auto it = std::ranges::find(current, target[index]);
		if (static_cast<size_t>(it - current.begin()) == index)
		{
			continue;
		}
		commands.push_back(std::make_unique<ChangeShapeIndexCommand>(m_document, target[index], index));
		std::rotate(current.begin() + static_cast<std::ptrdiff_t>(index), it, it + 1);
	}
	return commands;
}

std::vector<int> TimelineController::GetLayerOrder() const
{
	std::vector<int> order;
	order.reserve(m_document.GetShapeCount());
	for (size_t row = 0; row < m_document.GetShapeCount(); ++row)
	{
		order.push_back(GetShapeIdAtRow(row));
	}
	return order;
}

int TimelineController::GetShapeIdAtRow(const size_t row) const
{
	return m_document.GetShapeId(m_document.GetShapeCount() - 1 - row);
}

void TimelineController::ExecuteCommands(std::vector<std::unique_ptr<ICommand>> commands) const
{
	if (commands.empty())
	{
		return;
	}
	m_history.Execute(std::make_unique<MacroCommand>(std::move(commands)));
}
